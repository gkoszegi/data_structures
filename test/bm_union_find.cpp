#include <benchmark/benchmark_api.h>
#include <union_find.hpp>
#include <random>

// =================================================================================================
void bm_union_find(benchmark::State& state)
{
    const unsigned nsets = state.range(0);
    union_find<unsigned> uf(nsets);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> dist(0, nsets-1);

    while (state.KeepRunning()) {
        for (size_t i = 0; i < 1000000; ++i)
            uf.join(dist(gen), dist(gen));
    }
}

// =================================================================================================
BENCHMARK(bm_union_find)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

BENCHMARK_MAIN()

