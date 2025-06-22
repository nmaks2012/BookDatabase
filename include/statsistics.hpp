#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <string_view>

#include "book.hpp"
#include "book_database.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

#include <print>

namespace bookdb {

using HistogramContainer = boost::container::flat_map<std::string_view, size_t>;
using GenreStatsContainer = boost::container::flat_map<Genre, double>;

template <BookContainerLike T>
HistogramContainer buildAuthorHistogramFlat(BookDatabase<T> &cont) {

    HistogramContainer histogram;

    std::ranges::for_each(cont, [&](const Book &book) { histogram[book.author]++; });

    return histogram;
}

struct rating_sum_item {
    double sum_ratings = 0.0;
    size_t count_book = 0;
    double Avg() { return sum_ratings / count_book; }
};

template <BookIterator It>
GenreStatsContainer calculateGenreRatings(It begin, It end) {

    boost::container::flat_map<Genre, rating_sum_item> sum_ratings;

    // Заполняем значения по сумме и количеству всех рейтингов
    std::ranges::for_each(begin, end, [&](const Book &book) {
        sum_ratings[book.genre].sum_ratings += book.rating;
        sum_ratings[book.genre].count_book += 1;
    });

    GenreStatsContainer ratings_avg;
    ratings_avg.reserve(sum_ratings.size());

    // Заполняем итоговый средний рейтинг по всем жанрам
    std::ranges::for_each(sum_ratings, [&](auto &item) {
        auto [genre, stat] = item;
        ratings_avg[genre] = stat.Avg();
    });

    return ratings_avg;
}

template <BookIterator It>
double calculateAverageRating(It begin, It end) {
    size_t size = std::distance(begin, end);
    if (size == 0) {
        return 0.0;
    }
    return std::reduce(begin, end, 0.0, TransparentRatingSum{}) / size;
}

template <BookIterator It, BookComparator Comp>
auto getTopNBy(It begin, It end, size_t count, const Comp comp) {

    count = std::min(count, static_cast<size_t>(std::distance(begin, end)));

    auto middle = std::next(begin, count);
    std::partial_sort(begin, middle, end, comp);

    return std::vector<std::reference_wrapper<const Book>>(begin, middle);
}

template <BookIterator It>
auto sampleRandomBooks(It begin, It end, size_t count) {

    count = std::min(count, static_cast<size_t>(std::distance(begin, end)));
    std::vector<std::reference_wrapper<const Book>> dest;

    std::ranges::sample(begin, end, std::back_inserter(dest), count, std::mt19937{std::random_device{}()});

    return dest;
}

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::HistogramContainer> {
    template <typename FormatContext>
    auto format(const bookdb::HistogramContainer &histogram, FormatContext &fc) const {

        format_to(fc.out(), "\033[4m|{:^20}|{:^20}|\033[0m\n", "AUTHOR", "BOOKS");

        std::ranges::for_each(histogram, [&](const auto &item) {
            format_to(fc.out(), "\033[4m|{:^20}|{:^20}|\033[0m\n", item.first, item.second);
        });

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::GenreStatsContainer> {
    template <typename FormatContext>
    auto format(const bookdb::GenreStatsContainer &ratings_avg, FormatContext &fc) const {

        format_to(fc.out(), "\033[4m|{:^20}|{:^20}|\033[0m\n", "GENRE", "AVG RATING");

        std::ranges::for_each(ratings_avg, [&](const auto &item) {
            format_to(fc.out(), "\033[4m|{:^20}|{:^20.2f}|\033[0m\n", bookdb::ConvertGenre(item.first), item.second);
        });

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
