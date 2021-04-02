#include "catch.hpp"
#include "matrix.h"

TEST_CASE("Matrix")
{
    SECTION("Empty")
    {
        auto matrix = Matrix<int>(0,0);
        REQUIRE(matrix.rows() == 0);
        REQUIRE(matrix.columns() == 0);
    }

    SECTION("Non-Empty")
    {
        auto matrix = Matrix<int>(2,3);
        SECTION("Dimensions")
        {
            REQUIRE(matrix.rows() == 2);
            REQUIRE(matrix.columns() == 3);
        }

        SECTION("Default")
        {
            REQUIRE(matrix(1, 1) == 0);
        }

        SECTION("Update")
        {
            matrix(1, 1) = 1;
            REQUIRE(matrix(1, 1) == 1);
        }
    }
}
