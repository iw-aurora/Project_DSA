#include "../../interface/interface_mytra/Benchmark.h"

#include <iostream>
#include <iomanip>

using namespace std;

// Hiển thị kết quả benchmark của 2 phương pháp:
//      1. Baseline Linear Filter
//      2. Optimized Linear Filter
void Benchmark::printComparison(
    const string &classId,
    const BenchmarkResult &baseline,
    const BenchmarkResult &optimized)
{
    cout << "\n=============================================\n";
    cout << "       BENCHMARK - LOC SINH VIEN THEO LOP\n";
    cout << "=============================================\n";
    // Hiển thị thông tin truy vấn
    cout << "Class ID: " << classId << '\n';

    cout << "So sinh vien tim thay: "
         << optimized.resultCount << "\n\n";
    // Thiết lập định dạng bảng
    cout << left
         << setw(25) << "Phuong phap"
         << setw(15) << "Time (ms)"
         << setw(15) << "Comparisons"
         << '\n';
    cout << "-------------------------------------------------------\n";
    cout << fixed << setprecision(6);
    // Hiển thị kết quả Baseline
    cout << left
         << setw(25) << "Baseline Linear"
         << setw(15) << baseline.timeMs
         << setw(15) << baseline.comparisons
         << '\n';
    // Hiển thị kết quả Final Solution
    cout << left
         << setw(25) << "Optimized Linear"
         << setw(15) << optimized.timeMs
         << setw(15) << optimized.comparisons
         << '\n';
    cout << "=============================================\n";
}
// - Hiển thị danh sách sinh viên tìm được.
// - Final Solution không lưu bản sao Student chỉ lưu index của sinh viên trong database.
// - Dùng index để truy cập lại Student gốc:
void Benchmark::printStudents(
    const vector<Student> &students,
    const vector<int> &indexes)
{
    cout << "\n========== DANH SACH SINH VIEN ==========\n";
    cout << left
         << setw(15) << "MSSV"
         << setw(25) << "Ho ten"
         << setw(15) << "Lop"
         << setw(10) << "GPA"
         << '\n';

    cout << "---------------------------------------------------------------\n";
    // Duyệt qua các index mà OptimizedLinearFilter tìm được
    for (int index : indexes)
    {
        // Lấy Student tương ứng từ database gốc.
        //
        // const Student&:
        // - Không tạo bản sao Student.
        // - Chỉ tham chiếu đến Student đang có trong vector students.
        const Student &student = students[index];
        // In thông tin sinh viên
        cout << left
             << setw(15) << student.id
             << setw(25) << student.name
             << setw(15) << student.classId
             << setw(10) << fixed
             << setprecision(2) << student.gpa
             << '\n';
    }
    cout << "---------------------------------------------------------------\n";
}