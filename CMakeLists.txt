cmake_minimum_required(VERSION 3.23)

#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(aoc)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")# using regular Clang OR AppleClang
    set(CMAKE_CXX_FLAGS -stdlib=libc++)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")# using GCC
    set(CMAKE_CXX_FLAGS -stdlib=libstdc++)
endif()

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(2021)
add_subdirectory(2022)