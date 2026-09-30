#include "interface/student.h"
#include "interface/interface_minhanh/LinearGpaFilter.h"
#include "interface/interface_minhanh/SortedGpaFilter.h"
#include "interface/interface_mytra/LinearFilter.h"
#include "interface/interface_mytra/OptimizedLinearFilter.h"
#include "interface/interface_tra/CustomMaxHeapGpaFinder.h"
#include "interface/interface_tra/LinearMaxScanGpaFinder.h"
#include "interface/interface_trang/HashTable.h"
#include "interface/interface_trang/LinearSearch.h"
#include "nlohmann/json.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using json = nlohmann::json;
using Clock = std::chrono::steady_clock;

namespace {

volatile std::size_t sinkSize = 0;
volatile double sinkGpa = 0.0;

struct Stats {
    double minMs{};
    double medianMs{};
    double meanMs{};
    double maxMs{};
};

struct TimedResult {
    double ms{};
    std::size_t resultCount{};
    long long comparisons{};
};

template <typename Func>
double measureMs(Func &&func) {
    auto start = Clock::now();
    func();
    auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

Stats summarize(std::vector<double> values) {
    std::sort(values.begin(), values.end());
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    Stats stats;
    stats.minMs = values.front();
    stats.maxMs = values.back();
    stats.meanMs = sum / static_cast<double>(values.size());
    if (values.size() % 2 == 0) {
        stats.medianMs = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2.0;
    } else {
        stats.medianMs = values[values.size() / 2];
    }
    return stats;
}

std::vector<Student> loadStudents(const std::string &path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Cannot open " + path);
    }

    json data;
    in >> data;

    std::vector<Student> students;
    const auto &items = data.at("students");
    students.reserve(items.size());
    for (const auto &item : items) {
        students.push_back({
            item.value("id", ""),
            item.value("name", ""),
            item.value("classId", ""),
            item.value("gpa", 0.0),
        });
    }
    return students;
}

std::vector<std::string> makeIdQueries(const std::vector<Student> &students, std::size_t queryCount) {
    std::vector<std::string> queries;
    queries.reserve(queryCount);
    for (std::size_t i = 0; i < queryCount; ++i) {
        if (i % 20 == 19) {
            queries.push_back("NOT_FOUND_" + std::to_string(i));
        } else {
            queries.push_back(students[(i * 9973ULL + 17ULL) % students.size()].id);
        }
    }
    return queries;
}

std::string pickDenseClassId(const std::vector<Student> &students) {
    if (students.empty()) {
        return "";
    }
    // Sample a small deterministic subset and choose the most frequent class in it.
    std::vector<std::string> sample;
    const std::size_t limit = std::min<std::size_t>(students.size(), 200000);
    sample.reserve(limit);
    for (std::size_t i = 0; i < limit; ++i) {
        sample.push_back(students[(i * 7919ULL) % students.size()].classId);
    }
    std::sort(sample.begin(), sample.end());
    std::string best = sample.front();
    std::size_t bestCount = 1;
    for (std::size_t i = 0; i < sample.size();) {
        std::size_t j = i + 1;
        while (j < sample.size() && sample[j] == sample[i]) {
            ++j;
        }
        if (j - i > bestCount) {
            best = sample[i];
            bestCount = j - i;
        }
        i = j;
    }
    return best;
}

template <typename Func>
std::vector<TimedResult> runTimedSamples(int warmupRuns, int samples, Func &&func) {
    for (int i = 0; i < warmupRuns; ++i) {
        auto warm = func();
        sinkSize += warm.resultCount + static_cast<std::size_t>(std::max<long long>(0, warm.comparisons));
    }

    std::vector<TimedResult> results;
    results.reserve(samples);
    for (int i = 0; i < samples; ++i) {
        results.push_back(func());
        sinkSize += results.back().resultCount + static_cast<std::size_t>(std::max<long long>(0, results.back().comparisons));
    }
    return results;
}

Stats statsOf(const std::vector<TimedResult> &samples) {
    std::vector<double> values;
    values.reserve(samples.size());
    for (const auto &sample : samples) {
        values.push_back(sample.ms);
    }
    return summarize(values);
}

long long medianComparisons(std::vector<TimedResult> samples) {
    std::sort(samples.begin(), samples.end(), [](const TimedResult &a, const TimedResult &b) {
        return a.comparisons < b.comparisons;
    });
    return samples[samples.size() / 2].comparisons;
}

std::size_t medianResultCount(std::vector<TimedResult> samples) {
    std::sort(samples.begin(), samples.end(), [](const TimedResult &a, const TimedResult &b) {
        return a.resultCount < b.resultCount;
    });
    return samples[samples.size() / 2].resultCount;
}

void printStats(const std::string &label, const Stats &stats) {
    std::cout << std::left << std::setw(34) << label
              << std::right << std::fixed << std::setprecision(6)
              << std::setw(14) << stats.minMs
              << std::setw(14) << stats.medianMs
              << std::setw(14) << stats.meanMs
              << std::setw(14) << stats.maxMs << '\n';
}

} // namespace

