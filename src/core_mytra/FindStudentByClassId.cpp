// ============================================================================
// MODULE: LỌC SINH VIÊN THEO LỚP (TÁC GIẢ: MỸ TRÀ)
// ============================================================================
// [CHI TIẾT CÁC ĐIỂM ĐÃ SỬA CHỮA & TẠI SAO LÀM VẬY]:
//
// 1. Hàm getAvailableClassIds():
//    - TẠI SAO: Tránh việc người dùng phải đoán mã lớp. Tự động trích xuất các
//      mã lớp duy nhất (unique) từ CSDL sinh viên để hiển thị ra màn hình cho người dùng chọn.
//
// 2. Tối ưu vòng lặp lọc Baseline trong filterBaseline():
//    - SỬA TỪ: Sao chép object hoặc dùng push nhiều lần
//    - SANG:   `const Student &student = (*this->studentsPtr)[i];` (tham chiếu hằng)
//    - TẠI SAO: Tránh copy dữ liệu sinh viên trong vòng lặp N phần tử, tăng tốc độ xử lý.
//
// 3. Đóng gói quy trình (Encapsulation):
//    - TẠI SAO: `main.cpp` chỉ cần gọi `classFilter.filterBaseline();`, toàn bộ việc
//      in danh sách lớp, nhận input và xuất bảng benchmark được xử lý bên trong class.
// ============================================================================

#include "../../interface/interface_mytra/FindStudentByClassId.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

// 1. Constructor: Lưu con trỏ trỏ tới danh sách sinh viên
FindStudentByClassId::FindStudentByClassId(const vector<Student> &students)
{
    this->studentsPtr = &students;
}

// ----------------------------------------------------------------------------
// Lấy danh sách các mã lớp duy nhất (unique) đang có trong CSDL
// ----------------------------------------------------------------------------
vector<string> FindStudentByClassId::getAvailableClassIds() const
{
    vector<string> classIds;
    if (this->studentsPtr == nullptr)
    {
        return classIds;
    }

    for (const auto &student : *this->studentsPtr)
    {
        if (student.classId.empty())
            continue;

        bool exists = false;
        for (const auto &c : classIds)
        {
            if (c == student.classId)
            {
                exists = true;
                break;
            }
        }
        if (!exists)
        {
            classIds.push_back(student.classId);
        }
    }
    return classIds;
}

// ----------------------------------------------------------------------------
// 2. Toàn bộ quy trình: Hiển thị các lớp -> Nhập lớp -> Lọc Baseline -> Benchmark
// ----------------------------------------------------------------------------
FilterClassResult FindStudentByClassId::filterBaseline()
{
    cout << "======================================================================\n";
    cout << "             CHUONG TRINH LOC SINH VIEN THEO LOP (MY TRA)             \n";
    cout << "======================================================================\n";

    // 1. Hiển thị các lớp hiện có trong CSDL
    vector<string> availableClasses = getAvailableClassIds();
    cout << "Cac lop hien co trong CSDL (" << availableClasses.size() << " lop):\n";
    for (size_t i = 0; i < availableClasses.size(); i++)
    {
        cout << " [" << (i + 1) << "] " << availableClasses[i] << "\t";
        if ((i + 1) % 4 == 0 || i + 1 == availableClasses.size())
            cout << "\n";
    }
    cout << "----------------------------------------------------------------------\n";

    // 2. Nhập mã lớp cần lọc (hỗ trợ nhập tên lớp hoặc số thứ tự)
    string defaultClass = availableClasses.empty() ? "21DTHD1" : availableClasses[0];
    cout << "Nhap ma lop can loc (hoac so thu tu) [Mac dinh: " << defaultClass << "]: ";
    string input;
    cin >> input;

    string targetClass = defaultClass;
    if (!input.empty())
    {
        // Nếu người dùng nhập số thứ tự 1, 2, 3...
        bool isNumber = true;
        for (char c : input)
        {
            if (!isdigit(c))
            {
                isNumber = false;
                break;
            }
        }

        if (isNumber)
        {
            int index = stoi(input);
            if (index >= 1 && index <= (int)availableClasses.size())
            {
                targetClass = availableClasses[index - 1];
            }
            else
            {
                targetClass = input;
            }
        }
        else
        {
            targetClass = input;
        }
    }

    // 3. Thực hiện đo thời gian và lọc Baseline
    FilterClassResult result;
    result.buildTimeMs = 0.0;

    auto start = high_resolution_clock::now();

    if (this->studentsPtr != nullptr)
    {
        for (size_t i = 0; i < this->studentsPtr->size(); i++)
        {
            result.comparisons++;
            const Student &student = (*this->studentsPtr)[i];
            if (student.classId == targetClass)
            {
                result.students.push_back(student);
            }
        }
    }

    auto end = high_resolution_clock::now();

    result.queryTimeMs = duration_cast<microseconds>(end - start).count();
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;

    // 4. Xuất kết quả và bảng chỉ số benchmark
    runBenchmark(targetClass, result);

    return result;
}

// ----------------------------------------------------------------------------
// 3. Hiển thị kết quả lọc và bảng chỉ số benchmark
// ----------------------------------------------------------------------------
void FindStudentByClassId::runBenchmark(const string &classId, const FilterClassResult &result) const
{
    cout << "\n======================================================================\n";
    cout << "          KET QUA BENCHMARK (LOC SINH VIEN THEO LOP)                  \n";
    cout << "======================================================================\n";
    cout << "Lop can loc: " << classId << "\n";

    if (this->studentsPtr == nullptr || result.students.empty())
    {
        cout << "Khong tim thay sinh vien nao trong lop nay.\n";
    }
    else
    {
        cout << "So luong sinh vien trong lop: " << result.students.size() << "\n\n";
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
                 << setw(10) << fixed << setprecision(2) << student.gpa << endl;
        }
    }

    cout << "======================================================================\n";
    cout << "                          CHI SO BENCHMARK                            \n";
    cout << "======================================================================\n";
    cout << "Build time : " << result.buildTimeMs << " ms\n";
    cout << "Query time : " << result.queryTimeMs << " ms\n";
    cout << "Total time : " << result.totalTimeMs << " ms\n";
    cout << "So lan so sanh: " << result.comparisons << "\n";
    cout << "======================================================================\n";
}
