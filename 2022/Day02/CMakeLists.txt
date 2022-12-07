project(Day02)

add_executable(Day02 main.cpp)
target_compile_options(Day02 PRIVATE -std=c++20 -stdlib=libc++)


#add_library(include include.h include.cpp)
#target_link_libraries(include PUBLIC gmock_main)
#
#target_include_directories(include PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})