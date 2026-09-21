// ============================================================================
// MODULE: LỌC SINH VIÊN THEO KHOẢNG GPA (TÁC GIẢ: MINH ANH)
// ============================================================================
// [CHI TIẾT CÁC ĐIỂM ĐÃ SỬA CHỮA & TẠI SAO LÀM VẬY]:
//
// 1. Hàm getGpaRangeInData():
//    - TẠI SAO: Tránh việc hardcode giá trị mặc định cố định (như 8.0 hay 9.5).
//      Khi nạp bất kỳ file CSDL nào, thuật toán tự động quét 1 vòng O(N) để
//      biết GPA nhỏ nhất và lớn nhất thực tế là bao nhiêu để gợi ý cho người dùng.
//
// 2. Tối ưu vòng lặp lọc Baseline trong filterBaseline():
//    - SỬA TỪ: `Student student = this->studentsPtr->at(i);` (sao chép toàn bộ object)
//    - SANG:   `const Student &student = (*this->studentsPtr)[i];` (dùng tham chiếu hằng)
//    - TẠI SAO: Tránh việc tạo bản sao đối tượng Student liên tục trong vòng lặp N phần tử,
//      giúp giảm tải CPU, tiết kiệm RAM và đo lường thời gian truy vấn chính xác hơn.
//
// 3. Gom gọn luồng nhập xuất và Benchmark:
//    - TẠI SAO: Tránh phân mảnh code giữa `main.cpp` và class. `main.cpp` không cần
//      phải biết chi tiết logic nhập xuất, kiểm tra min > max hay in bảng kết quả.
//    - Tự động hoán đổi `swap(minGpa, maxGpa)` nếu người dùng nhập ngược, tăng độ ổn định.
// ============================================================================

#include "../../interface/interface_minhanh/FindStudentByGpaRange.h"
#include "../../interface/interface_minhanh/LinearGpaFilter.h"
#include "../../interface/interface_minhanh/SortedGpaFilter.h"
#include "../../interface/interface_minhanh/Benchmark.h"
#include <iostream>
#include <iomanip>

using namespace std;

FindStudentByGpaRange::FindStudentByGpaRange(const vector<Student> &students)
{
    this->studentsPtr = &students;
}

// ----------------------------------------------------------------------------
// 1. LẤY GPA MIN VÀ MAX TRONG CSDL
// ----------------------------------------------------------------------------
pair<double, double> FindStudentByGpaRange::getGpaRangeInData() const
{
    if (this->studentsPtr == nullptr || this->studentsPtr->empty())
    {
        return {0.0, 0.0};
    }

    double minVal = (*this->studentsPtr)[0].gpa;
    double maxVal = (*this->studentsPtr)[0].gpa;

    for (const auto &s : *this->studentsPtr)
    {
        if (s.gpa < minVal)
            minVal = s.gpa;
        if (s.gpa > maxVal)
            maxVal = s.gpa;
    }

    return {minVal, maxVal};
}

// ----------------------------------------------------------------------------
// 2. NHẬP KHOẢNG GPA TỪ NGƯỜI DÙNG
// MỤC ĐÍCH:
// - Hiển thị thông số GPA [min, max] thực tế đang có trong CSDL làm gợi ý.
// - Lấy dữ liệu minGpa và maxGpa từ bàn phím, có xử lý lỗi nhập liệu (Validation).
// - Tự động sửa lỗi người dùng nhập ngược (min > max) bằng hàm swap().
// ----------------------------------------------------------------------------

