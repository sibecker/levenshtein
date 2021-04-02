#include <optional>
#include "levenshtein/levenshtein.h"
#include "levenshtein/operation.h"

using namespace std::literals;

namespace levenshtein
{
    Operation::Operation(Start rhs) :
        variant_{std::move(rhs)}
    {}

    Operation::Operation(Match rhs) :
        variant_{std::move(rhs)}
    {}

    Operation::Operation(Change rhs) :
        variant_{std::move(rhs)}
    {}

    Operation::Operation(Insert rhs) :
        variant_{std::move(rhs)}
    {}

    Operation::Operation(Delete rhs) :
        variant_{std::move(rhs)}
    {}

    std::size_t Operation::cost() const
    {
        return std::visit([](auto const &op) { return op.cost(); }, variant_);
    }
    std::size_t Start::cost() const { return 0u; }
    std::size_t Match::cost() const { return 0u; }
    std::size_t Change::cost() const { return 1u; }
    std::size_t Insert::cost() const { return 1u; }
    std::size_t Delete::cost() const { return 1u; }

    std::optional<SrcChar> Operation::src() const
    {
        return std::visit([](auto const &op) -> std::optional<SrcChar> { return op.src(); }, variant_);
    }
    std::nullopt_t Start::src() const { return std::nullopt; }
    SrcChar Match::src() const { return from; }
    SrcChar Change::src() const { return from; }
    std::nullopt_t Insert::src() const { return std::nullopt; }
    SrcChar Delete::src() const { return value; }

    std::optional<DstChar> Operation::dst() const
    {
        return std::visit([](auto const &op) -> std::optional<DstChar> { return op.dst(); }, variant_);
    }
    std::nullopt_t Start::dst() const { return std::nullopt; }
    DstChar Match::dst() const { return to; }
    DstChar Change::dst() const { return to; }
    DstChar Insert::dst() const { return value; }
    std::nullopt_t Delete::dst() const { return std::nullopt; }

    std::ostream &operator<<(std::ostream& lhs, Operation const& rhs)
    {
        std::visit([&lhs](auto const &op) { lhs << op; }, rhs.variant_);
        return lhs;
    }
    std::ostream& operator<<(std::ostream& lhs, Start const&)
    {
        return lhs << "Start"s;
    }
    std::ostream& operator<<(std::ostream& lhs, Match const& rhs)
    {
        return lhs << "Match\t"s << rhs.from << "\twith\t"s << rhs.to;
    }
    std::ostream& operator<<(std::ostream& lhs, Change const& rhs)
    {
        return lhs << "Change\t"s << rhs.from << "\tinto\t"s << rhs.to;
    }
    std::ostream& operator<<(std::ostream& lhs, Insert const& rhs)
    {
        return lhs << "Insert\t\t    \t"s << rhs.value;
    }
    std::ostream& operator<<(std::ostream& lhs, Delete const& rhs)
    {
        return lhs << "Delete\t"s << rhs.value;
    }

    Count& operator+=(Count& lhs, Operation const& rhs)
    {
        std::visit([&lhs](auto const &op) { lhs += op; }, rhs.variant_);
        return lhs;
    }
    Count& operator+=(Count& lhs, Start const&)
    {
        return lhs;
    }
    Count& operator+=(Count& lhs, Match const&)
    {
        ++lhs.matches;
        return lhs;
    }
    Count& operator+=(Count& lhs, Change const &)
    {
        ++lhs.changes;
        return lhs;
    }
    Count& operator+=(Count& lhs, Insert const &)
    {
        ++lhs.insertions;
        return lhs;
    }
    Count& operator+=(Count& lhs, Delete const &)
    {
        ++lhs.deletions;
        return lhs;
    }

    Count operator+(Count lhs, Operation const& rhs)
    {
        return lhs += rhs;
    }

    Count operator+(Operation const& lhs, Count rhs)
    {
        return rhs += lhs;
    }
}
