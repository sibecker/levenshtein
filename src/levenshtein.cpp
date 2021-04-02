#include <numeric>
#include "levenshtein/result.h"
#include "calculator.h"

namespace levenshtein
{
    Levenshtein::Levenshtein(Transforms transforms) :
            transforms_{std::move(transforms)} {}

    Result Levenshtein::operator()(SrcString const &source, DstString const &destination) const {
        std::deque<Operation> operations;
        auto calculator = Calculator{transforms_, source, destination};

        // The end result is in the (m, n)th cell of the calculator
        auto const &resultCell = calculator.cell(source.size(), destination.size());
        for (auto const *cell = &resultCell; cell->previous() != nullptr; cell = cell->previous())
            operations.push_front(cell->operation());

        return Result{operations};
    }
}
