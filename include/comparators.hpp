#pragma once

#include "book.hpp"
#include <algorithm>
#include <string_view>

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const Book &lhs, const Book &rhs) const {
        return std::ranges::lexicographical_compare(lhs.author, rhs.author);
    }
};

struct LessByPopularity {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.read_count > rhs.read_count; }
};

struct LessByRating {
    bool operator()(const Book &lhs, const Book &rhs) const { return lhs.rating > rhs.rating; }
};

}  // namespace bookdb::comp