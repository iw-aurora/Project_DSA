#include "../../interface/interface_trang/Benchmark.h"

#include <chrono>
#include <iomanip>
#include <iostream>

using namespace std;

vector<string> Benchmark::createQueries(
    const vector<Student>& students) const {
    vector<string> queries;
    if (students.empty())
        return queries;
    queries.reserve(1000);
    // 950 truy vấn có trong dữ liệu.
    for (size_t i = 0; i < 950; ++i)
        queries.push_back(students[(i * 37) % students.size()].id);
    // 50 truy vấn không có trong dữ liệu.
    for (size_t i = 0; i < 50; ++i)
        queries.push_back("NOT_FOUND_" + std::to_string(i));
    return queries;
}

Benchmark::Result Benchmark::run(
    const vector<Student>& students, size_t queryCount) const {
    Result result{};
    result.datasetSize = students.size();
    if (students.empty() || queryCount == 0)
        return result;
    // Hai thuật toán sử dụng cùng một workload
    vector<string> queries = createQueries(students);
    if (queries.size() > queryCount)
        queries.resize(queryCount);
    else{
        while (queries.size() < queryCount){
            queries.push_back(
                students[queries.size() % students.size()].id);
        }
    }
    result.queryCount = queries.size();

    // -------------------- LINEAR SEARCH --------------------
    LinearSearch linear;
    auto linearStart = chrono::steady_clock::now();
    for (const string& query : queries){
        const Student* found = linear.search(students, query);
        const bool shouldExist = query.rfind("NOT_FOUND_", 0) != 0;
        // Kiểm tra đúng cả kết quả tìm thấy và MSSV
        if (shouldExist){
            if (found != nullptr && found->id == query)
                ++result.linearCorrect;
        }
        else{
            if (found == nullptr)
                ++result.linearCorrect;
        }
        result.linearComparisons += linear.getComparisons();
    }
    auto linearEnd = chrono::steady_clock::now();
    result.linearQueryTimeMs = chrono::duration<double,milli>( linearEnd - linearStart).count();

    // -------------------- HASH TABLE --------------------
    HashTable hashTable;
    auto buildStart = chrono::steady_clock::now();
    hashTable.build(students);
    auto buildEnd = chrono::steady_clock::now();
    result.hashBuildTimeMs = chrono::duration<double, milli>(buildEnd - buildStart).count();
    auto hashStart = chrono::steady_clock::now();
    for (const string& query : queries){
        const Student* found = hashTable.search(query);
        const bool shouldExist = query.rfind("NOT_FOUND_", 0) != 0;
        if (shouldExist){
            if (found != nullptr && found->id == query)
                ++result.hashCorrect;
        }
        else{
            if (found == nullptr)
                ++result.hashCorrect;
        }
        result.hashProbes += hashTable.getProbes();
    }
    auto hashEnd = chrono::steady_clock::now();
    result.hashQueryTimeMs =chrono::duration<double, milli>(hashEnd - hashStart).count();
    result.hashTotalTimeMs = result.hashBuildTimeMs + result.hashQueryTimeMs;
    result.hashCollisions = hashTable.getCollisions();
    return result;
}

void Benchmark::printResult(const Result& result) const{
    cout << "\n============================================================\n";
    cout << "                 BENCHMARK MC1\n";
    cout << "          LINEAR SEARCH VS HASH TABLE\n";
    cout << "============================================================\n";

    cout << "Dataset hien tai: "
              << result.datasetSize
              << " sinh vien\n";

    cout << "So luong query: "
              << result.queryCount
              << "\n\n";

    cout << left << setw(28) << "CHI SO"
              << right << setw(18) << "LINEAR SEARCH"
              << setw(18) << "HASH TABLE"
              << '\n';

    cout << "------------------------------------------------------------\n";

    cout << left << setw(28) << "Build Time (ms)"
              << right << setw(18) << "N/A"
              << setw(18) << fixed << setprecision(4)
              << result.hashBuildTimeMs
              << '\n';

    cout << left << setw(28) << "Query Time (ms)"
              << right << setw(18)
              << result.linearQueryTimeMs
              << setw(18)
              << result.hashQueryTimeMs
              << '\n';

    cout << left << setw(28) << "Total Time (ms)"
              << right << setw(18)
              << result.linearQueryTimeMs
              << setw(18)
              << result.hashTotalTimeMs
              << '\n';

    cout << left << setw(28)
              << "So phep kiem tra / probe"
              << right << setw(18)
              << result.linearComparisons
              << setw(18)
              << result.hashProbes
              << '\n';

    cout << left << setw(28) << "Collision"
              << right << setw(18) << "N/A"
              << setw(18)
              << result.hashCollisions
              << '\n';

    cout << left << setw(28) << "Correctness"
              << right << setw(18)
              << (to_string(result.linearCorrect)
                  + "/" + to_string(result.queryCount))
              << setw(18)
              << (to_string(result.hashCorrect)
                  + "/" + to_string(result.queryCount))
              << '\n';

    cout << "------------------------------------------------------------\n";
    cout << "Baseline: Linear Search -> O(n)\n";
    cout << "Final:    Hash Table    -> O(1) average\n";
    cout << "============================================================\n";
}
