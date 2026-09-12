// ============================================================================
// MODULE: LỌC SINH VIÊN THEO LỚP (TÁC GIẢ: MỸ TRÀ)
// ============================================================================
// [NHẬN XÉT THIẾT KẾ VÀ CÁC THAY ĐỔI / NÂNG CẤP]:
// 1. TÍNH ĐÓNG GÓI (Encapsulation):
//    - Đưa toàn bộ quy trình tương tác (hiển thị danh sách lớp có sẵn,
//      nhập mã lớp, thực thi lọc Baseline và xuất bảng benchmark) vào trong class.
//    - Giúp `main.cpp` gọn gàng, chỉ cần gọi `classFilter.filterBaseline()`.
// 2. DỮ LIỆU ĐỘNG THAY VÌ FIX CỨNG (Dynamic Class Listing):
//    - Bổ sung hàm private `getAvailableClassIds()` để tự động quét tất cả các
//      mã lớp thực tế đang có trong CSDL và hiển thị danh sách cho người dùng chọn.
// 3. TỐI ƯU VÀ TINH GỌN (Optimization & Clean Code):
//    - Tối ưu truy xuất dữ liệu bằng tham chiếu hằng (`const Student &`).
// ============================================================================

#ifndef FIND_STUDENT_BY_CLASS_ID_H
#define FIND_STUDENT_BY_CLASS_ID_H

#include <string>
#include <vector>
#include "../student.h"

using namespace std;

// Cấu trúc lưu trữ kết quả lọc theo lớp và các chỉ số đo lường hiệu năng (Benchmark)
struct FilterClassResult
{
    vector<Student> students; // Danh sách sinh viên thuộc lớp cần lọc
    double buildTimeMs;       // Thời gian xây dựng cấu trúc (nếu có)
    double queryTimeMs;       // Thời gian thực thi truy vấn lọc (ms)
    double totalTimeMs;       // Tổng thời gian (ms)
    long long comparisons;    // Số phép so sánh mã lớp đã thực hiện

    FilterClassResult()
    {
        this->students = vector<Student>();
        this->buildTimeMs = 0.0;
        this->queryTimeMs = 0.0;
        this->totalTimeMs = 0.0;
        this->comparisons = 0;
    }
};

class FindStudentByClassId
{
private:
    const vector<Student> *studentsPtr; // Con trỏ tham chiếu đến mảng dữ liệu sinh viên gốc

    // Hàm phụ trợ: Lấy danh sách các mã lớp duy nhất (distinct) hiện có trong CSDL
    vector<string> getAvailableClassIds() const;

public:
    // Constructor: Nhận tham chiếu danh sách sinh viên
    explicit FindStudentByClassId(const vector<Student> &students);

    // Thực hiện toàn bộ quy trình: Hiển thị các lớp có sẵn -> Nhập mã lớp -> Lọc Baseline -> Xuất benchmark
    FilterClassResult filterBaseline();

    // Hiển thị danh sách kết quả và bảng chỉ số benchmark
    void runBenchmark(const string &classId, const FilterClassResult &result) const;
};

#endif // FIND_STUDENT_BY_CLASS_ID_H
