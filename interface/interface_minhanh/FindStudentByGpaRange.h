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
#include <utility>
#include "../student.h"
#include "FilterGpaResult.h"     
#include "SortedGpaFilter.h"     

using namespace std;

class FindStudentByGpaRange
{
private:
    const vector<Student> *studentsPtr;
    SortedGpaFilter sortedFilter;
    bool isBuilt;

public:
    FindStudentByGpaRange(const vector<Student> &students);

    pair<double, double> getGpaRangeInData() const;
    void getGpaRangeFromUser(double& minGpa, double& maxGpa);

    FilterGpaResult filterBaseline(double minGpa, double maxGpa);
    FilterGpaResult filterFinalSolution(double minGpa, double maxGpa);

    void runComparison();
    void runFinalSolution();
    void displayResult(double minGpa, double maxGpa, const FilterGpaResult& result) const;
};

#endif