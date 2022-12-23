project(2022_Day05)

add_executable(2022_Day05 main.cpp)

# Link against LLVM libraries
target_link_libraries(2022_Day05 PRIVATE ${llvm_libs})

