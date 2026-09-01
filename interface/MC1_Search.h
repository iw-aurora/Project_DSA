#ifndef MC1_SEARCH_H
#define MC1_SEARCH_H

#include <string>
#include <vector>
#include <unordered_map>
#include "student.h"

using namespace std;

// Cấu trúc lưu trữ kết quả tra cứu và chỉ số đo lường hiệu năng MC1
struct SearchResult {
    bool found;
    Student student;
    double buildTimeMs;
    double queryTimeMs;
    double totalTimeMs;
    long long comparisons;

    // Constructor mặc định khởi tạo giá trị rõ ràng bằng con trỏ 'this->'
    SearchResult() {
        this->found = false;
        this->buildTimeMs = 0.0;
        this->queryTimeMs = 0.0;
        this->totalTimeMs = 0.0;
        this->comparisons = 0;
    }
};

class MC1_Search {
private:
    const vector<Student>* studentsPtr;               // Con trỏ trỏ tới mảng sinh viên trong RAM
    unordered_map<string, size_t> idIndexMap;         // Bảng băm Index (MSSV -> Vị trí)
    bool isIndexBuilt;                                // Cờ kiểm tra đã dựng index chưa
    double indexBuildTimeMs;                          // Thời gian dựng index

public:
    // Constructor nhận tham chiếu mảng sinh viên
    explicit MC1_Search(const vector<Student>& students);

    // Xây dựng Hash Index cho Final Solution
    void buildIndex();

    // Baseline Solution: Linear Search O(N)
    SearchResult searchBaseline(const string& targetId) const;

    // Final Solution: Hash Map Index O(1)
    SearchResult searchFinal(const string& targetId);

    // Chạy benchmark MC1 và hiển thị bảng so sánh thống kê chi tiết
    void runBenchmark(const string& targetId);
};

#endif
