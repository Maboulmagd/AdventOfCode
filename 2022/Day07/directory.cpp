#include "directory.h"

using namespace AOC2022Day07Directory;
using namespace AOC2022Day07File;

Directory::Directory(const std::string& dir_name) : File(dir_name), parent_dir_(nullptr) {}
Directory::~Directory() {
    // Delete all files
    for (auto& [_, file] : files_) {
        delete file;
        file = nullptr;
    }
    files_.clear();

    // Delete all subdirectories recursively
    for (auto& [_, sub_dir] : sub_directories_) {
        delete sub_dir;
        sub_dir = nullptr;
    }
    sub_directories_.clear();
}

std::size_t Directory::GetRecursiveSize() {
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

File* Directory::AddFile(const std::string& file_name, const std::size_t file_size) {
    File* file = new File(file_name, file_size);
    assert(file != nullptr);
    files_[file_name] = file;// Add to files map
    return file;
}

Directory* Directory::AddSubDirectory(const std::string& dir_name) {
    Directory* dir = new Directory(dir_name);
    assert(dir != nullptr);
    dir->parent_dir_ = this;// Set parent directory to this
    sub_directories_[dir_name] = dir;// Add to subdirectories map
    return dir;
}