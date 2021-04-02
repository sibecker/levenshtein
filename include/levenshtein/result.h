#pragma once

#include <deque>
#include "levenshtein/operation.h"

namespace levenshtein
{
    // Result of a levenshtein calculation
    class Result {
    public:
        explicit Result(std::deque<Operation> operations);

        // The list of operations performed
        std::deque<Operation> const &operations() const;

        // Breakdown of how many of each type of operation is used
        Count count() const;

    private:
        std::deque<Operation> operations_;
        Count count_;
    };
}
