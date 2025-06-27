#include "book.hpp"
#include "book_database.hpp"
#include <algorithm>
#include <array>
#include <deque>
#include <gtest/gtest.h>
#include <initializer_list>
#include <iterator>
#include <sstream>
#include <sys/types.h>
#include <vector>

using namespace bookdb;
using namespace std::string_view_literals;
using namespace std::literals;

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

class TestEmptyBookDataBase : public ::testing::Test {
protected:
    TestContainer db{};
};

class TestBookDataBase : public ::testing::Test {
protected:
    TestContainer db{books_list};
};

class TestBookDataBaseIncorrect : public ::testing::Test {
protected:
    TestContainer db{books_list};
};

// ################ Тесты на пустой базе ###################
TEST_F(TestEmptyBookDataBase, Empty) {
    EXPECT_TRUE(db.empty());
    EXPECT_TRUE(db.empty());
}

TEST_F(TestEmptyBookDataBase, Clear) {
    db.Clear();
    db.Clear();
    EXPECT_TRUE(db.empty());
}

TEST_F(TestEmptyBookDataBase, Size) { EXPECT_EQ(db.size(), 0); }

TEST_F(TestEmptyBookDataBase, GetBooks) {
    EXPECT_TRUE(db.GetBooks().empty());
    EXPECT_EQ(db.GetBooks().size(), 0);
}

TEST_F(TestEmptyBookDataBase, GetAuthors) {
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.GetAuthors().size(), 0);
}
// ################ Тесты на пустой базе ###################

// ################ Тесты базы с входными данными ###################
TEST_F(TestBookDataBase, CreateFromInitializerList) {

    // Coздание из std::initializer_list
    TestContainer db_il{books_list};
    EXPECT_EQ(db_il.size(), 10);

    // Проверяем наличие всех книг в базе
    std::vector<Book> example{books_list};
    // clang-format off
    EXPECT_TRUE(
        std::ranges::all_of(
            example, 
            [&](const Book &book) { 
                return std::ranges::find(db_il, book) != db_il.end(); 
            }
        ));
    // clang-format on
}

TEST_F(TestBookDataBase, ClearEmptySize) {

    EXPECT_EQ(db.size(), 10);
    EXPECT_FALSE(db.empty());
    db.Clear();
    EXPECT_EQ(db.size(), 0);
    EXPECT_TRUE(db.empty());
}

TEST_F(TestBookDataBase, GetBooks) { EXPECT_EQ(db.GetBooks().size(), 10); }

TEST_F(TestBookDataBase, GetAuthors) { EXPECT_EQ(db.GetAuthors().size(), 9); }

TEST_F(TestBookDataBase, EmplaceBack) {
    Book book{"Author", "Titile", 1999, Genre::Biography, 0.1, 1};
    db.EmplaceBack("Author", "Titile", 1999, Genre::Biography, 0.1, 1);
    db.EmplaceBack("Author", "Titile", 1999, Genre::Biography, 0.1, 1);
    db.EmplaceBack("Author", "Titile", 1999, Genre::Biography, 0.1, 1);
    EXPECT_EQ(db.size(), 13);
    EXPECT_EQ(book, db.back());
}

TEST_F(TestBookDataBase, PushBack) {
    Book book{"Author", "Titile", 1999, Genre::Biography, 0.1, 1};
    db.PushBack(book);
    db.PushBack(book);
    db.PushBack(book);
    EXPECT_EQ(db.size(), 13);
    EXPECT_EQ(db.back(), book);
}

TEST_F(TestBookDataBase, Iterator) {
    EXPECT_EQ(std::distance<TestContainer::iterator>(db.begin(), db.end()), db.size());
}

TEST_F(TestBookDataBase, Const_iterator) {
    EXPECT_EQ(std::distance<TestContainer::const_iterator>(db.cbegin(), db.cend()), db.size());
}

TEST_F(TestBookDataBase, Reverse_iterator) {
    EXPECT_EQ(std::distance<TestContainer::reverse_iterator>(db.rbegin(), db.rend()), db.size());
}

TEST_F(TestBookDataBase, Const_reverse_iterator) {
    EXPECT_EQ(std::distance<TestContainer::const_reverse_iterator>(db.crbegin(), db.crend()), db.size());
}

TEST_F(TestBookDataBase, PrintBookDataBase) {
    std::stringstream output;
    EXPECT_NO_THROW(std::print(output, "{}", db));
}
// ################ Тесты базы с входными данными ###################

// ################ Некорректные входные данные ###################
TEST_F(TestBookDataBaseIncorrect, FindNotExistsBook) {
    // Поиск отсутствующей книги
    Book book{"Author", "Titile", 1999, Genre::Biography, 0.1, 1};
    EXPECT_TRUE(std::ranges::find(db, book) == db.end());
}
// ################ Некорректные входные данные ###################