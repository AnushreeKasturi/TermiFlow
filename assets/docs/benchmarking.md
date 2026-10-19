# Benchmarking Guide

TermiFlow includes a lightweight microbenchmark runner for command tokenization and configuration lookups. It is separate from the unit test suite and uses only the C++ standard library.

## Run benchmarks

```bash
make benchmark
```

The default run performs 50,000 iterations per case. To choose a different count:

```bash
make benchmark BENCHMARK_ARGS="--iterations 100000"
```

The runner prints compiler, C++ standard, platform, build mode, nanoseconds per operation, operations per second, and a checksum for each case. Benchmark builds use `-O3 -DNDEBUG`; unit tests retain their normal build flags.

Cases cover tokenization of 2, 16, and 128 tokens, plus configuration map lookups for both present and missing keys. Run from the repository root, preferably on an otherwise idle machine, and compare results only when compiler, build flags, platform, and workload match. These are microbenchmarks, not end-to-end application startup measurements.