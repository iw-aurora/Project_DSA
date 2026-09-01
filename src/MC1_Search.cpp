#include "../interface/MC1_Search.h"
#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// 1. Constructor khởi tạo rõ ràng bằng con trỏ 'this->' trong thân hàm {}
MC1_Search::MC1_Search(const vector<Student>& students) {
    this->studentsPtr = &students;
    this->isIndexBuilt = false;
    this->indexBuildTimeMs = 0.0;
}

// 2. Xây dựng Hash Index cho Final Solution
void MC1_Search::buildIndex() {
    auto start = high_resolution_clock::now();
    
    this->idIndexMap.clear();
    if (this->studentsPtr != nullptr) {
        this->idIndexMap.reserve(this->studentsPtr->size());
        for (size_t i = 0; i < this->studentsPtr->size(); ++i) {
            const Student& currentStudent = (*this->studentsPtr)[i];
            this->idIndexMap[currentStudent.id] = i;
        }
    }
    
    auto end = high_resolution_clock::now();
    this->indexBuildTimeMs = duration<double, milli>(end - start).count();
    this->isIndexBuilt = true;
}

// 3. Baseline Solution: Linear Search O(N)
SearchResult MC1_Search::searchBaseline(const string& targetId) const {
    SearchResult result;
    result.buildTimeMs = 0.0;
    
    auto start = high_resolution_clock::now();
    
    if (this->studentsPtr != nullptr) {
        for (size_t i = 0; i < this->studentsPtr->size(); ++i) {
            result.comparisons = result.comparisons + 1;
            const Student& currentStudent = (*this->studentsPtr)[i];
            if (currentStudent.id == targetId) {
                result.found = true;
                result.student = currentStudent;
                break;
            }
        }
    }
    
    auto end = high_resolution_clock::now();
    result.queryTimeMs = duration<double, milli>(end - start).count();
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;
    
    return result;
}

// 4. Final Solution: Hash Map Index O(1) trung bình
SearchResult MC1_Search::searchFinal(const string& targetId) {
    SearchResult result;
    
    if (this->isIndexBuilt == false) {
        this->buildIndex();
    }
    result.buildTimeMs = this->indexBuildTimeMs;
    
    auto start = high_resolution_clock::now();
    
    result.comparisons = 1;
    auto it = this->idIndexMap.find(targetId);
    if (it != this->idIndexMap.end()) {
        result.found = true;
        size_t index = it->second;
        if (this->studentsPtr != nullptr) {
            result.student = (*this->studentsPtr)[index];
        }
    }
    
    auto end = high_resolution_clock::now();
    result.queryTimeMs = duration<double, milli>(end - start).count();
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;
    
    return result;
}

// 5. Chạy benchmark so sánh MC1 Baseline vs Final Solution
void MC1_Search::runBenchmark(const string& targetId) {
    SearchResult baseRes = this->searchBaseline(targetId);
    SearchResult finalRes = this->searchFinal(targetId);

    cout << "\n======================================================================\n";
    cout << "          KET QUA PHAN TICH & BENCHMARK MC1 (TRA CUU MSSV)\n";
    cout << "======================================================================\n";
    cout << "MSSV Can Tim: " << targetId << "\n\n";

    if (baseRes.found == true) {
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
