#pragma once

#include <deque>
#include <iostream>
#include <unordered_set>
#include "hash_append.h"
#include "levenshtein/result.h"

namespace levenshtein {
    // A Levenstein problem to hold the transforms
    // Set of pairs of matchings from the source alphabet to the destination alphabet
    using Transforms = std::unordered_set<std::pair<SrcChar, DstChar>, xstd::uhash<>>;

    class Levenshtein {
    public:
        explicit Levenshtein(Transforms transforms);

        // Calculate the operations used in converting from source to destination
        Result operator()(SrcString const &source, DstString const &destination) const;

    private:
        Transforms transforms_;
    };
}
