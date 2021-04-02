#pragma once

#include <iostream>

namespace levenshtein {
    // Count of how many of each typeof operation there is
    struct Count {
        std::size_t matches = 0;
        std::size_t changes = 0;
        std::size_t insertions = 0;
        std::size_t deletions = 0;

        std::size_t score() const;

        friend std::ostream &operator<<(std::ostream &lhs, Count const &rhs);
    };
}
