#include "interface/student.h"
#include "interface/core_sorted_gpa/LinearGpaFilter.h"
#include "interface/core_sorted_gpa/SortedGpaFilter.h"
#include "interface/core_class_filter/LinearFilter.h"
#include "interface/core_class_filter/OptimizedLinearFilter.h"
#include "interface/core_heap/CustomMaxHeapGpaFinder.h"
#include "interface/core_heap/LinearMaxScanGpaFinder.h"
#include "interface/core_hash/HashTable.h"
#include "interface/core_hash/LinearSearch.h"
#include "nlohmann/json.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;
using json = nlohmann::json;
using Clock = chrono::steady_clock;

namespace {

volatile size_t sinkSize = 0;
volatile double sinkGpa = 0.0;

struct Stats {
    double minMs{};
    double medianMs{};
    double meanMs{};
    double maxMs{};
};

struct TimedResult {
    double ms{};
    size_t resultCount{};
    long long comparisons{};
};

template <typename Func>
double measureMs(Func &&func) {
    auto start = Clock::now();
    func();
    auto end = Clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

Stats summarize(vector<double> values) {
    sort(values.begin(), values.end());
    double sum = accumulate(values.begin(), values.end(), 0.0);
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

vector<Student> loadStudents(const string &path) {
    ifstream in(path);
    if (!in) {
        throw runtime_error("Cannot open " + path);
    }

    json data;
    in >> data;

    vector<Student> students;
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

vector<Student> generateFastStudents(size_t count, size_t startId = 25150000) {
    static const vector<string> firstNames = {"Nguyen", "Tran", "Le", "Pham", "Hoang", "Phan", "Vu", "Dang", "Bui", "Do"};
    static const vector<string> middleNames = {"Van", "Thi", "Duc", "Minh", "Huu", "Quoc", "Thanh", "Dinh", "Xuan", "Ngoc"};
    static const vector<string> lastNames = {"An", "Binh", "Chau", "Dung", "Em", "Giang", "Hai", "Hung", "Khoa", "Linh", "Minh", "Nam", "Phat", "Quan", "Sang", "Trang", "Tra", "Tung", "Vinh", "Yen"};
    static const vector<string> classPrefixes = {"23DTH", "24DTH", "25DTH", "23KTP", "24KTP", "25KTP", "23ATTT", "24ATTT", "25ATTT"};

    vector<Student> students;
    students.reserve(count);

    mt19937 rng(1337);
    uniform_int_distribution<int> fnDist(0, static_cast<int>(firstNames.size()) - 1);
    uniform_int_distribution<int> mnDist(0, static_cast<int>(middleNames.size()) - 1);
    uniform_int_distribution<int> lnDist(0, static_cast<int>(lastNames.size()) - 1);
    uniform_int_distribution<int> cpDist(0, static_cast<int>(classPrefixes.size()) - 1);
    uniform_int_distribution<int> classNumDist(1, 20);
    uniform_int_distribution<int> gpaIntDist(400, 1000);

    for (size_t i = 0; i < count; ++i) {
        const int classNum = classNumDist(rng);
        students.push_back({
            to_string(startId + i + 1),
            firstNames[fnDist(rng)] + " " + middleNames[mnDist(rng)] + " " + lastNames[lnDist(rng)],
            classPrefixes[cpDist(rng)] + (classNum < 10 ? "0" : "") + to_string(classNum),
            gpaIntDist(rng) / 100.0,
        });
    }
    return students;
}

vector<string> makeIdQueries(const vector<Student> &students, size_t queryCount) {
    vector<string> queries;
    queries.reserve(queryCount);
    for (size_t i = 0; i < queryCount; ++i) {
        if (i % 20 == 19) {
            queries.push_back("NOT_FOUND_" + to_string(i));
        } else {
            queries.push_back(students[(i * 9973ULL + 17ULL) % students.size()].id);
        }
    }
    return queries;
}

string pickDenseClassId(const vector<Student> &students) {
    if (students.empty()) {
        return "";
    }
    // Sample a small deterministic subset and choose the most frequent class in it.
    vector<string> sample;
    const size_t limit = min<size_t>(students.size(), 200000);
    sample.reserve(limit);
    for (size_t i = 0; i < limit; ++i) {
        sample.push_back(students[(i * 7919ULL) % students.size()].classId);
    }
    sort(sample.begin(), sample.end());
    string best = sample.front();
    size_t bestCount = 1;
    for (size_t i = 0; i < sample.size();) {
        size_t j = i + 1;
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
vector<TimedResult> runTimedSamples(int warmupRuns, int samples, Func &&func) {
    for (int i = 0; i < warmupRuns; ++i) {
        auto warm = func();
        sinkSize += warm.resultCount + static_cast<size_t>(max<long long>(0, warm.comparisons));
    }

    vector<TimedResult> results;
    results.reserve(samples);
    for (int i = 0; i < samples; ++i) {
        results.push_back(func());
        sinkSize += results.back().resultCount + static_cast<size_t>(max<long long>(0, results.back().comparisons));
    }
    return results;
}

Stats statsOf(const vector<TimedResult> &samples) {
    vector<double> values;
    values.reserve(samples.size());
    for (const auto &sample : samples) {
        values.push_back(sample.ms);
    }
    return summarize(values);
}

long long medianComparisons(vector<TimedResult> samples) {
    sort(samples.begin(), samples.end(), [](const TimedResult &a, const TimedResult &b) {
        return a.comparisons < b.comparisons;
    });
    return samples[samples.size() / 2].comparisons;
}

size_t medianResultCount(vector<TimedResult> samples) {
    sort(samples.begin(), samples.end(), [](const TimedResult &a, const TimedResult &b) {
        return a.resultCount < b.resultCount;
    });
    return samples[samples.size() / 2].resultCount;
}

void printStats(const string &label, const Stats &stats) {
    cout << left << setw(34) << label
              << right << fixed << setprecision(6)
              << setw(14) << stats.minMs
              << setw(14) << stats.medianMs
              << setw(14) << stats.meanMs
              << setw(14) << stats.maxMs << '\n';
}

} // namespace

int main(int argc, char **argv) {
    try {
        string path = "data/database.json";
        size_t generateCount = 0;
        size_t bulkTarget = 0;
        int argOffset = 1;

        if (argc > 1 && string(argv[1]) == "--generate") {
            generateCount = argc > 2 ? static_cast<size_t>(stoull(argv[2])) : 10000000ULL;
            argOffset = 3;
            path = "generated";
        } else if (argc > 1 && string(argv[1]) == "--bulk-to") {
            bulkTarget = argc > 2 ? static_cast<size_t>(stoull(argv[2])) : 10000000ULL;
            path = argc > 3 ? argv[3] : "data/database.json";
            argOffset = 4;
        } else if (argc > 1) {
            path = argv[1];
            argOffset = 2;
        }

        const int warmupRuns = argc > argOffset ? stoi(argv[argOffset]) : 5;
        const int samples = argc > argOffset + 1 ? stoi(argv[argOffset + 1]) : 15;
        const size_t idQueryCount = argc > argOffset + 2 ? static_cast<size_t>(stoull(argv[argOffset + 2])) : 1000;
        const int repeatedQueries = argc > argOffset + 3 ? stoi(argv[argOffset + 3]) : 1000;

        auto loadMs = measureMs([&]() {
            sinkSize = 0;
        });
        auto loadStart = Clock::now();
        vector<Student> students;
        if (generateCount > 0) {
            students = generateFastStudents(generateCount, 25150000);
        } else {
            students = loadStudents(path);
            if (bulkTarget > students.size()) {
                auto added = generateFastStudents(bulkTarget - students.size(), 25150000 + students.size());
                students.reserve(bulkTarget);
                students.insert(students.end(),
                                make_move_iterator(added.begin()),
                                make_move_iterator(added.end()));
            }
        }
        auto loadEnd = Clock::now();
        loadMs = chrono::duration<double, milli>(loadEnd - loadStart).count();

        const auto idQueries = makeIdQueries(students, idQueryCount);
        const string classId = pickDenseClassId(students);
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

        ClassIndexFilter classIndexFilter;
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
            size_t found = 0;
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
            size_t found = 0;
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

        cout << "CORE_BENCHMARK_REPORT\n";
        cout << "dataset_path=" << path << '\n';
        if (generateCount > 0) {
            cout << "dataset_mode=generate\n";
        } else if (bulkTarget > 0) {
            cout << "dataset_mode=bulk_to_" << bulkTarget << '\n';
        } else {
            cout << "dataset_mode=json\n";
        }
        cout << "dataset_size=" << students.size() << '\n';
        cout << "load_ms=" << fixed << setprecision(3) << loadMs << '\n';
        cout << "warmup_runs=" << warmupRuns << '\n';
        cout << "samples=" << samples << '\n';
        cout << "id_query_count=" << idQueryCount << '\n';
        cout << "repeated_queries=" << repeatedQueries << '\n';
        cout << "class_id=" << classId << '\n';
        cout << "gpa_range=[" << minGpa << "," << maxGpa << "]\n";
        cout << "hash_build_median_ms=" << statsOf(hashBuildSamples).medianMs << '\n';
        cout << "hash_collisions=" << hashTable.getCollisions() << '\n';
        cout << "heap_build_median_ms=" << statsOf(heapBuildSamples).medianMs << '\n';
        cout << "sorted_gpa_build_median_ms=" << statsOf(sortedBuildSamples).medianMs << '\n';
        cout << "class_index_build_median_ms=" << statsOf(classIndexBuildSamples).medianMs << '\n';
        cout << '\n';
        cout << left << setw(34) << "metric"
                  << right << setw(14) << "min_ms"
                  << setw(14) << "median_ms"
                  << setw(14) << "mean_ms"
                  << setw(14) << "max_ms" << '\n';
        printStats("Build HashTable", statsOf(hashBuildSamples));
        printStats("Build MaxHeap", statsOf(heapBuildSamples));
        printStats("Build SortedGPA", statsOf(sortedBuildSamples));
        printStats("Build ClassIndex", statsOf(classIndexBuildSamples));
        printStats("MC1 Linear Search " + to_string(idQueryCount) + "q", statsOf(mc1Linear));
        printStats("MC1 Hash Search " + to_string(idQueryCount) + "q", statsOf(mc1Hash));
        printStats("MC2 Linear Max " + to_string(repeatedQueries) + "q", statsOf(mc2Linear));
        printStats("MC2 Heap Max " + to_string(repeatedQueries) + "q", statsOf(mc2Heap));
        printStats("RQ1 Linear Class", statsOf(rq1Linear));
        printStats("RQ1 ClassIndex View", statsOf(rq1Optimized));
        printStats("RQ2 Linear GPA", statsOf(rq2Linear));
        printStats("RQ2 Sorted GPA", statsOf(rq2Sorted));
        cout << '\n';
        cout << "median_counts_and_work\n";
        cout << "MC1_linear_found=" << medianResultCount(mc1Linear)
                  << ", comparisons=" << medianComparisons(mc1Linear) << '\n';
        cout << "MC1_hash_found=" << medianResultCount(mc1Hash)
                  << ", probes=" << medianComparisons(mc1Hash) << '\n';
        cout << "MC2_linear_comparisons=" << medianComparisons(mc2Linear) << '\n';
        cout << "RQ1_linear_matches=" << medianResultCount(rq1Linear)
                  << ", comparisons=" << medianComparisons(rq1Linear) << '\n';
        cout << "RQ1_index_matches=" << medianResultCount(rq1Optimized)
                  << ", comparisons=" << medianComparisons(rq1Optimized) << '\n';
        cout << "RQ2_linear_matches=" << medianResultCount(rq2Linear)
                  << ", comparisons=" << medianComparisons(rq2Linear) << '\n';
        cout << "RQ2_sorted_matches=" << medianResultCount(rq2Sorted)
                  << ", comparisons=" << medianComparisons(rq2Sorted) << '\n';
        cout << "sink=" << sinkSize << "," << sinkGpa << '\n';
    } catch (const exception &ex) {
        cerr << "Benchmark failed: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
