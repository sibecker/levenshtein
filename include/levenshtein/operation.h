#pragma once

#include "levenshtein/count.h"
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace levenshtein
{
    using SrcChar = std::string;
    using SrcString = std::vector<SrcChar>;
    using DstChar = std::string;
    using DstString = std::vector<DstChar>;

    struct Start
    {
        std::size_t cost() const;
        std::nullopt_t src() const;
        std::nullopt_t dst() const;

        friend std::ostream& operator<<(std::ostream& lhs, Start const& rhs);
        friend Count& operator+=(Count& lhs, Start const& rhs);
    };


    struct Match
    {
        std::size_t cost() const;
        SrcChar src() const;
        DstChar dst() const;

        friend std::ostream& operator<<(std::ostream& lhs, Match const& rhs);
        friend Count& operator+=(Count& lhs, Match const& rhs);

        SrcChar from;
        DstChar to;
    };

    struct Change
    {
        std::size_t cost() const;
        SrcChar src() const;
        DstChar dst() const;

        friend std::ostream& operator<<(std::ostream& lhs, Change const& rhs);
        friend Count& operator+=(Count& lhs, Change const& rhs);

        SrcChar from;
        DstChar to;
    };

    struct Insert
    {
        std::size_t cost() const;
        std::nullopt_t src() const;
        DstChar dst() const;

        friend std::ostream& operator<<(std::ostream& lhs, Insert const& rhs);
        friend Count& operator+=(Count& lhs, Insert const& rhs);

        DstChar value;
    };

    struct Delete
    {
        std::size_t cost() const;
        SrcChar src() const;
        std::nullopt_t dst() const;

        friend std::ostream& operator<<(std::ostream& lhs, Delete const& rhs);
        friend Count& operator+=(Count& lhs, Delete const& rhs);

        SrcChar value;
    };

    // Wrapper around std::variant to hold the different types of operation
    class Operation {
    public:
        Operation(Start rhs);
        Operation(Match rhs);
        Operation(Change rhs);
        Operation(Insert rhs);
        Operation(Delete rhs);

        std::size_t cost() const;
        std::optional<SrcChar> src() const;
        std::optional<DstChar> dst() const;

        friend std::ostream &operator<<(std::ostream& lhs, Operation const& rhs);
        friend Count& operator+=(Count &lhs, Operation const& rhs);
        friend Count operator+(Count lhs, Operation const& rhs);
        friend Count operator+(Operation const& lhs, Count rhs);

    private:
        std::variant<Start, Match, Change, Insert, Delete> variant_;
    };
}
