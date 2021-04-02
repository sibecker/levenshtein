#include "levenshtein.h"

int main() {
    Levenshtein::Transforms const transforms = {
        {"alpha", "a"},
        {"beta", "b"},
        {"gamma", "g"},
        {"delta", "d"},
        {"epsilon", "e"},
        {"zeta", "z"},
        {"eta", "e"},
        {"theta", "th"},
        {"iota", "i"},
        {"kappa", "c"}, {"kappa", "k"},
        {"lambda", "l"},
        {"mu", "m"},
        {"nu", "n"},
        {"xi", "x"},
        {"omicron", "o"},
        {"pi", "p"},
        {"rho", "r"}, {"rho", "rh"},
        {"sigma", "s"},
        {"tau", "t"},
        {"upsilon", "u"}, {"upsilon", "y"},
        {"phi", "f"}, {"phi", "ph"},
        {"chi", "ch"}, {"chi", "kh"},
        {"psi", "ps"},
        {"omega", "o"}
    };

    auto const lev = Levenshtein(transforms);
    Levenshtein::SrcString const source = {"alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta", "iota", "kappa", "lambda", "mu", "nu", "xi", "omicron", "pi", "rho", "sigma", "tau", "upsilon", "phi", "chi", "psi", "omega"};
    Levenshtein::DstString const destination = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};

    auto const result = lev(source, destination);
    for(const auto& op : result.operations())
        std::cout << op << std::endl;

    std::cout << std::endl << result.count() << std::endl;

    return 0;
}
