#pragma once

#include <vector>

// A simple matrix class giving 2-part index (row and column) into a data vector.
template<typename T>
class Matrix
{
public:
    Matrix(std::size_t rows, std::size_t columns) :
            rows_{rows}, columns_{columns},
            data_(rows * columns)
    {}

    std::size_t rows() const
    {
        return rows_;
    }

    std::size_t columns() const
    {
        return columns_;
    }

    T& operator()(std::size_t row, std::size_t column)
    {
        return data_[row * columns_ + column];
    }

    T const& operator()(std::size_t row, std::size_t column) const
    {
        return data_[row * columns_ + column];
    }

private:
    std::size_t rows_, columns_;
    std::vector<T> data_;
};
