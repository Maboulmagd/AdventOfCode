project(2022_Day01)

add_executable(2022_Day01 main.cpp)

# Link against LLVM libraries
target_link_libraries(2022_Day01 PRIVATE ${llvm_libs})