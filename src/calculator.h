#pragma once

#include <optional>
#include "levenshtein.h"
#include "matrix.h"

class Calculator {
public:
    using Operation = Levenshtein::Operation;
    using Transforms = Levenshtein::Transforms;
    using SrcString = Levenshtein::SrcString;
    using DstString = Levenshtein::DstString;

    struct Node
    {
        std::size_t score;
        Operation operation;
        Node const* previous;
    };

    Calculator(Transforms const& transforms, SrcString const &source, DstString const& destination);

    Node const& node(std::size_t i, std::size_t j) const;

private:
    Transforms const &transforms_;
    SrcString const &source_;
    DstString const &destination_;
    mutable Matrix<std::optional<Node>> cache_;
};