void FindStudentByGpaRange::getGpaRangeFromUser( double &minGpa, double &maxGpa)
{
    cout << "======================================================================\n";
    cout << "          CHUONG TRINH LOC SINH VIEN THEO KHOANG GPA (MINHANH)        \n";
    cout << "======================================================================\n";

    auto [actualMin, actualMax] = getGpaRangeInData();
    cout << "Thong ke du lieu CSDL: GPA thap nhat = " << fixed << setprecision(2) << actualMin
         << " | GPA cao nhat = " << actualMax << "\n";
    cout << "----------------------------------------------------------------------\n";

    minGpa = actualMin;
    maxGpa = actualMax;

    cout << "Nhap GPA toi thieu (min) [Mac dinh " << actualMin << "]: ";
    if (!(cin >> minGpa))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        minGpa = actualMin;
    }

    cout << "Nhap GPA toi da (max)   [Mac dinh " << actualMax << "]: ";
    if (!(cin >> maxGpa))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        maxGpa = actualMax;
    }

    if (minGpa > maxGpa)
    {
        cout << "[Luu y] GPA toi thieu lon hon toi da, tu dong hoan doi: [" << maxGpa << " - " << minGpa << "]\n";
        swap(minGpa, maxGpa);
    }
}

// ----------------------------------------------------------------------------
// 3. BASELINE - LINEAR SEARCH
// ----------------------------------------------------------------------------

FilterGpaResult FindStudentByGpaRange::filterBaseline(double minGpa, double maxGpa)
{
    if (studentsPtr == nullptr)
    {
        return FilterGpaResult{};
    }
    const vector<Student> &students = *studentsPtr;
    return LinearGpaFilter::filter(students, minGpa, maxGpa);
    
}

// ----------------------------------------------------------------------------
// 4. FINAL SOLUTION - SORT + BINARY SEARCH
// ----------------------------------------------------------------------------

FilterGpaResult FindStudentByGpaRange::filterFinalSolution(double minGpa, double maxGpa)
{
    if (studentsPtr == nullptr)
    {
        return FilterGpaResult{};
    }
    const vector<Student> &students = *studentsPtr;
    SortedGpaFilter sortedFilter;
    return sortedFilter.filter(students, minGpa, maxGpa);
     
}

// ----------------------------------------------------------------------------
// 5. CHẠY SO SÁNH BASELINE VS FINAL SOLUTION
// ----------------------------------------------------------------------------
void FindStudentByGpaRange::runComparison()
{
    if (studentsPtr == nullptr ||
        studentsPtr->empty())
    {
        cout << "CSDL khong co sinh vien.\n";
        return;
    }

    double minGpa = 0.0;
    double maxGpa = 0.0;

    getGpaRangeFromUser(minGpa,maxGpa);

    FilterGpaResult baseline =filterBaseline(minGpa, maxGpa);
    FilterGpaResult optimized =filterFinalSolution(minGpa, maxGpa);

    Benchmark::printComparison(minGpa, maxGpa, baseline, optimized
    );

    displayResult(minGpa, maxGpa, optimized);
}

void FindStudentByGpaRange::displayResult(double minGpa, double maxGpa, const FilterGpaResult &result) const
{
    cout << "\n======================================================================\n";
    cout << "          DANH SACH SINH VIEN THEO KHOANG GPA                          \n";
    cout << "======================================================================\n";
    cout << "Khoang GPA can loc: [" << fixed << setprecision(2) << minGpa << " - " << maxGpa << "]\n\n";

    if (result.students.empty())
    {
        cout << "Khong tim thay sinh vien nao trong khoang GPA nay.\n";
    }
    else
    {
        cout << "So luong sinh vien thoa dieu kien gpa: " << result.students.size() << "\n\n";
        cout << left << setw(12) << "MSSV"
             << setw(25) << "Ho va ten"
             << setw(15) << "Lop"
             << setw(10) << "GPA" << endl;
        cout << "----------------------------------------------------------------------\n";

        for (const auto &student : result.students)
        {
            cout << left << setw(12) << student.id
                 << setw(25) << student.name
                 << setw(15) << student.classId
                 << setw(10) << fixed << setprecision(2) 
                 << student.gpa 
                 << endl;
        }
    }
    cout << "======================================================================\n";
}

