#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <ranges>
#include <numeric>

struct File {
    std::string name_;
    std::size_t size_;

    explicit File(const std::string& name) : name_(name), size_(0) {}
    explicit File(const std::string& name, const std::size_t size) : name_(name), size_(size) {}
    virtual ~File() = default;// Need a virtual destructor
};

class Directory final : public File {
public:
    std::unordered_map<std::string, Directory*> sub_directories_;// Required for "$ cd x"
    std::unordered_map<std::string, File*> files_;// Required for files

    Directory* parent_dir_;// Required for "$ cd .."

    explicit Directory(const std::string& dir_name) : File(dir_name), parent_dir_(nullptr) {
    }
    ~Directory() {
        // Delete all subdirectories and files recursively
    }

    std::size_t GetRecursiveSize() {
        // Previously visited/computed this directory's recursive size, simply return recursive size.
        // This only works in this case, since we don't add/remove files or directories! Once we create a directory, it's there forever.
        if (size_ != 0) {
            return size_;
        }

        // Sizes of files in this directory
        const auto files_sizes = files_ |
                std::views::transform([](const std::pair<std::string, File*>& file) { return file.second->size_; }) |
                std::views::common;

        // Size of subdirectories in this directory
        const auto dir_recursive_sizes = sub_directories_ |
                std::views::transform([](const std::pair<std::string, Directory*>& directory) { return directory.second; }) |
                std::views::transform(&Directory::size_) |
                std::views::common;

        const auto sum = std::reduce(files_sizes.begin(), files_sizes.end()) + std::reduce(dir_recursive_sizes.begin(), dir_recursive_sizes.end());
        size_ = sum;// Cache the sum
        return sum;
    }

    File* AddFile(const std::string& file_name, const std::size_t file_size) {
        File* file = new File(file_name, file_size);
        assert(file != nullptr);
        files_[file_name] = file;// Add to files map
        return file;
    }

    Directory* AddSubDirectory(const std::string& dir_name) {
        Directory* dir = new Directory(dir_name);
        assert(dir != nullptr);
        dir->parent_dir_ = this;// Set parent directory to this
        sub_directories_[dir_name] = dir;// Add to subdirectories map
        return dir;
    }
};

class FileSystem final {
public:
    Directory* root_dir_;
    Directory* curr_dir_;
    std::vector<Directory*> directories_;

    explicit FileSystem() : root_dir_(nullptr), curr_dir_(nullptr) {
    }

    ~FileSystem() {
        if (root_dir_ != nullptr) {
            delete root_dir_;
        }
    }

    Directory* GetCurrentDirectory() const {
        return curr_dir_;
    }

    // Implementation of "cd .. and cd x", also acts as "mkdir" for directories that we don't know about
    void ChangeDirectory(const std::string& dir_name) {
        if (dir_name == "/") {// Root directory
            assert(root_dir_ == nullptr);
            assert(curr_dir_ == nullptr);

            root_dir_ = new Directory(dir_name);// This should be the only location where we call new Directory outside our AddSubDirectory function
            assert(root_dir_->parent_dir_ == nullptr);
            curr_dir_ = root_dir_;

            directories_.push_back(root_dir_);
        }
        else if (dir_name == "..") {// Go back one directory up
            assert(curr_dir_ != nullptr);
            assert(curr_dir_->parent_dir_ != nullptr);
            assert(curr_dir_ != root_dir_); // If we are at the root directory, we can't go back up one level

            // Update current directory
            curr_dir_ = curr_dir_->parent_dir_;
            assert(curr_dir_ != nullptr);
        }
        else if (curr_dir_->sub_directories_.contains(dir_name)) {// If subdirectory already exists, cd into it
            curr_dir_ = curr_dir_->sub_directories_[dir_name];
            assert(curr_dir_ != nullptr);
        }
        else {// Act as if this is a "$ mkdir ${dir_name}" command, AKA create the subdirectory, and cd into it
            curr_dir_->AddSubDirectory(dir_name);
            directories_.push_back(curr_dir_->sub_directories_[dir_name]);

            // Update current directory
            curr_dir_ = curr_dir_->sub_directories_[dir_name];
            assert(curr_dir_ != nullptr);
        }
    }
};

std::istream& operator>>(std::istream& s, Directory* curr_dir) {
    assert(curr_dir != nullptr);

    // Read until the next "$ ls", or "$ cmd", or end of input
    while (s.peek() != '$' && s) {
        if (std::isdigit(s.peek())) {// Parse file
            std::size_t file_size;
            std::string file_name;

            s >> file_size >> file_name;

            // Make sure file was not already created
            if (!curr_dir->files_.contains(file_name)) {
                curr_dir->AddFile(file_name, file_size);
            }

            // Skip any trailing whitespace
            while (std::isspace(s.peek())) {
                s.get();
            }
        }
        else {// Since we handle directory creation in FileSystem::ChangeDirectory(), we can ignore output of directories from "$ ls" commands
            std::string line;
            std::getline(s, line);
        }
    }

    return s;
}

FileSystem* ParseOutput(std::istream& s) {
    FileSystem* fs = new FileSystem();
    //std::skipws(s);// Skip trailing whitespace (if any)

    while (s.peek() == '$') {
        char c;
        std::string command;
        s >> c >> command;// Read '$' and either 'cd' or 'ls' command
        if (command == "cd") {
            std::string dir_name;
            s >> dir_name;
            fs->ChangeDirectory(dir_name);

            // Skip any trailing whitespace
            while (std::isspace(s.peek())) {
                s.get();
            }
        }
        else if (command == "ls") {
            s >> fs->GetCurrentDirectory();
        }
    }

    return fs;
}

std::size_t TraverseAndSum(Directory* directory) {
    assert(directory != nullptr);

    // Visit subdirectories and sum sizes
    auto directory_recursive_size = directory->sub_directories_ |
            std::views::transform([](const std::pair<std::string, Directory*>& directory) { return directory.second; }) |
            std::views::transform(TraverseAndSum) |
            std::views::common;
    std::size_t sum = std::reduce(directory_recursive_size.begin(), directory_recursive_size.end());

    // If this directory has a recursive size less than our size_offset, add it to sum
    if (directory->GetRecursiveSize() <= 100000) {// TODO If I want to use above, I can't make this a (default) argument to TraverseAndSum, or can I?
        sum += directory->GetRecursiveSize();// Never computing twice, recall we cache recursive size
    }

    return sum;
}

int ParseAndRun(const std::string& path) {
    std::fstream file_name(path);

    if (!file_name.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    FileSystem* fs = ParseOutput(file_name);

    std::cout << TraverseAndSum(fs->root_dir_) << std::endl;


    delete fs;// Delete our file system, could use smart pointers to avoid this crap

    return 0;
}

int Test() {
    std::stringstream s(R"($ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k)");
    FileSystem* file_system = ParseOutput(s);
    assert(TraverseAndSum(file_system->root_dir_) == 95437);
//  std::size_t used = file_system->root_dir_->GetRecursiveSize();
//    if (70000000 - used < 30000000) {
//        std::size_t goal = 30000000 - (70000000 - used);
//        assert(find_smallest_but_sufficient(&tree->root, goal) == 24933642);
//    }

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}