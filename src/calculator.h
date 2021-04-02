#pragma once

#include <optional>
#include "levenshtein.h"
#include "matrix.h"

// Levenshtein distance calculator
class Calculator {
public:
    using Operation = Levenshtein::Operation;
    using Transforms = Levenshtein::Transforms;
    using SrcString = Levenshtein::SrcString;
    using DstString = Levenshtein::DstString;

    // Initialise the calculation
    Calculator(Transforms const& transforms, SrcString const &source, DstString const& destination);

    // Cell class for storing in the matrix used in calculation
    class Cell
    {
    public:
        Cell(Operation operation, Cell const* previous);

        Operation const& operation() const;
        Cell const* previous() const;
        std::size_t score() const;

    private:
        Operation operation_; // operation performed to reach this cell from the previous one
        Cell const* previous_; // previous cell
        std::size_t score_; // Number of non-match operations required to reach this cell
    };

    // Recursive function that gets the node in the grid (calculating and caching) as required.
    Cell const& cell(std::size_t i, std::size_t j);

private:
    Transforms const &transforms_;
    SrcString const &source_;
    DstString const &destination_;
    Matrix<std::optional<Cell>> cache_;
};