#include "file.h"

using namespace AOC2022Day07File;

File::File(const std::string& name) : name_(name), size_(0) {}
File::File(const std::string& name, const std::size_t size) : name_(name), size_(size) {}