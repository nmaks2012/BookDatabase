#include "book_database.hpp"
#include "comparators.hpp"
#include <algorithm>
#include <deque>
#include <gtest/gtest.h>
#include <string_view>

using namespace bookdb;
using namespace std::string_view_literals;
using namespace std::literals;
using namespace comp;

const std::initializer_list<Book> books_list{
    {"George Orwell", "1984", 1949, Genre::SciFi, 4., 190},
    {"George Orwell", "Animal Farm", 1945, Genre::Fiction, 4.4, 143},
    {"F. Scott Fitzgerald", "The Great Gatsby", 1925, Genre::Fiction, 4.5, 120},
    {"Harper Lee", "To Kill a Mockingbird", 1960, Genre::Fiction, 4.8, 156},
    {"Jane Austen", "Pride and Prejudice", 1813, Genre::Fiction, 4.7, 178},
    {"J.D. Salinger", "The Catcher in the Rye", 1951, Genre::Fiction, 4.3, 112},
    {"Aldous Huxley", "Brave New World", 1932, Genre::SciFi, 4.5, 98},
    {"Charlotte Brontë", "Jane Eyre", 1847, Genre::Fiction, 4.6, 110},
    {"J.R.R. Tolkien", "The Hobbit", 1937, Genre::Fiction, 4.9, 203},
    {"William Golding", "Lord of the Flies", 1954, Genre::Fiction, 4.2, 89}};

// using TestContainer = bookdb::BookDatabase<>;
using TestContainer = bookdb::BookDatabase<std::deque<Book>>;

// ############################# Тесты на заполненной базе #################################
class TestComparators : public ::testing::Test {
protected:
    TestContainer db{books_list};
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