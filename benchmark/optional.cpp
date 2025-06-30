#include <benchmark/benchmark.h>
#include "optional.hpp"
#include "vector.hpp"
#include "algorithm.hpp"

static void StdOptionalBaseline(benchmark::State& state) {
    for (auto _ : state) {
        int i = 0;
        std::vector<int> vec(1000);
        for (auto& _ : vec) {
            i++;
        }
    }
}
BENCHMARK(StdOptionalBaseline);

static void StdOptionalCreation(benchmark::State& state) {
    for (auto _ : state) {
        int i = 0;
        std::vector<std::optional<int>> vec(1000);
        for (auto& v : vec) {
            *v;
            i++;
        }
    }
}
BENCHMARK(StdOptionalCreation);

static void SafeOptionalCreation(benchmark::State& state) {
    for (auto _ : state) {
        int i = 0;
        safe::vector<int> vec(1000);
        safe::for_each(vec, [&i](int&) {
            i++;
        });
    }
}
BENCHMARK(SafeOptionalCreation);

BENCHMARK_MAIN();