add_subdirectory(Catch2)
add_executable(test matrix.cpp)
target_include_directories(test PRIVATE Catch2/include)
target_link_libraries(test PRIVATE Catch2::Catch2WithMain levenshtein)

add_test(NAME test COMMAND test)