#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

#include <boost/container/flat_map.hpp>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

static const boost::container::flat_map<std::string_view, Genre> map_str_to_genre{
    {"Fiction", Genre::Fiction}, {"Mystery", Genre::Mystery},     {"NonFiction", Genre::NonFiction},
    {"SciFi", Genre::SciFi},     {"Biography", Genre::Biography}, {"Unknown", Genre::Unknown}};

static const boost::container::flat_map<Genre, std::string_view> map_genre_to_str{
    {Genre::Fiction, "Fiction"}, {Genre::Mystery, "Mystery"},     {Genre::NonFiction, "NonFiction"},
    {Genre::SciFi, "SciFi"},     {Genre::Biography, "Biography"}, {Genre::Unknown, "Unknown"}};

constexpr Genre ConvertGenre(const std::string_view s) {

    if (map_str_to_genre.contains(s)) {
        return map_str_to_genre.at(s);
    }
    throw std::logic_error{"Unsupported conversion from std::string_view to bookdb::Genre"};
};

constexpr const std::string_view ConvertGenre(const Genre g) {

    if (map_genre_to_str.contains(g)) {
        return map_genre_to_str.at(g);
    }
    throw std::logic_error{"Unsupported conversion from bookdb::Genre to std::string_view"};
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
        : author(author), title(title), year(year), genre(ConvertGenre(genre)), rating(rating),
          read_count(read_count) {};

    constexpr Book(std::string_view author, std::string &&title, int year, Genre genre, double rating, int read_count)
        : author(author), title(title), year(year), genre(genre), rating(rating), read_count(read_count) {};

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
