#ifndef MC1_SEARCH_H
#define MC1_SEARCH_H

#include <string>
#include <vector>
#include <unordered_map>
#include "student.h"

using namespace std;

// Cấu trúc lưu trữ kết quả tra cứu và chỉ số đo lường hiệu năng MC1
struct SearchResult {
    bool found = false;
    Student student;
    double buildTimeMs = 0.0;    // Thời gian xây dựng Index (ms)
    double queryTimeMs = 0.0;    // Thời gian thực thi truy vấn (ms)
    double totalTimeMs = 0.0;    // Tổng thời gian = Build + Query (ms)
    long long comparisons = 0;   // Số phép so sánh mã sinh viên
};

class MC1_Search {
private:
    const vector<Student>& studentsRef;         // Tham chiếu đến danh sách sinh viên trong RAM
    unordered_map<string, size_t> idIndexMap;   // Bảng băm Index cho Final Solution (id -> index)
    bool isIndexBuilt = false;                  // Cờ kiểm tra đã dựng index chưa
    double indexBuildTimeMs = 0.0;              // Thời gian dựng index

public:
    // Constructor nhận tham chiếu đến danh sách sinh viên trong RAM
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