int main(int argc, char **argv) {
    try {
        const std::string path = argc > 1 ? argv[1] : "data/database.json";
        const int warmupRuns = argc > 2 ? std::stoi(argv[2]) : 5;
        const int samples = argc > 3 ? std::stoi(argv[3]) : 15;
        const std::size_t idQueryCount = argc > 4 ? static_cast<std::size_t>(std::stoull(argv[4])) : 1000;
        const int repeatedQueries = argc > 5 ? std::stoi(argv[5]) : 1000;

        auto loadMs = measureMs([&]() {
            sinkSize = 0;
        });
        auto loadStart = Clock::now();
        std::vector<Student> students = loadStudents(path);
        auto loadEnd = Clock::now();
        loadMs = std::chrono::duration<double, std::milli>(loadEnd - loadStart).count();

        const auto idQueries = makeIdQueries(students, idQueryCount);
        const std::string classId = pickDenseClassId(students);
        const double minGpa = 8.0;
        const double maxGpa = 10.0;

        HashTable hashTable;
        auto hashBuildSamples = runTimedSamples(warmupRuns, samples, [&]() {
            double ms = measureMs([&]() {
                hashTable.build(students);
            });
            return TimedResult{ms, students.size(), static_cast<long long>(hashTable.getCollisions())};
        });

        CustomMaxHeapGpaFinder heapFinder(students);
        auto heapBuildSamples = runTimedSamples(warmupRuns, samples, [&]() {
            double ms = measureMs([&]() {
                heapFinder.buildStructure();
            });
            return TimedResult{ms, students.size(), 0};
        });

        SortedGpaFilter sortedGpa;
        auto sortedBuildSamples = runTimedSamples(warmupRuns, samples, [&]() {
            double ms = measureMs([&]() {
                sortedGpa.build(students);
            });
            return TimedResult{ms, students.size(), 0};
        });

        OptimizedLinearFilter classIndexFilter;
        auto classIndexBuildSamples = runTimedSamples(warmupRuns, samples, [&]() {
            double ms = measureMs([&]() {
                classIndexFilter.build(students);
            });
            return TimedResult{ms, students.size(), 0};
        });

        LinearSearch linearSearch;
        LinearMaxScanGpaFinder linearMax(students);

        auto mc1Linear = runTimedSamples(warmupRuns, samples, [&]() {
            long long comparisons = 0;
            std::size_t found = 0;
            double ms = measureMs([&]() {
                for (const auto &query : idQueries) {
                    const Student *student = linearSearch.search(students, query);
                    if (student != nullptr) {
                        ++found;
                        sinkGpa += student->gpa;
                    }
                    comparisons += static_cast<long long>(linearSearch.getComparisons());
                }
            });
            return TimedResult{ms, found, comparisons};
        });

        auto mc1Hash = runTimedSamples(warmupRuns, samples, [&]() {
            long long probes = 0;
            std::size_t found = 0;
            double ms = measureMs([&]() {
                for (const auto &query : idQueries) {
                    const Student *student = hashTable.search(query);
                    if (student != nullptr) {
                        ++found;
                        sinkGpa += student->gpa;
                    }
                    probes += static_cast<long long>(hashTable.getProbes());
                }
            });
            return TimedResult{ms, found, probes};
        });

        auto mc2Linear = runTimedSamples(warmupRuns, samples, [&]() {
            MC2Result result;
            double ms = measureMs([&]() {
                for (int i = 0; i < repeatedQueries; ++i) {
                    result = linearMax.findMaxGPA();
                    sinkGpa += result.student.gpa;
                }
            });
            return TimedResult{ms, result.found ? 1U : 0U, result.comparisons * repeatedQueries};
        });

        auto mc2Heap = runTimedSamples(warmupRuns, samples, [&]() {
            MC2Result result;
            double ms = measureMs([&]() {
                for (int i = 0; i < repeatedQueries; ++i) {
                    result = heapFinder.findMaxGPA();
                    sinkGpa += result.student.gpa;
                }
            });
            return TimedResult{ms, result.found ? 1U : 0U, 0};
        });

        auto rq1Linear = runTimedSamples(warmupRuns, samples, [&]() {
            LinearFilterResult result;
            double ms = measureMs([&]() {
                result = LinearFilter::filter(students, classId);
            });
            return TimedResult{ms, result.students.size(), result.comparisons};
        });

        auto rq1Optimized = runTimedSamples(warmupRuns, samples, [&]() {
            ClassIndexViewResult result;
            double ms = measureMs([&]() {
                result = classIndexFilter.filterView(classId);
            });
            return TimedResult{ms, result.size(), result.comparisons};
        });

        auto rq2Linear = runTimedSamples(warmupRuns, samples, [&]() {
            FilterGpaResult result;
            double ms = measureMs([&]() {
                result = LinearGpaFilter::filter(students, minGpa, maxGpa);
            });
            return TimedResult{ms, result.students.size(), result.comparisons};
        });

        auto rq2Sorted = runTimedSamples(warmupRuns, samples, [&]() {
            GpaRangeViewResult result;
            double ms = measureMs([&]() {
                result = sortedGpa.filter(minGpa, maxGpa);
            });
            return TimedResult{ms, result.size(), result.comparisons};
        });

        std::cout << "CORE_BENCHMARK_REPORT\n";
        std::cout << "dataset_path=" << path << '\n';
        std::cout << "dataset_size=" << students.size() << '\n';
        std::cout << "load_ms=" << std::fixed << std::setprecision(3) << loadMs << '\n';
        std::cout << "warmup_runs=" << warmupRuns << '\n';
        std::cout << "samples=" << samples << '\n';
        std::cout << "id_query_count=" << idQueryCount << '\n';
        std::cout << "repeated_queries=" << repeatedQueries << '\n';
        std::cout << "class_id=" << classId << '\n';
        std::cout << "gpa_range=[" << minGpa << "," << maxGpa << "]\n";
        std::cout << "hash_build_median_ms=" << statsOf(hashBuildSamples).medianMs << '\n';
        std::cout << "hash_collisions=" << hashTable.getCollisions() << '\n';
        std::cout << "heap_build_median_ms=" << statsOf(heapBuildSamples).medianMs << '\n';
        std::cout << "sorted_gpa_build_median_ms=" << statsOf(sortedBuildSamples).medianMs << '\n';
        std::cout << "class_index_build_median_ms=" << statsOf(classIndexBuildSamples).medianMs << '\n';
        std::cout << '\n';
        std::cout << std::left << std::setw(34) << "metric"
                  << std::right << std::setw(14) << "min_ms"
                  << std::setw(14) << "median_ms"
                  << std::setw(14) << "mean_ms"
                  << std::setw(14) << "max_ms" << '\n';
        printStats("Build HashTable", statsOf(hashBuildSamples));
        printStats("Build MaxHeap", statsOf(heapBuildSamples));
        printStats("Build SortedGPA", statsOf(sortedBuildSamples));
        printStats("Build ClassIndex", statsOf(classIndexBuildSamples));
        printStats("MC1 Linear Search " + std::to_string(idQueryCount) + "q", statsOf(mc1Linear));
        printStats("MC1 Hash Search " + std::to_string(idQueryCount) + "q", statsOf(mc1Hash));
        printStats("MC2 Linear Max " + std::to_string(repeatedQueries) + "q", statsOf(mc2Linear));
        printStats("MC2 Heap Max " + std::to_string(repeatedQueries) + "q", statsOf(mc2Heap));
        printStats("RQ1 Linear Class", statsOf(rq1Linear));
        printStats("RQ1 ClassIndex View", statsOf(rq1Optimized));
        printStats("RQ2 Linear GPA", statsOf(rq2Linear));
        printStats("RQ2 Sorted GPA", statsOf(rq2Sorted));
        std::cout << '\n';
        std::cout << "median_counts_and_work\n";
        std::cout << "MC1_linear_found=" << medianResultCount(mc1Linear)
                  << ", comparisons=" << medianComparisons(mc1Linear) << '\n';
        std::cout << "MC1_hash_found=" << medianResultCount(mc1Hash)
                  << ", probes=" << medianComparisons(mc1Hash) << '\n';
        std::cout << "MC2_linear_comparisons=" << medianComparisons(mc2Linear) << '\n';
        std::cout << "RQ1_linear_matches=" << medianResultCount(rq1Linear)
                  << ", comparisons=" << medianComparisons(rq1Linear) << '\n';
        std::cout << "RQ1_index_matches=" << medianResultCount(rq1Optimized)
                  << ", comparisons=" << medianComparisons(rq1Optimized) << '\n';
        std::cout << "RQ2_linear_matches=" << medianResultCount(rq2Linear)
                  << ", comparisons=" << medianComparisons(rq2Linear) << '\n';
        std::cout << "RQ2_sorted_matches=" << medianResultCount(rq2Sorted)
                  << ", comparisons=" << medianComparisons(rq2Sorted) << '\n';
        std::cout << "sink=" << sinkSize << "," << sinkGpa << '\n';
    } catch (const std::exception &ex) {
        std::cerr << "Benchmark failed: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
