#include <numeric>
#include "levenshtein/result.h"

namespace levenshtein {
    Result::Result(std::deque<Operation> operations) :
            operations_{std::move(operations)},
            count_{std::accumulate(std::begin(operations_), std::end(operations_), Count{})} {}

    std::deque<Operation> const &Result::operations() const {
        return operations_;
    }

    Count Result::count() const {
        return count_;
    }
}
