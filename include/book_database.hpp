#pragma once

#include <print>
#include <string>
#include <unordered_set>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases

    // Type definitions for easier access to types used in the container
    // Type of elements stored in the container
    using value_type = typename BookContainer::value_type;

    // Allocator type used for memory management
    using allocator_type = typename BookContainer::allocator_type;

    // Reference type to the container's elements
    using reference = typename BookContainer::reference;

    // Constant reference type to the container's elements
    using const_reference = typename BookContainer::const_reference;

    // Type for size and capacity of the container
    using size_type = typename BookContainer::size_type;

    // Type for difference between iterator positions
    using difference_type = typename BookContainer::difference_type;

    using pointer = typename BookContainer::pointer;
    using const_pointer = typename BookContainer::const_pointer;

    // Standart container iterators
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using reverse_iterator = typename BookContainer::reverse_iterator;
    using const_reverse_iterator = typename BookContainer::const_reverse_iterator;

    using AuthorContainer = std::unordered_set<std::string>;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> list) : books_(list) {
        std::for_each(books_.begin(), books_.end(), [&](reference book) { AddAuthor(book); });
    };

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    const_reference back() { return books_.back(); }

    // Standard container interface methods
    template <typename... Args>
    const_reference EmplaceBack(Args &&...args) {
        books_.emplace_back(std::forward<Args>(args)...);
        AddAuthor(books_.back());
        return books_.back();
    }

    template <typename BookRef>
    void PushBack(BookRef &&book) {
        books_.push_back(std::forward<BookRef>(book));
        AddAuthor(books_.back());
    }

    reference operator[](size_type idx) { return books_[idx]; }

    const BookContainer &GetBooks() const { return books_; }

    const AuthorContainer &GetAuthors() const { return authors_; }

    size_type size() const { return books_.size(); }

    bool empty() const { return books_.empty(); }

    iterator begin() { return books_.begin(); }

    iterator end() { return books_.end(); }

    const_iterator cbegin() const { return books_.cbegin(); }

    const_iterator cend() const { return books_.cend(); }

    reverse_iterator rbegin() { return books_.rbegin(); }

    reverse_iterator rend() { return books_.rend(); }

    const_reverse_iterator crbegin() const { return books_.crbegin(); }

    const_reverse_iterator crend() const { return books_.crend(); }

private:
    void AddAuthor(reference book) { book.author = *authors_.emplace(book.author).first; }
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <bookdb::BookContainerLike T>
struct formatter<bookdb::BookDatabase<T>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<T> &db, FormatContext &fc) const {

        format_to(fc.out(), "BookDatabase (size = {}): \n", db.size());

        format_to(fc.out(), "\033[4m|{:^25}|{:^25}|{:^15}|{:^15}|{:^15}|{:^15}|\033[0m\n", "TITLE", "AUTHOR", "YEAR",
                  "GENRE", "RATING", "READS");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "{}\n", book);
        }

        format_to(fc.out(), "\n\033[4m|{:^25}|\033[0m\n", "AUTHORS:");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "\033[4m|{:^25}|\033[0m\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
