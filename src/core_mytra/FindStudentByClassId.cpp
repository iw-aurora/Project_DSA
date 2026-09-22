#include "../../interface/interface_mytra/FindStudentByClassId.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;
using namespace chrono;

static void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

FindStudentByClassId::FindStudentByClassId(
    const vector<Student> &students)
{
    studentsPtr = &students;
}

vector<string> FindStudentByClassId::getAvailableClassIds() const
{
    vector<string> classIds;
    if (studentsPtr == nullptr)
    {
        return classIds;
    }

    for (const Student &student : *studentsPtr)
    {
        if (student.classId.empty()) continue;

        bool exists = false;
        for (const string &id : classIds)
        {
            if (id == student.classId)
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

    // Sắp xếp tên lớp theo thứ tự alphabet để dễ nhìn
    sort(classIds.begin(), classIds.end());

    return classIds;
}

// Hàm tương tác chọn mã lớp bằng phím mũi tên Lên/Xuống, phân trang Trái/Phải, ESC để hủy
string FindStudentByClassId::selectClassIdInteractive() const
{
    vector<string> classIds = getAvailableClassIds();

    if (classIds.empty())
    {
        cout << "[Thong bao] Khong co du lieu lop hoc trong CSDL!\n";
        return "";
    }

    const int PAGE_SIZE = 10;
    int totalClasses = static_cast<int>(classIds.size());
    int totalPages = (totalClasses + PAGE_SIZE - 1) / PAGE_SIZE;
    int currentIndex = 0;

    while (true)
    {
        clearScreen();
        int currentPage = currentIndex / PAGE_SIZE;
        int startIdx = currentPage * PAGE_SIZE;
        int endIdx = min(startIdx + PAGE_SIZE, totalClasses);

        cout << "=========================================================================================\n";
        cout << "                  CHON LOP HOC CAN LOC (Trang " << (currentPage + 1) << "/" << totalPages << ")\n";
        cout << "=========================================================================================\n";
        cout << left << setw(6)  << " CHON"
             << " | " << setw(5)  << "STT"
             << " | " << setw(20) << "MA LOP (CLASS ID)"
             << " | " << "SO LUONG SINH VIEN" << "\n";
        cout << "-----------------------------------------------------------------------------------------\n";

        for (int i = startIdx; i < endIdx; i++)
        {
            const string &cId = classIds[i];

            // Đếm số lượng sinh viên trong lớp này để hiển thị trực quan
            int countInClass = 0;
            if (studentsPtr != nullptr)
            {
                for (const auto &st : *studentsPtr)
                {
                    if (st.classId == cId) countInClass++;
                }
            }

            if (i == currentIndex)
            {
                cout << "  -->  | "
                     << right << setw(4) << (i + 1) << " | "
                     << left << setw(20) << cId << " | "
                     << right << setw(4) << countInClass << " sinh vien"
                     << "  <== [DANG CHON]\n";
            }
            else
            {
                cout << "       | "
                     << right << setw(4) << (i + 1) << " | "
                     << left << setw(20) << cId << " | "
                     << right << setw(4) << countInClass << " sinh vien\n";
            }
        }

        cout << "=========================================================================================\n";
        cout << " [HUONG DAN DIEU KHIEN]:                                                                 \n";
        cout << "   [ Phim Len / Xuong ] : Di chuyen con tro chon lop hoc                                 \n";
        cout << "   [ Phim Trai / Phai ] : Chuyen Trang truoc / Trang sau                                 \n";
        cout << "   [ Phim Enter ]       : XAC NHAN CHON LOP DE LOC                                       \n";
        cout << "   [ Phim Esc / q ]     : HUY BO VA QUAY LAI MENU CHINH                                  \n";
        cout << "=========================================================================================\n";

#ifdef _WIN32
        int ch = _getch();
        if (ch == 0 || ch == 224) // Phím mũi tên đặc biệt trên Windows
        {
            int arrow = _getch();
            if (arrow == 72) // Mũi tên LÊN (UP)
            {
                if (currentIndex > 0)
                    currentIndex--;
                else
                    currentIndex = totalClasses - 1; // Vòng xuống cuối
            }
            else if (arrow == 80) // Mũi tên XUỐNG (DOWN)
            {
                if (currentIndex < totalClasses - 1)
                    currentIndex++;
                else
                    currentIndex = 0; // Vòng lên đầu
            }
            else if (arrow == 75) // Mũi tên TRÁI (LEFT - Trang trước)
            {
                currentIndex = max(0, currentIndex - PAGE_SIZE);
            }
            else if (arrow == 77) // Mũi tên PHẢI (RIGHT - Trang sau)
            {
                currentIndex = min(totalClasses - 1, currentIndex + PAGE_SIZE);
            }
        }
        else if (ch == 13) // Phím ENTER
        {
            return classIds[currentIndex];
        }
        else if (ch == 27 || ch == 'q' || ch == 'Q') // Phím ESC hoặc Q
        {
            return ""; // Trả về rỗng biểu thị người dùng hủy
        }
#else
        cout << "Nhap STT lop can chon (1-" << totalClasses << ") hoac 0 de thoat: ";
        int num;
        if (cin >> num && num >= 1 && num <= totalClasses)
        {
            return classIds[num - 1];
        }
        return "";
#endif
    }
}

// HÀM: filterBaseline()
// Quy trình:
// Chọn classId bằng mũi tên/ESC -> chạy Baseline -> chạy Final Solution -> Benchmark -> hiển thị kết quả.
void FindStudentByClassId::filterBaseline()
{
    // 1. CHỌN MÃ LỚP QUA MENU TƯƠNG TÁC PHÍM MŨI TÊN
    string classId = selectClassIdInteractive();

    if (classId.empty())
    {
        cout << "\n[Thong bao] Da huy thao tac loc theo lop.\n";
        return;
    }

    clearScreen();

    // 2. CHẠY BASELINE (Linear Filter)
    auto startBaseline = high_resolution_clock::now();

    LinearFilterResult baselineResult =
        LinearFilter::filter(
            *studentsPtr,
            classId);

    auto endBaseline = high_resolution_clock::now();

    // Tính thời gian Baseline.
    double baselineTime =
        duration<double, milli>(
            endBaseline - startBaseline)
            .count();

    BenchmarkResult baselineBenchmark;
    baselineBenchmark.timeMs = baselineTime;
    baselineBenchmark.comparisons = baselineResult.comparisons;
    baselineBenchmark.resultCount = baselineResult.students.size();

    // 3. CHẠY FINAL SOLUTION (Optimized Linear Filter)
    auto startOptimized = high_resolution_clock::now();

    OptimizedFilterResult optimizedResult =
        OptimizedLinearFilter::filter(
            *studentsPtr,
            classId);

    auto endOptimized = high_resolution_clock::now();

    // Tính thời gian Final.
    double optimizedTime =
        duration<double, milli>(
            endOptimized - startOptimized)
            .count();

    BenchmarkResult optimizedBenchmark;
    optimizedBenchmark.timeMs = optimizedTime;
    optimizedBenchmark.comparisons = optimizedResult.comparisons;
    optimizedBenchmark.resultCount = optimizedResult.indexes.size();

    // 4. HIỂN THỊ BENCHMARK
    Benchmark::printComparison(
        classId,
        baselineBenchmark,
        optimizedBenchmark);

    // 5. HIỂN THỊ DANH SÁCH SINH VIÊN TRONG LỚP
    Benchmark::printStudents(
        *studentsPtr,
        optimizedResult.indexes);
}

// HÀM: filterFinalSolution()
// Chạy trực tiếp thuật toán tối ưu (Optimized Linear Filter) và hiển thị kết quả
void FindStudentByClassId::filterFinalSolution()
{
    // 1. CHỌN MÃ LỚP QUA MENU TƯƠNG TÁC PHÍM MŨI TÊN
    string classId = selectClassIdInteractive();

    if (classId.empty())
    {
        cout << "\n[Thong bao] Da huy thao tac loc theo lop.\n";
        return;
    }

    clearScreen();

    // 2. CHẠY FINAL SOLUTION (Optimized Linear Filter)
    auto startOptimized = high_resolution_clock::now();

    OptimizedFilterResult optimizedResult =
        OptimizedLinearFilter::filter(
            *studentsPtr,
            classId);

    auto endOptimized = high_resolution_clock::now();

    double optimizedTime =
        duration<double, milli>(
            endOptimized - startOptimized)
            .count();

    // 3. HIỂN THỊ THÔNG TIN KẾT QUẢ
    cout << "=========================================================================================\n";
    cout << "             KET QUA LOC SINH VIEN THEO LOP (GIAI THUAT TOI UU: OPTIMIZED INDEX)         \n";
    cout << "=========================================================================================\n";
    cout << "  - Ma lop can loc        : " << classId << "\n";
    cout << "  - So sinh vien tim thay : " << optimizedResult.indexes.size() << " sinh vien\n";
    cout << "  - Thoi gian thuc thi    : " << fixed << setprecision(4) << optimizedTime << " ms\n";
    cout << "  - So phep so sanh       : " << optimizedResult.comparisons << " phep so sanh (O(N))\n";
    cout << "=========================================================================================\n";

    // 4. HIỂN THỊ DANH SÁCH SINH VIÊN TRONG LỚP
    Benchmark::printStudents(
        *studentsPtr,
        optimizedResult.indexes);
}
