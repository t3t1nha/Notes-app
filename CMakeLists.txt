cmake_minimum_required(VERSION 3.16)
project(notes_app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable (assuming your file is named main.cpp)
add_executable(notes_app src/main.cpp)

# Since json.hpp is a single header file, just include the current directory
target_include_directories(notes_app PRIVATE .)