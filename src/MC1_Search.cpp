#include "../interface/MC1_Search.h"
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// Constructor
MC1_Search::MC1_Search(const vector<Student>& students)
    : studentsRef(students), isIndexBuilt(false), indexBuildTimeMs(0.0) {}

// Xây dựng Hash Index cho Final Solution
void MC1_Search::buildIndex() {
    auto start = high_resolution_clock::now();
    
    idIndexMap.clear();
    idIndexMap.reserve(studentsRef.size());
    for (size_t i = 0; i < studentsRef.size(); ++i) {
        idIndexMap[studentsRef[i].id] = i;
    }
    
    auto end = high_resolution_clock::now();
    indexBuildTimeMs = duration<double, milli>(end - start).count();
    isIndexBuilt = true;
}

// 1. Baseline Solution: Linear Search O(N)
SearchResult MC1_Search::searchBaseline(const string& targetId) const {
    SearchResult result;
    result.buildTimeMs = 0.0; // Baseline không tốn chi phí dựng index
    
    auto start = high_resolution_clock::now();
    
    for (size_t i = 0; i < studentsRef.size(); ++i) {
        result.comparisons++;
        if (studentsRef[i].id == targetId) {
            result.found = true;
            result.student = studentsRef[i];
            break;
        }
    }
    
    auto end = high_resolution_clock::now();
    result.queryTimeMs = duration<double, milli>(end - start).count();
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;
    
    return result;
}

// 2. Final Solution: Hash Map Index O(1) trung bình
SearchResult MC1_Search::searchFinal(const string& targetId) {
    SearchResult result;
    
    if (!isIndexBuilt) {
        buildIndex();
    }
    result.buildTimeMs = indexBuildTimeMs;
    
    auto start = high_resolution_clock::now();
    
    result.comparisons = 1; // Kiểm tra hash table 1 lần
    auto it = idIndexMap.find(targetId);
    if (it != idIndexMap.end()) {
        result.found = true;
        result.student = studentsRef[it->second];
    }
    
    auto end = high_resolution_clock::now();
    result.queryTimeMs = duration<double, milli>(end - start).count();
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;
    
    return result;
}

// 3. Chạy benchmark so sánh MC1 Baseline vs Final Solution
void MC1_Search::runBenchmark(const string& targetId) {
    SearchResult baseRes = searchBaseline(targetId);
    SearchResult finalRes = searchFinal(targetId);

    cout << "\n======================================================================\n";
    cout << "          KET QUA PHAN TICH & BENCHMARK MC1 (TRA CUU MSSV)\n";
    cout << "======================================================================\n";
    cout << "MSSV Can Tim: " << targetId << "\n\n";

    if (baseRes.found) {
        cout << "[KET QUA THAY]:\n";
        cout << "  - MSSV: " << baseRes.student.id << "\n";
        cout << "  - Ho ten: " << baseRes.student.name << "\n";
        cout << "  - Lop: " << baseRes.student.classId << "\n";
        cout << "  - GPA: " << fixed << setprecision(2) << baseRes.student.gpa << "\n";
    } else {
        cout << "[KET QUA]: Student not found.\n";
    }

    cout << "\n----------------------------------------------------------------------\n";
    cout << left << setw(32) << "CHI SO THONG KE" 
         << setw(26) << "BASELINE (Linear Search)" 
         << setw(26) << "FINAL SOLUTION (Hash Index)" << "\n";
    cout << "----------------------------------------------------------------------\n";

    cout << left << setw(32) << "1. Build Time (Thoi gian dung):" 
         << setw(26) << (to_string(baseRes.buildTimeMs) + " ms")
         << setw(26) << (to_string(finalRes.buildTimeMs) + " ms") << "\n";

    cout << left << setw(32) << "2. Query Time (Thoi gian tim):" 
         << setw(26) << (to_string(baseRes.queryTimeMs) + " ms")
         << setw(26) << (to_string(finalRes.queryTimeMs) + " ms") << "\n";

    cout << left << setw(32) << "3. Total Time (Tong thoi gian):" 
         << setw(26) << (to_string(baseRes.totalTimeMs) + " ms")
         << setw(26) << (to_string(finalRes.totalTimeMs) + " ms") << "\n";

    cout << left << setw(32) << "4. So phep so sanh (Comp):" 
         << setw(26) << (to_string(baseRes.comparisons) + " phep")
         << setw(26) << (to_string(finalRes.comparisons) + " phep") << "\n";

    cout << "----------------------------------------------------------------------\n";
    
    if (finalRes.queryTimeMs > 0 && baseRes.queryTimeMs > 0) {
        double speedup = baseRes.queryTimeMs / finalRes.queryTimeMs;
        cout << "=> [DANH GIA]: Final Solution nhanh hon Baseline " 
             << fixed << setprecision(2) << speedup << " lan o buoc truy van (Query Time)!\n";
    } else if (baseRes.comparisons > finalRes.comparisons) {
        cout << "=> [DANH GIA]: Final Solution giam so phep so sanh tu " 
             << baseRes.comparisons << " xuong con " << finalRes.comparisons << " phep!\n";
    }
    cout << "======================================================================\n\n";
}
