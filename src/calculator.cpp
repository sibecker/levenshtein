#include "calculator.h"

Calculator::Cell::Cell(Operation operation, Cell const* previous) :
        operation_{std::move(operation)},
        previous_{previous},
        score_{operation_.cost()}
{
    if (previous_)
        score_ += previous_->score_;
}

Calculator::Operation const& Calculator::Cell::operation() const
{
    return operation_;
}

Calculator::Cell const* Calculator::Cell::previous() const
{
    return previous_;
}

std::size_t Calculator::Cell::score() const
{
    return score_;
}

Calculator::Calculator(Transforms const& transforms, SrcString const &source, DstString const& destination) :
        transforms_{transforms},
        source_{source},
        destination_{destination},
        cache_{source.size() + 1, destination.size() + 1}
{
    // Initialise by creating a matrix
    // An entry of nullopt means we haven't calculated this entry yet
    // Otherwise, the entry details how to get from the first i characters of source to the first j characters of destination
    // We prefill all the (i, 0) entries (0, j) as they are terminating cases in the recursion

    // Empty string -> Empty string: nothing to do
    cache_(0, 0) = Cell{Operation::Start{}, nullptr};
    // String -> Empty string: Do as you did for First (i-1) characters of source then delete the ith one
    for (auto i = 1u; i < cache_.rows(); ++i) {
        cache_(i, 0) = Cell{Operation::Delete{source_[i - 1]}, &*cache_(i-1, 0)};
    }
    // Empty String -> String: Do as you did for First (j-1) characters of destination then insert the jth one
    for (auto j = 1u; j < cache_.columns(); ++j) {
        cache_(0, j) = Cell{Operation::Insert{destination_[j - 1]}, &*cache_(0, j - 1)};
    }
}

Calculator::Cell const& Calculator::cell(std::size_t i, std::size_t j)
{
    if (!cache_(i, j)) {
        const auto &src = source_[i - 1];
        const auto &dst = destination_[j - 1];

        // Cumulative score achieved by inserting the jth character from destination
        const auto ins_dst = cell(i, j - 1).score() + 1;
        // Cumulative score achieved by deleting the ith character from source
        const auto del_src = cell(i - 1, j).score() + 1;

        // Cost of changing the ith character from source to the jth character of source
        // This is 0 if there is a matching transform from src to dst and 1 if not
        const auto change_cost = transforms_.count({src, dst}) ? 0 : 1;

        // Cumulative score of changing the ith character from source into the jth character of source
        const auto change = cell(i - 1, j - 1).score() + change_cost;
        const auto score = std::min(std::min(del_src, ins_dst), change);
        if (score == ins_dst) {
            // If inserting is (joint) best, choose that
            cache_(i, j) = Cell{Operation::Insert{dst}, &cell(i,j-1)};
        } else if (score == del_src) {
            // Otherwise, if deleting is (joint) best, choose that
            cache_(i, j) = Cell{Operation::Delete{src}, &cell(i-1, j)};
        } else if (change_cost) {
            // Otherwise, changing must be best, but it depends wheher there is a cost to changing as to whether we
            // consider this a change or a match
            cache_(i, j) = Cell{Operation::Change{src, dst}, &cell(i-1,j-1)};
        } else {
            cache_(i, j) = Cell{Operation::Match{src, dst}, &cell(i-1,j-1)};
        }
    }

    return *cache_(i, j);
}
