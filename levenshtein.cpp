#include "calculator.h"

using namespace std::literals;

namespace
{
    class StreamTo {
    public:
        StreamTo(std::ostream& stream) :
            stream_{stream}
        {}

        std::ostream& operator()(Levenshtein::Operation::Start const &)
        {
            return stream_ << "Start"s;
        }

        std::ostream& operator()(Levenshtein::Operation::Match const &op)
        {
            return stream_ << "Match\t"s << op.from << "\twith\t"s << op.to;
        }

        std::ostream& operator()(Levenshtein::Operation::Change const &op)
        {
            return stream_ << "Change\t"s << op.from << "\tinto\t"s << op.to;
        }

        std::ostream& operator()(Levenshtein::Operation::Insert const &op)
        {
            return stream_ << "Insert\t\t    \t"s << op.value;
        }

        std::ostream& operator()(Levenshtein::Operation::Delete const &op)
        {
            return stream_ << "Delete\t"s << op.value;
        }

    private:
        std::ostream& stream_;
    };
}

std::ostream& operator<<(std::ostream& lhs, Levenshtein::Operation const& rhs)
{
    return std::visit(StreamTo{lhs}, rhs.variant_);
}

Levenshtein::Levenshtein(Transforms transforms) :
    transforms_{std::move(transforms)}
{}

Levenshtein::Result Levenshtein::operator()(SrcString const& source, DstString const& destination) const
{
    std::deque<Operation> operations;
    auto calculator = Calculator{transforms_, source, destination};
    auto const& result = calculator.node(source.size(), destination.size());
    for(auto const* node = &result; node != nullptr; node = node->previous)
        operations.push_front(node->operation);

    return {result.score, operations};
}
