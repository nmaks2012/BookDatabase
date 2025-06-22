#include "book.hpp"
#include <algorithm>
#include <format>
#include <gtest/gtest.h>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace bookdb;
using namespace std::string_view_literals;
using namespace std::literals;

TEST(TestBook, ComparisonGenresAndStringView) {
    // Тестирование корректности маппинга из std::string_view в Genre и обратно
    EXPECT_TRUE(std::ranges::all_of(bookdb::map_genre_to_str, [&](const auto &item) {
        auto [genre, sv] = item;
        return genre == bookdb::ConvertGenre(sv) && sv == bookdb::ConvertGenre(genre);
    }));
}

TEST(TestBook, ConversionGenreThrow) {
    // Выбрасывание исключения при поиске не существующего жанра
    EXPECT_THROW(bookdb::ConvertGenre("NotExistingGenre"), std::logic_error);
    EXPECT_THROW(bookdb::ConvertGenre(bookdb::Genre{111}), std::logic_error);
}

TEST(TestBook, CreateGenreIsGenre) {
    // Создание Book жанр - Genre
    Book book{"George Orwell"sv, "Animal Farm"s, 1949, Genre::SciFi, 4.0, 190};
    EXPECT_EQ(book.genre, Genre::SciFi);
}

TEST(TestBook, CreateGenreIsStringView) {
    // Создание Book жанр - std::string_view
    Book book{"George Orwell"sv, "Animal Farm"s, 1949, "SciFi"sv, 4.0, 190};
    EXPECT_EQ(book.genre, Genre::SciFi);
}

TEST(TestBook, FormatingGenreToStr) {
    // Тестирование вывода в стрим из Genre
    std::string expected = std::format("{}", "Mystery");
    std::stringstream output;
    std::print(output, "{}", Genre::Mystery);

    std::string result = {std::istreambuf_iterator<std::string::value_type>(output),
                          std::istreambuf_iterator<std::string::value_type>()};

    EXPECT_EQ(result, expected);
}

TEST(TestBook, FormatingBookToStr) {
    // Тестирование вывода в стрим из Book
    Book book{"George Orwell"sv, "Animal Farm"s, 1949, "SciFi"sv, 4.1, 190};

    std::stringstream output;

    EXPECT_NO_THROW(std::print(output, "{}", book));
}