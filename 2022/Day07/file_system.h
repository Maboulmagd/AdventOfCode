#ifndef AOC_2022_DAY07_FILE_SYSTEM_H
#define AOC_2022_DAY07_FILE_SYSTEM_H

#include <vector>
#include <algorithm>

#include "directory.h"

namespace AOC2022Day07FileSystem {
    class FileSystem final {
    public:
        AOC2022Day07Directory::Directory* root_dir_;
        AOC2022Day07Directory::Directory* curr_dir_;
        std::vector<AOC2022Day07Directory::Directory*> directories_;

        explicit FileSystem();
        ~FileSystem();

        [[nodiscard]] AOC2022Day07Directory::Directory* GetCurrentDirectory() const;

        // Implementation of "cd .. and cd x", also acts as "mkdir" for directories that we don't know about
        void ChangeDirectory(const std::string& dir_name);
    };
}

#endif //AOC_2022_DAY07_FILE_SYSTEM_H
