#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "filters.hpp"
#include "statsistics.hpp"
#include <algorithm>
#include <deque>
#include <gtest/gtest.h>
#include <string_view>
#include <vector>

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

class TestStatistics : public ::testing::Test {
protected:
    TestContainer db{BOOKS_LIST};
};

// ############################# Тесты на заполненной базе #################################
TEST_F(TestStatistics, buildAuthorHistogramFlat) {
    auto histogram = buildAuthorHistogramFlat(db);
    EXPECT_TRUE(std::ranges::all_of(histogram, [&](const auto &item) {
        if (item.first == "George Orwell"s) {
            return item.second == 2;
        } else {
            return item.second == 1;
        }
    }));
}

TEST_F(TestStatistics, calculateGenreRatings) {
    GenreStatsContainer genreRatings = calculateGenreRatings(db.begin(), db.end());
    EXPECT_DOUBLE_EQ(genreRatings[Genre::Fiction], 4.55);
    EXPECT_DOUBLE_EQ(genreRatings[Genre::SciFi], 4.25);
}

TEST_F(TestStatistics, calculateAverageRating) { EXPECT_DOUBLE_EQ(calculateAverageRating(db.begin(), db.end()), 4.49); }

TEST_F(TestStatistics, filterBooks) {
    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    EXPECT_TRUE(std::ranges::all_of(
        filtered, [&](const Book &book) { return book.rating > 4.5 && book.year > 1900 && book.year < 1999; }));
}

TEST_F(TestStatistics, getTopNBy) {

    std::vector<Book> top_3_book{{"J.R.R. Tolkien"sv, "The Hobbit"s, 1937, Genre::Fiction, 4.9, 203},
                                 {"Harper Lee"sv, "To Kill a Mockingbird"s, 1960, Genre::Fiction, 4.8, 156},
                                 {"Jane Austen"sv, "Pride and Prejudice"s, 1813, Genre::Fiction, 4.7, 178}};

    auto topBooks = getTopNBy(db.begin(), db.end(), 3, comp::LessByRating{});

    EXPECT_TRUE(std::ranges::all_of(
        top_3_book, [&](const Book &book) { return std::ranges::find(topBooks, book) != topBooks.end(); }));
}
// ############################# Тесты на заполненной базе #################################

// ############################# Тесты на пустой базе #################################
class TestEmptyStatistics : public ::testing::Test {
protected:
    TestContainer db{};
};

TEST_F(TestEmptyStatistics, buildAuthorHistogramFlat) {
    auto histogram = buildAuthorHistogramFlat(db);
    EXPECT_TRUE(histogram.empty());
}

TEST_F(TestEmptyStatistics, calculateGenreRatings) {
    GenreStatsContainer genreRatings = calculateGenreRatings(db.begin(), db.end());
    EXPECT_TRUE(genreRatings.empty());
}

TEST_F(TestEmptyStatistics, calculateAverageRating) {
    EXPECT_DOUBLE_EQ(calculateAverageRating(db.begin(), db.end()), 0.0);
}

TEST_F(TestEmptyStatistics, filterBooks) {
    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    EXPECT_TRUE(filtered.empty());
}

TEST_F(TestEmptyStatistics, getTopNBy) {

    auto topBooks = getTopNBy(db.begin(), db.end(), 3, comp::LessByRating{});
    EXPECT_TRUE(topBooks.empty());
}
// ############################# Тесты на пустой базе #################################

// ############################# Не верные данные #################################
class TestNotExistsData : public ::testing::Test {
protected:
    TestContainer db{
        {""sv, ""s, -10, Genre::Unknown, -999.0, 0},
        {""sv, ""s, -1, Genre::Unknown, -10000000000.0, -0},
    };
};

TEST_F(TestNotExistsData, buildAuthorHistogramFlat) {
    auto histogram = buildAuthorHistogramFlat(db);
    EXPECT_TRUE(histogram.size() == 1);
}

TEST_F(TestNotExistsData, calculateGenreRatings) {
    GenreStatsContainer genreRatings = calculateGenreRatings(db.begin(), db.end());
    EXPECT_TRUE(genreRatings.size() == 1);
}

TEST_F(TestNotExistsData, calculateAverageRating) {
    EXPECT_DOUBLE_EQ(calculateAverageRating(db.begin(), db.end()), -5000000499.5);
}

TEST_F(TestNotExistsData, filterBooks) {
    auto filtered = filterBooks(db.begin(), db.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5)));
    EXPECT_TRUE(filtered.empty());
}

TEST_F(TestNotExistsData, getTopNBy) {
    auto topBooks = getTopNBy(db.begin(), db.end(), 3, comp::LessByRating{});
    EXPECT_TRUE(topBooks.size() == 2);
}
// ############################# Не верные данные #################################