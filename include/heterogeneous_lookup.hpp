#pragma once

#include "book.hpp"
#include <cstddef>
#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;
    bool operator()(std::string_view lhs, std::string_view rhs) { return lhs < rhs; };
    bool operator()(const std::string &lhs, std::string_view rhs) { return lhs < rhs; };
    bool operator()(std::string_view lhs, const std::string &rhs) { return lhs < rhs; };
    bool operator()(const std::string &lhs, const std::string &rhs) { return lhs < rhs; };
};

struct TransparentStringEqual {
    using is_transparent = void;
    bool operator()(std::string_view lhs, std::string_view rhs) { return lhs == rhs; };
    bool operator()(const std::string &lhs, std::string_view rhs) { return lhs == rhs; };
    bool operator()(std::string_view lhs, const std::string &rhs) { return lhs == rhs; };
    bool operator()(const std::string &lhs, const std::string &rhs) { return lhs == rhs; };
};

struct TransparentStringHash {
    using is_transparent = void;
    size_t operator()(std::string_view sv) const { return std::hash<std::string_view>()(sv); };
    size_t operator()(const std::string &str) const { return std::hash<std::string>()(str); };
};

struct TransparentRatingSum {
    using is_transparent = void;
    double operator()(const Book &lhs, const Book &rhs) { return lhs.rating + rhs.rating; };
    double operator()(const Book &lhs, double rhs) { return lhs.rating + rhs; };
    double operator()(double lhs, const Book &rhs) { return lhs + rhs.rating; };
    double operator()(double lhs, double rhs) { return lhs + rhs; };
};

}  // namespace bookdb
