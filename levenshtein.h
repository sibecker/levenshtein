#pragma once

#include <iostream>
#include <deque>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>
#include "hash_append.h"

class Levenshtein {
public:
    using SrcChar = std::string;
    using SrcString = std::vector<SrcChar>;
    using DstChar = std::string;
    using DstString = std::vector<DstChar>;

    class Operation {
    public:
        struct Start {};
        struct Match { SrcChar from; DstChar to; };
        struct Change { SrcChar from; DstChar to; };
        struct Insert { DstChar value; };
        struct Delete { SrcChar value; };

        template<typename T>
        /*implicit*/ Operation(T &&op) :
                variant_{std::forward<T &&>(op)}
        {}

        friend std::ostream& operator<<(std::ostream& lhs, Operation const& rhs);

    private:
        std::variant<Start, Match, Change, Insert, Delete> variant_;
    };

    using Transforms = std::unordered_set<std::pair<SrcChar, DstChar>, xstd::uhash<>>;

    struct Result
    {
        std::size_t score;
        std::deque<Operation> operations;
    };

    explicit Levenshtein(Transforms transforms);

    Result operator()(SrcString const& source, DstString const& destination) const;

private:
    Transforms transforms_;
};
