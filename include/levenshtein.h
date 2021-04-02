#pragma once

#include <deque>
#include <iostream>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>
#include "hash_append.h"

// A Levenstein problem to hold the transforms
class Levenshtein {
public:
    using SrcChar = std::string;
    using SrcString = std::vector<SrcChar>;
    using DstChar = std::string;
    using DstString = std::vector<DstChar>;

    struct Count
    {
        std::size_t matches = 0;
        std::size_t changes = 0;
        std::size_t insertions = 0;
        std::size_t deletions = 0;

        std::size_t score() const;
        friend std::ostream& operator<<(std::ostream& lhs, Count const& rhs);
    };

    // Wrapper around std::variant to hold the different types of operation
    class Operation {
    public:
        struct Start {};
        struct Match { SrcChar from; DstChar to; };
        struct Change { SrcChar from; DstChar to; };
        struct Insert { DstChar value; };
        struct Delete { SrcChar value; };

        Operation(Start rhs);
        Operation(Match rhs);
        Operation(Change rhs);
        Operation(Insert rhs);
        Operation(Delete rhs);

        std::size_t cost() const;

        friend std::ostream& operator<<(std::ostream& lhs, Operation const& rhs);

        friend Count& operator+=(Count& lhs, Operation const& rhs);
        friend Count operator+(Count lhs, Operation const& rhs);
        friend Count operator+(Operation const& lhs, Count rhs);

        template<typename Visitor>
        auto visit(Visitor&& visitor) const
        {
            return std::visit(std::forward<Visitor>(visitor), variant_);
        }

    private:
        std::variant<Start, Match, Change, Insert, Delete> variant_;
    };

    // Set of pairs of matchings from the source alphabet to the destination alphabet
    using Transforms = std::unordered_set<std::pair<SrcChar, DstChar>, xstd::uhash<>>;

    // Result class
    class Result
    {
    public:
        explicit Result(std::deque<Operation> operations);

        // The list of operations performed
        std::deque<Operation> const& operations() const;
        // Breakdown of how many of each type of operation is used
        Count count() const;

    private:
        std::deque<Operation> operations_;
        Count count_;
    };

    explicit Levenshtein(Transforms transforms);

    // Calculate the operations used in converting from source to destination
    Result operator()(SrcString const& source, DstString const& destination) const;

private:
    Transforms transforms_;
};
