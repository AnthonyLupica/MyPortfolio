# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(lzw435)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

# Set the source files for the project
set(SOURCE_FILES1 lzw435.cpp)

# Add an executable target named lzw435
add_executable(lzw435 ${SOURCE_FILES1})

# Set the source files for the project
set(SOURCE_FILES2 lzw435M.cpp)

# Add an executable target named lzw435M
add_executable(lzw435M ${SOURCE_FILES2})
