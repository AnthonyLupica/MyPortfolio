# @file CMakeLists.txt
# 
# CMake files for nqueens (Artificial Intelligence Project 1)

# Can be run by any version of CMake >= this
cmake_minimum_required(VERSION 3.22.1)

# project info
project(Treap)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)

# Source files for the main program main.cpp (using the CuckooHash class)
set(SOURCE main.cpp treap.cpp)

# convert application
add_executable(treap ${SOURCE})

# Turn on warnings
if (MSVC)
    # warning level 4
    add_compile_options(/W4)
else()
    # standard warnings
    add_compile_options(-Wall)
endif()

# run command
add_custom_target(run
        COMMENT "Run"
        COMMAND $<TARGET_FILE:treap>
        DEPENDS treap
        USES_TERMINAL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
