cmake_minimum_required(VERSION 3.17)

project (aoc)

set(CMAKE_PREFIX_PATH /Users/Mo/Coding/AOC/vcpkg/packages/llvm_x64-osx)

find_package(LLVM CONFIG REQUIRED)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

message(STATUS "LLVM include dir: ${LLVM_INCLUDE_DIRS}")
message(STATUS "LLVM libraries dir: ${LLVM_LIBRARY_DIRS}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")

list(APPEND CMAKE_MODULE_PATH "${LLVM_DIR}")
include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

option(LLVM_LINK_DYLIB "Link with LLVM dynamically" ON)
if(LLVM_LINK_DYLIB)
    message(STATUS "LLVM linking: dynamic")
    execute_process(COMMAND ./tools/llvm/llvm-config --libs all OUTPUT_VARIABLE llvm_libs)
else()
    message(STATUS "LLVM linking: static")
    # Find the libraries that correspond to the LLVM components that we wish to use
    llvm_map_components_to_libnames(llvm_libs bitwriter core irreader linker support)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -stdlib=libc++)

add_subdirectory(2021)
add_subdirectory(2022)