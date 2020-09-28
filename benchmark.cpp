// Copyright 2020 Matt Borland
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <benchmark/benchmark.h>
#include "include/cache_size.hpp"

void cache_size(benchmark::State& state)
{
    for(auto _ : state)
    {
        benchmark::DoNotOptimize(L1D_cache_size());
    }
    state.SetComplexityN(state.range(0));
}

BENCHMARK(cache_size)->RangeMultiplier(2)->Range(128, 1<<20)->Complexity()->Unit(benchmark::kNanosecond);
BENCHMARK_MAIN();
