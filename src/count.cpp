#include "levenshtein/count.h"

namespace levenshtein
{
    std::size_t Count::score() const {
        return changes + insertions + deletions;
    }

    std::ostream& operator<<(std::ostream& lhs, Count const& rhs)
    {
        return lhs <<
                   "Matches:    " << rhs.matches << std::endl <<
                   "Changes:    " << rhs.changes << std::endl <<
                   "Insertions: " << rhs.insertions << std::endl <<
                   "Deletions:  " << rhs.deletions << std::endl <<
                   "(" << rhs.score() << " non-match operations)";
    }
}
