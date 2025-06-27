#include <algorithm>
#include <benchmark/benchmark.h>
#include <boost/container/flat_map.hpp>
#include <cstddef>
#include <cstdlib>
#include <deque>
#include <random>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "concepts.hpp"
#include "filters.hpp"
#include "statsistics.hpp"

using benchmark::DoNotOptimize;
using namespace bookdb;
using namespace boost::container;

struct Book_data {
    std::string author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;
};

std::unordered_map<size_t, std::vector<Book_data>> cachedData;

const std::span<Book_data> generateData(size_t N) {

    auto it = cachedData.find(N);
    if (it != cachedData.end()) {
        return it->second;
    }

    std::vector<Book_data> data;
    data.reserve(N);

    for (size_t i = 0; i < N; ++i) {

        std::string author{"Author" + std::to_string(i)};
        std::string title{"Title" + std::to_string(i)};
        int year = 1900 + rand() % 69 + 20;
        Genre genre = Genre::Unknown;  //(map_str_to_genre.begin() + rand() % map_str_to_genre.size())->second;
        double rating = rand() % 10;
        int read_count = rand() % 1000;

        data.emplace_back(author, title, year, genre, rating, read_count);
    }

    // Перемешиваем вектор, чтобы порядок вставок был случайным
    std::shuffle(data.begin(), data.end(), std::mt19937{42});

    auto newIt = cachedData.try_emplace(it, N, std::move(data));
    return newIt->second;
}

template <BookContainerLike Cont>
static void BM_PushBack(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    for (auto _ : state) {
        {
            BookDatabase<Cont> cont;
            for (auto v : data) {
                cont.PushBack(Book{v.author, v.title, v.year, v.genre, v.rating, v.read_count});
            }

            state.PauseTiming();
        }
        state.ResumeTiming();
    }
}

template <BookContainerLike Cont>
static void BM_EmplaceBack(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    for (auto _ : state) {
        {
            BookDatabase<Cont> cont;
            for (auto v : data) {
                cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
            }

            state.PauseTiming();
        }
        state.ResumeTiming();
    }
}

template <BookContainerLike Cont>
static void BM_SortLessByAuthor(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            std::sort(cont.begin(), cont.end(), comp::LessByAuthor{});
            state.PauseTiming();
        }
        state.ResumeTiming();
        std::shuffle(cont.begin(), cont.end(), std::mt19937{42});
    }
}

template <BookContainerLike Cont>
static void BM_SortLessByPopularity(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            std::sort(cont.begin(), cont.end(), comp::LessByPopularity{});
            state.PauseTiming();
        }
        state.ResumeTiming();
        std::shuffle(cont.begin(), cont.end(), std::mt19937{42});
    }
}

template <BookContainerLike Cont>
static void BM_SortLessByRating(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            std::sort(cont.begin(), cont.end(), comp::LessByRating{});
            state.PauseTiming();
        }
        state.ResumeTiming();
        std::shuffle(cont.begin(), cont.end(), std::mt19937{42});
    }
}

template <BookContainerLike Cont>
static void BM_BuildAuthorHistogramFlat(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            DoNotOptimize(buildAuthorHistogramFlat(cont));
            state.PauseTiming();
        }
        state.ResumeTiming();
    }
}

template <BookContainerLike Cont>
static void BM_CalculateGenreRatings(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            DoNotOptimize(calculateGenreRatings(cont.begin(), cont.end()));
            state.PauseTiming();
        }
        state.ResumeTiming();
    }
}

template <BookContainerLike Cont>
static void BM_CalculateAverageRating(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            DoNotOptimize(calculateAverageRating(cont.begin(), cont.end()));
            state.PauseTiming();
        }
        state.ResumeTiming();
    }
}

template <BookContainerLike Cont>
static void BM_FilterBooksAllOf(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            DoNotOptimize(filterBooks(cont.begin(), cont.end(), all_of(YearBetween(1900, 1999), RatingAbove(4.5))));
            state.PauseTiming();
        }
        state.ResumeTiming();
    }
}

template <BookContainerLike Cont>
static void BM_FilterBooksAnyOf(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            DoNotOptimize(filterBooks(cont.begin(), cont.end(),
                                      any_of(GenreIs("SciFi"), YearBetween(1900, 1999), RatingAbove(4.5))));
            state.PauseTiming();
        }
        state.ResumeTiming();
    }
}

template <BookContainerLike Cont>
static void BM_GetTopNBy(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            DoNotOptimize(getTopNBy(cont.begin(), cont.end(), 10, comp::LessByRating{}));
            state.PauseTiming();
        }
        state.ResumeTiming();
    }
}

template <BookContainerLike Cont>
static void BM_SampleRandomBooks(benchmark::State &state) {
    int count = state.range(0);
    auto data = generateData(count);

    BookDatabase<Cont> cont;
    for (auto v : data) {
        cont.EmplaceBack(v.author, v.title, v.year, v.genre, v.rating, v.read_count);
    }

    for (auto _ : state) {
        {
            DoNotOptimize(sampleRandomBooks(cont.begin(), cont.end(), 10));
            state.PauseTiming();
        }
        state.ResumeTiming();
    }
}

const size_t ITERATIONS = 10;
const size_t RANGE_FROM = 1000;
const size_t RANGE_TO = 100000;

// ################### Тестирование с Vector ##################################
using Vector = std::vector<Book>;

BENCHMARK(BM_PushBack<Vector>)->Range(RANGE_FROM, RANGE_TO)->Iterations(ITERATIONS)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_EmplaceBack<Vector>)->Range(RANGE_FROM, RANGE_TO)->Iterations(ITERATIONS)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SortLessByAuthor<Vector>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SortLessByPopularity<Vector>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SortLessByRating<Vector>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_BuildAuthorHistogramFlat<Vector>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_CalculateGenreRatings<Vector>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_CalculateAverageRating<Vector>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_FilterBooksAllOf<Vector>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_FilterBooksAnyOf<Vector>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_GetTopNBy<Vector>)->Range(RANGE_FROM, RANGE_TO)->Iterations(ITERATIONS)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SampleRandomBooks<Vector>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
// ################### Тестирование с Vector ##################################

// ################### Тестирование с Deque ##################################
using Deque = std::deque<Book>;

BENCHMARK(BM_PushBack<Deque>)->Range(RANGE_FROM, RANGE_TO)->Iterations(ITERATIONS)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_EmplaceBack<Deque>)->Range(RANGE_FROM, RANGE_TO)->Iterations(ITERATIONS)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SortLessByAuthor<Deque>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SortLessByPopularity<Deque>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SortLessByRating<Deque>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_BuildAuthorHistogramFlat<Deque>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_CalculateGenreRatings<Deque>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_CalculateAverageRating<Deque>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_FilterBooksAllOf<Deque>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_FilterBooksAnyOf<Deque>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_GetTopNBy<Deque>)->Range(RANGE_FROM, RANGE_TO)->Iterations(ITERATIONS)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SampleRandomBooks<Deque>)
    ->Range(RANGE_FROM, RANGE_TO)
    ->Iterations(ITERATIONS)
    ->Unit(benchmark::kMicrosecond);
// ################### Тестирование с Deque ##################################

BENCHMARK_MAIN();