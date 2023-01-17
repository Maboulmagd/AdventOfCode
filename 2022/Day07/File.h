#ifndef AOC_2022_Day07_FILE_H
#define AOC_2022_Day07_FILE_H

#include <string>

namespace AOC2022Day07File {
class File {
public:
    std::string name_;
    std::size_t size_;

    explicit File(const std::string& name);
    explicit File(const std::string& name, std::size_t size);
    virtual ~File() = default;// Need a virtual destructor
};
}

#endif //AOC_2022_Day07_FILE_H
