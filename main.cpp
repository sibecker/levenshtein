#include <iostream>
#include <deque>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

using namespace std::literals;

template<typename T>
class Matrix
{
public:
    Matrix(std::size_t rows, std::size_t columns) :
            rows_{rows}, columns_{columns},
            data_(rows * columns)
    {}

    std::size_t rows() const { return rows_; }
    std::size_t columns() const { return columns_; }

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


class Levenshtein {
public:
    using SrcPhoneme = std::string;
    using SrcString = std::vector<SrcPhoneme>;
    using DstPhoneme = std::string;
    using DstString = std::vector<DstPhoneme>;

    struct Start {};
    struct Match { SrcPhoneme from; DstPhoneme to; };
    struct Change { SrcPhoneme from; DstPhoneme to; };
    struct Insert { DstPhoneme value; };
    struct Delete { SrcPhoneme value; };

    class Operation {
    public:
        template<typename T>
        /*implicit*/ Operation(T &&op) :
                variant_{std::forward<T &&>(op)}
        {}

        friend std::ostream &operator<<(std::ostream &lhs, Operation const &rhs) {
            return lhs << std::visit(toString, rhs.variant_);
        }

    private:
        static constexpr struct {
            std::string operator()(Start const &) const {
                return "Start"s;
            }

            std::string operator()(Match const &op) const {
                return "Match\t"s + op.from + "\twith\t"s + op.to;
            }

            std::string operator()(Change const &op) const {
                return "Change\t"s + op.from + "\tinto\t"s + op.to;
            }

            std::string operator()(Insert const &op) const {
                return "Insert\t\t    \t"s +  op.value;
            }

            std::string operator()(Delete const &op) const {
                return "Delete\t"s + op.value;
            }
        } toString = {};

        std::variant<Start, Match, Change, Insert, Delete> variant_;
    };

    using Transforms = std::set<std::pair<SrcPhoneme, DstPhoneme>>;

    explicit Levenshtein(Transforms transforms) :
            transforms_{std::move(transforms)}
    {}

    struct Result
    {
        std::size_t score;
        std::deque<Operation> operations;
    };

    Result operator()(SrcString const& source, DstString const& destination) const
    {
        std::deque<Operation> operations;
        auto calculator = Calculator{transforms_, source, destination};
        Node const& result = calculator.node(source.size(), destination.size());
        for(Node const* node = &result; node != nullptr; node = node->previous)
            operations.push_front(node->operation);

        return {result.score, operations};
    }

private:
    struct Node
    {
        std::size_t score;
        Operation operation;
        Node const* previous;
    };

    class Calculator {
    private:
        Transforms const &transforms_;
        SrcString const &source_;
        DstString const &destination_;
        Matrix<std::optional<Node>> cache_;

    public:
        Calculator(Transforms const& transforms, SrcString const &source, DstString const& destination) :
            transforms_{transforms},
            source_{source},
            destination_{destination},
            cache_{source.size() + 1, destination.size() + 1}
        {
            cache_(0, 0) = Node{0, Start{}, nullptr};
            for (auto i = 1u; i < cache_.rows(); ++i)
                cache_(i, 0) = Node{i, Delete{source_[i - 1]}, &*cache_(i-1, 0)};
            for (auto j = 1u; j < cache_.columns(); ++j)
                cache_(0, j) = Node{j, Insert{destination_[j - 1]}, &*cache_(0, j-1)};
        }

        Node const& node(std::size_t i, std::size_t j) {
            if (!cache_(i, j)) {
                const auto ins_dst = node(i, j - 1).score + 1;
                const auto del_src = node(i - 1, j).score + 1;
                const auto &from = source_[i - 1];
                const auto &to = destination_[j - 1];
                const auto change_cost = transforms_.count({from, to}) ? 0 : 1;
                const auto change = node(i - 1, j - 1).score + change_cost;
                const auto score = std::min(std::min(del_src, ins_dst), change);
                if (change == score) {
                    if(change_cost) {
                        cache_(i, j) = Node{score, Change{from, to}, &node(i-1,j-1)};
                    } else {
                        cache_(i, j) = Node{score, Match{from, to}, &node(i-1,j-1)};
                    }
                } else if (score == ins_dst) {
                    cache_(i, j) = Node{score, Insert{to}, &node(i,j-1)};
                } else {
                    cache_(i, j) = Node{score, Delete{from}, &node(i-1, j)};
                }
            }

            return *cache_(i, j);
        }
    };

    Transforms transforms_;
};

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

    auto const [score, operations] = lev(source, destination);
    for(const auto& op : operations)
        std::cout << op << std::endl;
    std::cout << std::endl << score << " non-match operations." << std::endl;

    return 0;
}
