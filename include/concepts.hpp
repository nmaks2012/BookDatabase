#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <utility>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T t) {
    { t.emplace_back() } -> std::same_as<Book &>;
    { t.push_back(std::declval<Book>()) } -> std::same_as<void>;
    { t[std::declval<size_t>()] } -> std::same_as<Book &>;
    { t.back() } -> std::same_as<Book &>;
    { t.clear() } -> std::same_as<void>;
    { t.size() } -> std::same_as<size_t>;
    { t.empty() } -> std::same_as<bool>;
    typename T::value_type;
    requires std::same_as<typename T::value_type, Book>;
    requires std::random_access_iterator<typename T::iterator>;  // с расчетом на возможное использование deque
};

template <typename T>
concept BookIterator =
    std::random_access_iterator<T> &&
    std::is_convertible_v<std::iter_reference_t<T>, Book>;  // с расчетом на возможное использование deque

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = std::predicate<P, Book>;

template <typename C>
concept BookComparator = requires(C c) {
    { c(std::declval<Book>(), std::declval<Book>()) } -> std::convertible_to<bool>;
};

template <typename T>
concept BookRef = std::convertible_to<T, Book>;

}  // namespace bookdb