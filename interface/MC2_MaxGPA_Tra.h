#ifndef MC2_MAXGPA_H
#define MC2_MAXGPA_H

#include <vector>
#include <string>
#include "student.h"

using namespace std;

// Cấu trúc lưu trữ kết quả và số liệu benchmark của MC2
struct MC2Result {
    bool found;
    Student student;
    double queryTimeMs;
    long long comparisons;

    MC2Result() {
        this->found = false;
        this->queryTimeMs = 0.0;
        this->comparisons = 0;
    }
};

class MC2_MaxGPA {
private:
    const vector<Student>* studentsPtr; // Con trỏ trỏ tới danh sách sinh viên trong RAM

public:
    // Constructor nhận tham chiếu mảng sinh viên
    explicit MC2_MaxGPA(const vector<Student>& students);

    // Baseline & Final Solution: Quét tuyến tính O(N) tìm GPA cao nhất
    MC2Result findMaxGPA() const;

    // Chạy benchmark và hiển thị báo cáo thống kê chi tiết
    void runBenchmark();
};

#endif