cmake_minimum_required(VERSION 3.0)
project(levenshtein)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(hash)

add_executable(levenshtein main.cpp levenshtein.cpp calculator.cpp)
target_link_libraries(levenshtein hash)
