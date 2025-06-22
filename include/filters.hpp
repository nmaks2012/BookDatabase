#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

auto GenreIs = [](const std::string_view genre) {
    return [genre](const Book &book) { return book.genre == ConvertGenre(genre); };
};

auto YearBetween = [](int from, int to) {
    return [from, to](const Book &book) { return book.year >= from && book.year < to; };
};

auto RatingAbove = [](double above) { return [above](const Book &book) { return book.rating > above; }; };

auto all_of = [](auto &&...filters) { return [&](const Book &book) { return (filters(book) && ...); }; };

auto any_of = [](auto &&...filters) { return [&](const Book &book) { return (filters(book) || ...); }; };

template <BookIterator It, BookPredicate Pred>
auto filterBooks(It begin, It end, Pred pred) {
    std::vector<std::reference_wrapper<const Book>> res;
    std::ranges::copy_if(begin, end, std::back_inserter(res), pred);
    return res;
};

}  // namespace bookdb