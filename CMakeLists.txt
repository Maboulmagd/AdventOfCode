cmake_minimum_required(VERSION 3.17)
project (aoc)

set(CMAKE_CXX_STANDARD 20)

if(APPLE)
    include_directories(SYSTEM /usr/local/include /opt/homebrew/include)
    link_directories(/usr/local/lib /opt/homebrew/lib)
endif()

add_subdirectory(2022)