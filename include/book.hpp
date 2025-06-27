#pragma once

#include <array>
#include <format>
#include <stdexcept>
#include <string>
#include <string_view>

#include <boost/container/flat_map.hpp>
#include <unordered_map>
#include <utility>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

constexpr Genre ConvertGenre(const std::string_view s) {

    if (s == "Fiction") {
        return Genre::Fiction;
    } else if (s == "Mystery") {
        return Genre::Mystery;
    } else if (s == "NonFiction") {
        return Genre::NonFiction;
    } else if (s == "SciFi") {
        return Genre::SciFi;
    } else if (s == "Biography") {
        return Genre::Biography;
    } else if (s == "Unknown") {
        return Genre::Unknown;
    } else {
        throw std::logic_error("Unsupported conversion from std::string_view to bookdb::Genre");
    }
};

constexpr const std::string_view ConvertGenre(const Genre g) {

    if (g == Genre::Fiction) {
        return "Fiction";
    } else if (g == Genre::Mystery) {
        return "Mystery";
    } else if (g == Genre::NonFiction) {
        return "NonFiction";
    } else if (g == Genre::SciFi) {
        return "SciFi";
    } else if (g == Genre::Biography) {
        return "Biography";
    } else if (g == Genre::Unknown) {
        return "Unknown";
    } else {
        throw std::logic_error{"Unsupported conversion from bookdb::Genre to std::string_view"};
    }
};

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;

    constexpr Book(std::string_view author, std::string &&title, int year, std::string_view genre, double rating,
                   int read_count)
        : author(author), title(std::move(title)), year(year), genre(ConvertGenre(genre)), rating(rating),
          read_count(read_count) {};

    constexpr Book(std::string_view author, std::string title, int year, Genre genre, double rating, int read_count)
        : author(author), title(std::move(title)), year(year), genre(genre), rating(rating), read_count(read_count) {};

    bool operator<=>(const Book &other) const = default;
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        return format_to(fc.out(), "{}", bookdb::ConvertGenre(g));
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

template <>
struct formatter<bookdb::Book> {
    template <typename FormatContext>
    auto format(const bookdb::Book book, FormatContext &fc) const {

        return format_to(fc.out(), "\033[4m|{:^25}|{:^25}|{:^15}|{:^15}|{:^15}|{:^15}|\033[0m", book.title, book.author,
                         book.year, bookdb::ConvertGenre(book.genre), book.rating, book.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
