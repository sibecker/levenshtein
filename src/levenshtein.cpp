#include <numeric>
#include "calculator.h"

using namespace std::literals;

namespace
{
    struct Cost {
        std::size_t operator()(Levenshtein::Operation::Start const&)
        {
            return 0u;
        }

        std::size_t operator()(Levenshtein::Operation::Match const&)
        {
            return 0u;
        }

        std::size_t operator()(Levenshtein::Operation::Change const&)
        {
            return 1u;
        }

        std::size_t operator()(Levenshtein::Operation::Insert const&)
        {
            return 1u;
        }

        std::size_t operator()(Levenshtein::Operation::Delete const&)
        {
            return 1u;
        }
    };

    class StreamTo {
    public:
        explicit StreamTo(std::ostream& stream) :
            stream_{stream}
        {}

        void operator()(Levenshtein::Operation::Start const&)
        {
            stream_ << "Start"s;
        }

        void operator()(Levenshtein::Operation::Match const& op)
        {
            stream_ << "Match\t"s << op.from << "\twith\t"s << op.to;
        }

        void operator()(Levenshtein::Operation::Change const& op)
        {
            stream_ << "Change\t"s << op.from << "\tinto\t"s << op.to;
        }

        void operator()(Levenshtein::Operation::Insert const& op)
        {
            stream_ << "Insert\t\t    \t"s << op.value;
        }

        void operator()(Levenshtein::Operation::Delete const& op)
        {
            stream_ << "Delete\t"s << op.value;
        }

    private:
        std::ostream& stream_;
    };

    class Increment {
    public:
        explicit Increment(Levenshtein::Count& count) :
                count_{count}
        {}

        void operator()(Levenshtein::Operation::Start const&)
        {
        }

        void operator()(Levenshtein::Operation::Match const& op)
        {
            ++count_.matches;
        }

        void operator()(Levenshtein::Operation::Change const& op)
        {
            ++count_.changes;
        }

        void operator()(Levenshtein::Operation::Insert const& op)
        {
            ++count_.insertions;
        }

        void operator()(Levenshtein::Operation::Delete const& op)
        {
            ++count_.deletions;
        }

    private:
        Levenshtein::Count& count_;
    };
}

std::size_t Levenshtein::Count::score() const
{
    return changes + insertions + deletions;
}

std::ostream& operator<<(std::ostream& lhs, Levenshtein::Count const& rhs)
{
    return lhs <<
        "Matches:    " << rhs.matches << std::endl <<
        "Changes:    " << rhs.changes << std::endl <<
        "Insertions: " << rhs.insertions << std::endl <<
        "Deletions:  " << rhs.deletions << std::endl <<
        "(" << rhs.score() << " non-match operations)";
}

Levenshtein::Operation::Operation(Start rhs) :
    variant_{std::move(rhs)}
{}

Levenshtein::Operation::Operation(Match rhs) :
    variant_{std::move(rhs)}
{}

Levenshtein::Operation::Operation(Change rhs) :
    variant_{std::move(rhs)}
{}

Levenshtein::Operation::Operation(Insert rhs) :
    variant_{std::move(rhs)}
{}

Levenshtein::Operation::Operation(Delete rhs) :
    variant_{std::move(rhs)}
{}


std::size_t Levenshtein::Operation::cost() const
{
    return visit(Cost{});
}

std::ostream& operator<<(std::ostream& lhs, Levenshtein::Operation const& rhs)
{
    rhs.visit(StreamTo{lhs});
    return lhs;
}

Levenshtein::Count& operator+=(Levenshtein::Count& lhs, Levenshtein::Operation const& rhs)
{
    rhs.visit(Increment{lhs});
    return lhs;
}
Levenshtein::Count operator+(Levenshtein::Count lhs, Levenshtein::Operation const& rhs)
{
    return lhs += rhs;
}
Levenshtein::Count operator+(Levenshtein::Operation const& lhs, Levenshtein::Count rhs)
{
    return rhs += lhs;
}

Levenshtein::Result::Result(std::deque<Operation> operations) :
        operations_{std::move(operations)},
        count_{std::accumulate(std::begin(operations_), std::end(operations_), Count{})}
{}

std::deque<Levenshtein::Operation> const& Levenshtein::Result::operations() const
{
    return operations_;
}

Levenshtein::Count Levenshtein::Result::count() const
{
    return count_;
}

Levenshtein::Levenshtein(Transforms transforms) :
    transforms_{std::move(transforms)}
{}

Levenshtein::Result Levenshtein::operator()(SrcString const& source, DstString const& destination) const
{
    std::deque<Operation> operations;
    auto calculator = Calculator{transforms_, source, destination};

    // The end result is in the (m, n)th cell of the calculator
    auto const& resultCell = calculator.cell(source.size(), destination.size());
    for(auto const* cell = &resultCell; cell->previous() != nullptr; cell = cell->previous())
        operations.push_front(cell->operation());

    return Result{operations};
}
