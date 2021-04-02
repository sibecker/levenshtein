# cmake_minimum_required(VERSION 3.0)
project(levenshtein)

set(CMAKE_CXX_STANDARD 17)

include_directories(hash)
add_executable(levenshtein main.cpp hash/siphash.cpp)