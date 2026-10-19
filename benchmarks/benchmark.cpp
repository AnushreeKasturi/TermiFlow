#include "config_service.hpp"
#include "command_parser.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

static std::string makeCommand(std::size_t tokenCount) {
    std::string command;
    for (std::size_t index = 0; index < tokenCount; ++index) {
        if (!command.empty()) {
            command += ' ';
        }
        command += "argument";
    }
    return command;
}

static std::uint64_t tokenizeAndCount(const std::string& command) {
    const std::vector<std::string> tokens = tokenizeCommand(command);
    std::uint64_t checksum = tokens.size();
    for (const std::string& token : tokens) {
        checksum += token.size();
    }
    return checksum;
}

template <typename Operation>
static void runBenchmark(const std::string& name, std::size_t iterations,
                         Operation operation) {
    std::uint64_t checksum = 0;
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t index = 0; index < iterations; ++index) {
        checksum += operation(index);
    }
    const auto elapsed = std::chrono::steady_clock::now() - start;
    const double seconds = std::chrono::duration<double>(elapsed).count();
    const double nanosecondsPerOperation = seconds * 1'000'000'000.0 / iterations;
    const double operationsPerSecond = iterations / seconds;

    std::cout << std::left << std::setw(28) << name
              << std::right << std::fixed << std::setprecision(2)
              << std::setw(12) << nanosecondsPerOperation << " ns/op  "
              << std::setw(12) << operationsPerSecond << " ops/s  "
              << "checksum=" << checksum << '\n';
}

static std::size_t parseIterations(int argc, char** argv) {
    if (argc == 1) {
        return 50000;
    }
    if (argc != 3 || std::string(argv[1]) != "--iterations") {
        throw std::invalid_argument("Usage: benchmark_runner [--iterations COUNT]");
    }

    std::size_t parsedLength = 0;
    const unsigned long long parsedValue = std::stoull(argv[2], &parsedLength);
    if (parsedLength != std::string(argv[2]).size() || parsedValue == 0) {
        throw std::invalid_argument("Iteration count must be a positive integer");
    }
    return static_cast<std::size_t>(parsedValue);
}

int main(int argc, char** argv) {
    try {
        const std::size_t iterations = parseIterations(argc, argv);

        std::cout << "TermiFlow microbenchmarks\n"
                  << "Compiler: " << __VERSION__ << '\n'
                  << "C++ standard: " << __cplusplus << '\n'
#ifdef _WIN32
                  << "Platform: Windows\n"
#elif __APPLE__
                  << "Platform: macOS\n"
#else
                  << "Platform: Linux/Unix\n"
#endif
                  << "Build: optimized (-O3 -DNDEBUG)\n"
                  << "Iterations per case: " << iterations << "\n\n";

        for (const std::size_t tokenCount : {2U, 16U, 128U}) {
            const std::string command = makeCommand(tokenCount);
            const std::string name = "tokenize (" + std::to_string(tokenCount) + " tokens)";
            runBenchmark(name, iterations, [command](std::size_t) {
                return tokenizeAndCount(command);
            });
        }

        ConfigService config;
        config.setValue("benchmark.key", "benchmark-value");
        runBenchmark("config lookup (hit)", iterations, [&config](std::size_t) {
            return config.getValue("benchmark.key").size();
        });
        runBenchmark("config lookup (miss)", iterations, [&config](std::size_t) {
            return config.getValue("missing.key", "fallback").size();
        });
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}