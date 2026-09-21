// ============================================================================
// MODULE: LỌC SINH VIÊN THEO KHOẢNG GPA (TÁC GIẢ: MINH ANH)
// ============================================================================

#include "../../interface/interface_minhanh/FindStudentByGpaRange.h"
#include "../../interface/interface_minhanh/LinearGpaFilter.h"
#include "../../interface/interface_minhanh/SortedGpaFilter.h"
#include "../../interface/interface_minhanh/Benchmark.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;
using namespace std::chrono;

static void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

FindStudentByGpaRange::FindStudentByGpaRange(const vector<Student> &students)
{
    this->studentsPtr = &students;
    this->isBuilt = false;
}

// ----------------------------------------------------------------------------
// 1. LẤY GPA MIN VÀ MAX TRONG CSDL
// ----------------------------------------------------------------------------
pair<double, double> FindStudentByGpaRange::getGpaRangeInData() const
{
    if (this->studentsPtr == nullptr || this->studentsPtr->empty())
    {
        return {0.0, 10.0};
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
// 2. GIAO DIỆN CHỌN KHOẢNG GPA TƯƠNG TÁC BẰNG PHÍM MŨI TÊN (KHÓA RANH GIỚI CSDL)
// ----------------------------------------------------------------------------
void FindStudentByGpaRange::getGpaRangeFromUser(double &minGpa, double &maxGpa)
{
    auto [actualMin, actualMax] = getGpaRangeInData();

    double currentMin = actualMin;
    double currentMax = actualMax;
    int focusField = 0; // 0: Đang chỉnh Min GPA, 1: Đang chỉnh Max GPA

    while (true)
    {
        clearScreen();
        cout << "=========================================================================================\n";
        cout << "                  LOC SINH VIEN THEO KHOANG GPA - MINH ANH                                \n";
        cout << "=========================================================================================\n";
        cout << "  - Ranh gioi CSDL thuc te: GPA Min = " << fixed << setprecision(2) << actualMin
             << " | GPA Max = " << actualMax << "\n";
        cout << "-----------------------------------------------------------------------------------------\n";
        cout << "  [ HUONG DAN DIEU KHIEN ]:                                                              \n";
        cout << "    [ Phim Len / Xuong ]    : Tang / Giam gia tri GPA (+/- 0.1)                          \n";
        cout << "    [ Phim Trai / Phai ]    : Tang / Giam nhanh (+/- 1.0)                                \n";
        cout << "    [ Phim TAB / Phim 1,2 ] : Chuyen doi giua Min GPA va Max GPA                         \n";
        cout << "    [ Phim Enter ]          : XAC NHAN KHOANG GPA VA CHAY BENCHMARK                      \n";
        cout << "    [ Phim Esc / q ]        : HUY BO VA QUAY LAI MENU CHINH                              \n";
        cout << "-----------------------------------------------------------------------------------------\n";

        if (focusField == 0)
        {
            cout << "  -->  [1] GPA TOI THIEU (Min) : [ " << fixed << setprecision(2) << currentMin << " ]  <== [DANG CHINH]\n";
            cout << "       [2] GPA TOI DA    (Max) : [ " << fixed << setprecision(2) << currentMax << " ]\n";
        }
        else
        {
            cout << "       [1] GPA TOI THIEU (Min) : [ " << fixed << setprecision(2) << currentMin << " ]\n";
            cout << "  -->  [2] GPA TOI DA    (Max) : [ " << fixed << setprecision(2) << currentMax << " ]  <== [DANG CHINH]\n";
        }

        cout << "=========================================================================================\n";
        cout << "  Khoang GPA duoc chon: [ " << fixed << setprecision(2) << currentMin << "  ===>  " << currentMax << " ]\n";
        cout << "=========================================================================================\n";

#ifdef _WIN32
        int ch = _getch();
        if (ch == 0 || ch == 224) // Phím mũi tên
        {
            int arrow = _getch();
            double step = 0.1;

            if (arrow == 72) // Mũi tên LÊN (UP: +0.1)
            {
                if (focusField == 0)
                {
                    currentMin = min(actualMax, currentMin + step);
                    if (currentMin > currentMax) currentMax = currentMin;
                    if (currentMin > currentMax)
                        currentMax = currentMin;
                }
                else
                {
                    currentMax = min(actualMax, currentMax + step);
                }
            }
            else if (arrow == 80) // Mũi tên XUỐNG (DOWN: -0.1)
            {
                if (focusField == 0)
                {
                    currentMin = max(actualMin, currentMin - step);
                }
                else
                {
                    currentMax = max(actualMin, currentMax - step);
                    if (currentMax < currentMin) currentMin = currentMax;
                    if (currentMax < currentMin)
                        currentMin = currentMax;
                }
            }
            else if (arrow == 77) // Mũi tên PHẢI (RIGHT: +1.0)
            {
                if (focusField == 0)
                {
                    currentMin = min(actualMax, currentMin + 1.0);
                    if (currentMin > currentMax) currentMax = currentMin;
                    if (currentMin > currentMax)
                        currentMax = currentMin;
                }
                else
                {
                    currentMax = min(actualMax, currentMax + 1.0);
                }
            }
            else if (arrow == 75) // Mũi tên TRÁI (LEFT: -1.0)
            {
                if (focusField == 0)
                {
                    currentMin = max(actualMin, currentMin - 1.0);
                }
                else
                {
                    currentMax = max(actualMin, currentMax - 1.0);
                    if (currentMax < currentMin) currentMin = currentMax;
                    if (currentMax < currentMin)
                        currentMin = currentMax;
                }
            }
        }
        else if (ch == 9) // Phím TAB: Chuyển trường chỉnh
        {
            focusField = 1 - focusField;
        }
        else if (ch == '1')
        {
            focusField = 0;
        }
        else if (ch == '2')
        {
            focusField = 1;
        }
        else if (ch == 13) // Phím ENTER: Xác nhận
        {
            if (focusField == 0)
            {
                focusField = 1;
            }
            else
            {
                if (currentMin > currentMax)
                    swap(currentMin, currentMax);
                minGpa = currentMin;
                maxGpa = currentMax;
                return;
            }
        }
        else if (ch == 27 || ch == 'q' || ch == 'Q') // Phím ESC: Hủy
        {
            minGpa = -1.0;
            maxGpa = -1.0;
            return;
        }
#else
        cout << "Nhap GPA min va max: ";
        if (cin >> minGpa >> maxGpa) return;
        if (cin >> minGpa >> maxGpa)
            return;
        minGpa = actualMin;
        maxGpa = actualMax;
        return;
#endif
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
    if (!isBuilt)
    {
        sortedFilter.build(students);
        isBuilt = true;
    }
    return sortedFilter.filter(minGpa, maxGpa);
}

// ----------------------------------------------------------------------------
// 5. CHẠY SO SÁNH BASELINE VS FINAL SOLUTION (ĐO LƯỜNG CHÍNH XÁC CAO)
// ----------------------------------------------------------------------------
void FindStudentByGpaRange::runComparison()
{
    if (studentsPtr == nullptr || studentsPtr->empty())
    {
        cout << "CSDL khong co sinh vien.\n";
        return;
    }

    // 1. Đo thời gian Build Sorting (thích ứng theo kích thước tập dữ liệu)

    sortedFilter.build(*studentsPtr);
    isBuilt = true;

    double minGpa = 0.0;
    double maxGpa = 0.0;

    getGpaRangeFromUser(minGpa, maxGpa);

    if (minGpa < 0.0 || maxGpa < 0.0)
    {
        cout << "\n[Thong bao] Da huy thao tac loc theo khoang GPA.\n";
        return;
    }

    clearScreen();

    // 1. Khởi tạo và chạy bộ lọc
    sortedFilter.build(*studentsPtr);
    isBuilt = true;

    // 2. Chạy 1 lần trên khoảng GPA vừa chọn
    FilterGpaResult baseline = filterBaseline(minGpa, maxGpa);
    FilterGpaResult optimized = filterFinalSolution(minGpa, maxGpa);
    

    // 3. Lặp workload để đo thời gian tổng quát ổn định (tự động điều chỉnh theo kích thước CSDL)
    const int TEST_LOOPS = (studentsPtr->size() > 50000) ? 50 : 1000;
    auto linWorkStart = high_resolution_clock::now();
    for (int i = 0; i < TEST_LOOPS; ++i)
    {
        LinearGpaFilter::filter(*studentsPtr, minGpa, maxGpa);
    }
    auto linWorkEnd = high_resolution_clock::now();
    double linWorkTimeMs = duration<double, milli>(linWorkEnd - linWorkStart).count();

    auto binWorkStart = high_resolution_clock::now();
    for (int i = 0; i < TEST_LOOPS; ++i)
    {
        sortedFilter.filter(minGpa, maxGpa);
    }
    auto binWorkEnd = high_resolution_clock::now();
    double binWorkTimeMs = duration<double, milli>(binWorkEnd - binWorkStart).count();

    // Cập nhật kết quả benchmark

    cout << "=========================================================================================\n";
    cout << "                 BENCHMARK SO SANH THUAT TOAN LOC THEO KHOANG GPA                        \n";
    cout << "=========================================================================================\n";
    cout << "  - Khoang GPA can loc       : [" << fixed << setprecision(2) << minGpa << " - " << maxGpa << "]\n";
    cout << "  - So sinh vien tim thay    : " << optimized.students.size() << " sinh vien\n";
    cout << "  - Tap thu nghiem tong quat : " << TEST_LOOPS << " lan lap (Workload)\n";
    cout << "-----------------------------------------------------------------------------------------\n";
    cout << left << setw(32) << "TIEU CHI SO SANH"
         << right << setw(25) << "BASELINE LINEAR"
         << setw(25) << "SORTED + BINARY SEARCH" << "\n";
    cout << "-----------------------------------------------------------------------------------------\n";
    cout << left << setw(32) << "Do phuc tap ly thuyet"
         << right << setw(25) << "O(N)"
         << setw(25) << "O(log N + K)" << "\n";
    cout << left << setw(32) << "Thoi gian 1 lan loc"
         << right << setw(20) << fixed << setprecision(4) << baseline.queryTimeMs << " ms"
         << setw(20) << fixed << setprecision(4) << optimized.queryTimeMs << " ms" << "\n";
    cout << left << setw(32) << "Tong thoi gian " + to_string(TEST_LOOPS) + " lan"
         << right << setw(20) << fixed << setprecision(4) << linWorkTimeMs << " ms"
         << setw(20) << fixed << setprecision(4) << binWorkTimeMs << " ms" << "\n";
    cout << left << setw(32) << "So phep so sanh / 1 lan"
         << right << setw(25) << baseline.comparisons
         << setw(25) << optimized.comparisons << "\n";
    cout << left << setw(32) << "Tong so sanh " + to_string(TEST_LOOPS) + " lan"
         << right << setw(25) << (baseline.comparisons * TEST_LOOPS)
         << setw(25) << (optimized.comparisons * TEST_LOOPS) << "\n";
    cout << left << setw(32) << "So luong ket qua tim thay"
         << right << setw(25) << baseline.students.size()
         << setw(25) << optimized.students.size() << "\n";
    cout << "=========================================================================================\n";

    displayResult(minGpa, maxGpa, optimized);
}

void FindStudentByGpaRange::displayResult(double minGpa, double maxGpa, const FilterGpaResult &result) const
{
    cout << "\n=========================================================================================\n";
    cout << "                             DANH SACH SINH VIEN THEO KHOANG GPA                         \n";
    cout << "=========================================================================================\n";

    if (result.students.empty())
    {
        cout << "  [!] Khong tim thay sinh vien nao trong khoang GPA [" << fixed << setprecision(2) << minGpa << " - " << maxGpa << "].\n";
        cout << "=========================================================================================\n";
        return;
    }

    cout << left << setw(6)  << "STT"
         << " | " << setw(12) << "MSSV"
         << " | " << setw(26) << "HO VA TEN"
         << " | " << setw(10) << "LOP"
         << " | " << setw(6)  << "GPA" << "\n";
    cout << "-----------------------------------------------------------------------------------------\n";

    size_t total = result.students.size();
    if (total <= 10)
    {
        int stt = 1;
        for (const auto &student : result.students)
        {
            cout << left << setw(6)  << stt++
                 << " | " << setw(12) << student.id
                 << " | " << setw(26) << student.name
                 << " | " << setw(10) << student.classId
                 << " | " << right << setw(5) << fixed << setprecision(2) << student.gpa << "\n";
        }
    }
    else
    {
        // 1. In 9 sinh viên đầu tiên
        for (size_t i = 0; i < 9; ++i)
        {
            const auto &student = result.students[i];
            cout << left << setw(6)  << (i + 1)
                 << " | " << setw(12) << student.id
                 << " | " << setw(26) << student.name
                 << " | " << setw(10) << student.classId
                 << " | " << right << setw(5) << fixed << setprecision(2) << student.gpa << "\n";
        }

        // 2. Dòng dấu chấm rút gọn hiển thị số lượng sinh viên được ẩn
        cout << left << setw(6)  << "..."
             << " | " << setw(12) << "..."
             << " | " << setw(26) << ("... (an " + to_string(total - 10) + " SV) ...")
             << " | " << setw(10) << "..."
             << " | " << right << setw(5) << "..." << "\n";

        // 3. In sinh viên cuối cùng
        const auto &lastStudent = result.students.back();
        cout << left << setw(6)  << total
             << " | " << setw(12) << lastStudent.id
             << " | " << setw(26) << lastStudent.name
             << " | " << setw(10) << lastStudent.classId
             << " | " << right << setw(5) << fixed << setprecision(2) << lastStudent.gpa << "\n";
    }

    cout << "=========================================================================================\n";
}
