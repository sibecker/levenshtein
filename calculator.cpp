#include "calculator.h"

Calculator::Calculator(Transforms const& transforms, SrcString const &source, DstString const& destination) :
        transforms_{transforms},
        source_{source},
        destination_{destination},
        cache_{source.size() + 1, destination.size() + 1}
{
    cache_(0, 0) = Node{0, Operation::Start{}, nullptr};
    for (auto i = 1u; i < cache_.rows(); ++i)
        cache_(i, 0) = Node{i, Operation::Delete{source_[i - 1]}, &*cache_(i-1, 0)};
    for (auto j = 1u; j < cache_.columns(); ++j)
        cache_(0, j) = Node{j, Operation::Insert{destination_[j - 1]}, &*cache_(0, j-1)};
}

Calculator::Node const& Calculator::node(std::size_t i, std::size_t j) const
{
    if (!cache_(i, j)) {
        const auto ins_dst = node(i, j - 1).score + 1;
        const auto del_src = node(i - 1, j).score + 1;
        const auto &from = source_[i - 1];
        const auto &to = destination_[j - 1];
        const auto change_cost = transforms_.count({from, to}) ? 0 : 1;
        const auto change = node(i - 1, j - 1).score + change_cost;
        const auto score = std::min(std::min(del_src, ins_dst), change);
        if (change == score) {
            if(change_cost) {
                cache_(i, j) = Node{score, Operation::Change{from, to}, &node(i-1,j-1)};
            } else {
                cache_(i, j) = Node{score, Operation::Match{from, to}, &node(i-1,j-1)};
            }
        } else if (score == ins_dst) {
            cache_(i, j) = Node{score, Operation::Insert{to}, &node(i,j-1)};
        } else {
            cache_(i, j) = Node{score, Operation::Delete{from}, &node(i-1, j)};
        }
    }

    return *cache_(i, j);
}
