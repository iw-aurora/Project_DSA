// ============================================================================
// MODULE: LỌC SINH VIÊN THEO KHOẢNG GPA (TÁC GIẢ: MINH ANH)
// ============================================================================
// [NHẬN XÉT THIẾT KẾ VÀ CÁC THAY ĐỔI / NÂNG CẤP]:
// 1. TÍNH ĐÓNG GÓI (Encapsulation):
//    - Đưa toàn bộ quy trình tương tác (thống kê min/max, nhập khoảng GPA,
//      thực thi lọc và xuất bảng benchmark) vào bên trong class.
//    - Giúp `main.cpp` gọn gàng, chỉ cần gọi `finder.filterBaseline()`.
// 2. DỮ LIỆU ĐỘNG THAY VÌ FIX CỨNG (Dynamic Range):
//    - Bổ sung hàm private `getGpaRangeInData()` để tự động quét khoảng GPA
//      thực tế [min, max] từ CSDL thay vì gán cứng giá trị 8.0 - 9.5.
// 3. TỐI ƯU VÀ TINH GỌN (Optimization & Clean Code):
//    - Loại bỏ hàm overload trùng lặp để tránh deadcode và nhầm lẫn.
//    - Tối ưu truy xuất dữ liệu bằng tham chiếu hằng (const reference).
// ============================================================================

#ifndef FIND_STUDENT_BY_GPA_RANGE_H
#define FIND_STUDENT_BY_GPA_RANGE_H

#include <string>
#include <vector>
#include "../student.h"

using namespace std;

// Cấu trúc lưu trữ kết quả lọc và các chỉ số đo lường hiệu năng (Benchmark)
struct FilterGpaResult
{
    vector<Student> students; // Danh sách sinh viên thỏa điều kiện
    double buildTimeMs;       // Thời gian xây dựng cấu trúc (nếu có)
    double queryTimeMs;       // Thời gian thực thi truy vấn lọc (ms)
    double totalTimeMs;       // Tổng thời gian (ms)
    long long comparisons;    // Số phép so sánh GPA đã thực hiện

    FilterGpaResult()
    {
        this->students = vector<Student>();
        this->buildTimeMs = 0.0;
        this->queryTimeMs = 0.0;
        this->totalTimeMs = 0.0;
        this->comparisons = 0;
    }
};

class FindStudentByGpaRange
{
private:
    const vector<Student> *studentsPtr; // Con trỏ tham chiếu đến mảng dữ liệu sinh viên gốc

    // Hàm phụ trợ: Thống kê GPA nhỏ nhất và lớn nhất thực tế đang có trong CSDL
    pair<double, double> getGpaRangeInData() const;

public:
    // Constructor: Nhận tham chiếu mảng sinh viên (explicit để chống ép kiểu ngầm định)
    explicit FindStudentByGpaRange(const vector<Student> &students);

    // Thực hiện toàn bộ quy trình lọc: Thống kê min/max -> Nhập khoảng GPA -> Đo hiệu năng Baseline -> Xuất bảng Benchmark
    FilterGpaResult filterBaseline();

    // Hiển thị danh sách kết quả và bảng chỉ số benchmark chi tiết
    void runBenchmark(double minGpa, double maxGpa, const FilterGpaResult &result) const;
};

#endif