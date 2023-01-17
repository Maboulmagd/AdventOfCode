#include <fstream>
#include <iostream>
#include <iomanip>

#include "file_system.h"

using namespace AOC2022Day07FileSystem;
using namespace AOC2022Day07Directory;

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
    if (directory->GetRecursiveSize() <= 100'000) {// TODO If I want to use above, I can't make this a (default) argument to TraverseAndSum, or can I?
        sum += directory->GetRecursiveSize();// Never computing twice, recall we cache recursive size
    }

    return sum;
}

std::size_t SmallestLargeEnoughDirectoryToDelete(Directory* dir, const std::size_t space_to_free) {
    assert(dir != nullptr);

    // Traverse subdirectories, and filter directories whose size (recursive size, that is) is not large enough
    auto dir_sizes_greater_than_target = dir->sub_directories_ |
                                         std::views::transform([](const std::pair<std::string, Directory*>& directory) { return directory.second; }) |
                                         std::views::transform([&space_to_free](Directory* current) { return SmallestLargeEnoughDirectoryToDelete(current, space_to_free); }) |
                                         std::views::filter([&space_to_free](const std::size_t dir_recursive_size) { return dir_recursive_size >= space_to_free; });

    // If none of the subdirectories' is large enough, just return the size of the current directory.
    if (dir_sizes_greater_than_target.empty()) {
        return dir->GetRecursiveSize();
    }

    // Return smallest subdirectory that is large enough
    return std::ranges::min(dir_sizes_greater_than_target);
}

int ParseAndRun(const std::string& path) {
    std::fstream file_name(path);

    if (!file_name.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    FileSystem* fs = ParseOutput(file_name);

    std::cout << TraverseAndSum(fs->root_dir_) << std::endl;

    const std::size_t size_required_for_update = 30'000'000;
    const std::size_t used_space = fs->root_dir_->GetRecursiveSize();
    const std::size_t unused_space = 70'000'000 - used_space;

    if (unused_space >= size_required_for_update) {// Don't need to delete any directory
        std::cout << 0 << std::endl;
    }
    else {
        assert(unused_space < size_required_for_update);
        const std::size_t space_to_free = size_required_for_update - unused_space;
        std::cout << SmallestLargeEnoughDirectoryToDelete(fs->root_dir_, space_to_free) << std::endl;
    }

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

    // Part one
    FileSystem* file_system = ParseOutput(s);
    assert(TraverseAndSum(file_system->root_dir_) == 95'437);

    // Part two
    const std::size_t used = file_system->root_dir_->GetRecursiveSize();
    if (70000000 - used < 30000000) {
        const std::size_t goal = 30000000 - (70000000 - used);
        assert(SmallestLargeEnoughDirectoryToDelete(file_system->root_dir_, goal) == 24933642);
    }

    delete file_system;

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}