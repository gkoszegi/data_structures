#include <benchmark/benchmark_api.h>
#include <indexed_heap.hpp>
#include <random>

// =================================================================================================
void bm_indexed_heap(benchmark::State& state)
{
    const unsigned nelems = state.range(0);
    indexed_heap<unsigned, unsigned> q(nelems);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> dist(0, nelems-1);

    while (state.KeepRunning())
    {
        for (size_t i = 0; i < 1000000; ++i)
        {
            unsigned elem = dist(gen);
            unsigned prio = dist(gen);

            q.set_priority(elem, prio);
        }
    }
}

// =================================================================================================
BENCHMARK(bm_indexed_heap)->Arg(100)->Arg(1000)->Arg(10000)->Arg(100000)->Arg(1000000);

BENCHMARK_MAIN()
