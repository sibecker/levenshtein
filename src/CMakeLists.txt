add_library(levenshtein STATIC levenshtein.cpp calculator.cpp operation.cpp count.cpp result.cpp)
target_include_directories(levenshtein PUBLIC ../include)
target_link_libraries(levenshtein hash)
