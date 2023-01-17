#include "file_system.h"

using namespace AOC2022Day07FileSystem;

FileSystem::FileSystem() : root_dir_(nullptr), curr_dir_(nullptr) {}

FileSystem::~FileSystem() {
    if (root_dir_ != nullptr) {
        delete root_dir_;
        root_dir_ = nullptr;
        curr_dir_ = nullptr;

        std::for_each(directories_.begin(), directories_.end(),[](auto& dir) { dir = nullptr; });
        directories_.clear();
    }
}

AOC2022Day07Directory::Directory* FileSystem::GetCurrentDirectory() const {
    return curr_dir_;
}

// Implementation of "cd .. and cd x", also acts as "mkdir" for directories that we don't know about
void FileSystem::ChangeDirectory(const std::string& dir_name) {
    if (dir_name == "/") {// Root directory
        assert(root_dir_ == nullptr);
        assert(curr_dir_ == nullptr);

        root_dir_ = new AOC2022Day07Directory::Directory(dir_name);// This should be the only location where we call new Directory outside our AddSubDirectory function
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