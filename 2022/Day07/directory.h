#ifndef AOC_2022_DAY07_DIRECTORY_H
#define AOC_2022_DAY07_DIRECTORY_H

#include <unordered_map>
#include <ranges>
#include <numeric>
#include <cassert>

#include "file.h"

namespace AOC2022Day07Directory {
    class Directory final : public AOC2022Day07File::File {
    public:
        std::unordered_map<std::string, Directory*> sub_directories_;// Required for "$ cd x"
        std::unordered_map<std::string, File*> files_;// Required for files

        Directory* parent_dir_;// Required for "$ cd .."

        explicit Directory(const std::string& dir_name);
        ~Directory();

        std::size_t GetRecursiveSize();

        File* AddFile(const std::string& file_name, const std::size_t file_size);
        Directory* AddSubDirectory(const std::string& dir_name);
    };
}

#endif //AOC_2022_DAY07_DIRECTORY_H
