#include "book_database.hpp"
#include "comparators.hpp"
#include <algorithm>
#include <deque>
#include <gtest/gtest.h>
#include <string_view>

#define BOOKS_LIST                                                                                                     \
    {"George Orwell"sv, "1984"s, 1949, Genre::SciFi, 4., 190},                                                         \
        {"George Orwell"sv, "Animal Farm"s, 1945, Genre::Fiction, 4.4, 143},                                           \
        {"F. Scott Fitzgerald"sv, "The Great Gatsby"s, 1925, Genre::Fiction, 4.5, 120},                                \
        {"Harper Lee"sv, "To Kill a Mockingbird"s, 1960, Genre::Fiction, 4.8, 156},                                    \
        {"Jane Austen"sv, "Pride and Prejudice"s, 1813, Genre::Fiction, 4.7, 178},                                     \
        {"J.D. Salinger"sv, "The Catcher in the Rye"s, 1951, Genre::Fiction, 4.3, 112},                                \
        {"Aldous Huxley"sv, "Brave New World"s, 1932, Genre::SciFi, 4.5, 98},                                          \
        {"Charlotte Brontë"sv, "Jane Eyre"s, 1847, Genre::Fiction, 4.6, 110},                                          \
        {"J.R.R. Tolkien"sv, "The Hobbit"s, 1937, Genre::Fiction, 4.9, 203},                                           \
        {"William Golding"sv, "Lord of the Flies"s, 1954, Genre::Fiction, 4.2, 89}

#define BOOK "Author"sv, "Titile"s, 1999, Genre::Biography, 0.1, 1

using namespace bookdb;
using namespace std::string_view_literals;
using namespace std::literals;
using namespace comp;

// using TestContainer = bookdb::BookDatabase<>;
using TestContainer = bookdb::BookDatabase<std::deque<Book>>;

// ############################# Тесты на заполненной базе #################################
class TestComparators : public ::testing::Test {
protected:
    TestContainer db{BOOKS_LIST};
};

TEST_F(TestComparators, LessByAuthor) {
    // Сотрировка по автору
    std::sort(db.begin(), db.end(), LessByAuthor{});
    // Пробегаемся по результату сортировки и проверяем
    EXPECT_EQ(std::ranges::adjacent_find(db,
                                         [](const Book &lhs, const Book &rhs) {
                                             return std::ranges::lexicographical_compare(rhs.author, lhs.author);
                                         }),
              db.end());
}

TEST_F(TestComparators, LessByPopularity) {
    // Сотритовка по популярности
    std::sort(db.begin(), db.end(), LessByPopularity{});
    // Проверка результата
    EXPECT_EQ(std::ranges::adjacent_find(
                  db, [](const Book &lhs, const Book &rhs) { return lhs.read_count <= rhs.read_count; }),
              db.end());
}
// ############################# Тесты на заполненной базе #################################

// ############################# Тесты на пустой базе #################################
class TestComparatorsEmptyDb : public ::testing::Test {
protected:
    TestContainer db{};
};

TEST_F(TestComparatorsEmptyDb, LessByAuthor) {
    // Сотрировка по автору
    std::sort(db.begin(), db.end(), LessByAuthor{});
    // Пробегаемся по результату сортировки и проверяем
    EXPECT_EQ(std::ranges::adjacent_find(db,
                                         [](const Book &lhs, const Book &rhs) {
                                             return std::ranges::lexicographical_compare(rhs.author, lhs.author);
                                         }),
              db.end());
}

TEST_F(TestComparatorsEmptyDb, LessByPopularity) {
    // Сотритовка по популярности
    std::sort(db.begin(), db.end(), LessByPopularity{});
    // Проверка результата
    EXPECT_EQ(std::ranges::adjacent_find(
                  db, [](const Book &lhs, const Book &rhs) { return lhs.read_count <= rhs.read_count; }),
              db.end());
}
// ############################# Тесты на пустой базе #################################