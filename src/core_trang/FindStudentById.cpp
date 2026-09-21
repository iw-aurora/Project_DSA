// ============================================================================
// MODULE: CÀI ĐẶT TÌM KIẾM SINH VIÊN THEO MSSV (TÁC GIẢ: TRANG)
// ============================================================================

#include "../../interface/interface_trang/FindStudentById.h"
#include "../../interface/interface_trang/LinearSearch.h"
#include "../../interface/interface_trang/HashTable.h"
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

FindStudentById::FindStudentById(const vector<Student> &students)
{
    this->studentsPtr = &students;
}

// ----------------------------------------------------------------------------
// GIAO DIỆN CHỌN SINH VIÊN TƯƠNG TÁC BẰNG PHÍM MŨI TÊN
// ----------------------------------------------------------------------------
int FindStudentById::selectStudentInteractive() const
{
    if (this->studentsPtr == nullptr || this->studentsPtr->empty())
    {
        cout << "[Thong bao] Danh sach sinh vien trong CSDL hien tai dang rong!\n";
        return -1;
    }

    const int PAGE_SIZE = 10;
    int totalStudents = static_cast<int>(this->studentsPtr->size());
    int totalPages = (totalStudents + PAGE_SIZE - 1) / PAGE_SIZE;
    int currentIndex = 0;

    while (true)
    {
        clearScreen();
        int currentPage = currentIndex / PAGE_SIZE;
        int startIdx = currentPage * PAGE_SIZE;
        int endIdx = min(startIdx + PAGE_SIZE, totalStudents);

        cout << "=========================================================================================\n";
        cout << "           TIM KIEM SINH VIEN THEO MSSV - TRANG (Trang " << (currentPage + 1) << "/" << totalPages << ")\n";
        cout << "=========================================================================================\n";
        cout << left << setw(6)  << " CHON"
             << " | " << setw(5)  << "STT"
             << " | " << setw(12) << "MSSV"
             << " | " << setw(26) << "HO VA TEN"
             << " | " << setw(10) << "LOP"
             << " | " << setw(6)  << "GPA" << "\n";
        cout << "-----------------------------------------------------------------------------------------\n";

        for (int i = startIdx; i < endIdx; i++)
        {
            const auto &st = (*this->studentsPtr)[i];
            if (i == currentIndex)
            {
                cout << "  -->  | "
                     << right << setw(4) << (i + 1) << " | "
                     << setw(12) << st.id << " | "
                     << left << setw(26) << st.name << " | "
                     << setw(10) << st.classId << " | "
                     << right << setw(5) << fixed << setprecision(2) << st.gpa
                     << "  <== [DANG CHON]\n";
            }
            else
            {
                cout << "       | "
                     << right << setw(4) << (i + 1) << " | "
                     << setw(12) << st.id << " | "
                     << left << setw(26) << st.name << " | "
                     << setw(10) << st.classId << " | "
                     << right << setw(5) << fixed << setprecision(2) << st.gpa << "\n";
            }
        }

        cout << "=========================================================================================\n";
        cout << " [HUONG DAN]: [Mui ten Len/Xuong]: Chon dong | [Trai/Phai]: Chuyen trang | [Enter]: Tim | [Esc]: Huy\n";
        cout << "=========================================================================================\n";

#ifdef _WIN32
        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            int arrow = _getch();
            if (arrow == 72) // Mũi tên LÊN (UP)
            {
                if (currentIndex > 0)
                    currentIndex--;
                else
                    currentIndex = totalStudents - 1;
            }
            else if (arrow == 80) // Mũi tên XUỐNG (DOWN)
            {
                if (currentIndex < totalStudents - 1)
                    currentIndex++;
                else
                    currentIndex = 0;
            }
            else if (arrow == 75) // Mũi tên TRÁI (LEFT: Trang trước)
            {
                currentIndex = max(0, currentIndex - PAGE_SIZE);
            }
            else if (arrow == 77) // Mũi tên PHẢI (RIGHT: Trang sau)
            {
                currentIndex = min(totalStudents - 1, currentIndex + PAGE_SIZE);
            }
        }
        else if (ch == 13) // Phím ENTER
        {
            return currentIndex;
        }
        else if (ch == 27 || ch == 'q' || ch == 'Q') // Phím ESC hoặc Q
        {
            return -1;
        }
#else
        cout << "Nhap STT sinh vien can chon (1-" << totalStudents << ") hoac 0 de thoat: ";
        int num;
        if (cin >> num && num >= 1 && num <= totalStudents)
        {
            return num - 1;
        }
        return -1;
#endif
    }
}

// ----------------------------------------------------------------------------
// THỰC THI TÌM KIẾM TƯƠNG TÁC + SO SÁNH BENCHMARK TỔNG QUÁT VÀ CHI TIẾT
// ----------------------------------------------------------------------------
void FindStudentById::runInteractiveSearch()
{
    int selectedIdx = selectStudentInteractive();
    if (selectedIdx < 0 || selectedIdx >= static_cast<int>(this->studentsPtr->size()))
    {
        cout << "\n[Thong bao] Da huy thao tac tim kiem.\n";
        return;
    }

    clearScreen();
    string targetId = (*this->studentsPtr)[selectedIdx].id;

    // 1. Khởi tạo Hash Table
    HashTable hashTable;
    hashTable.build(*this->studentsPtr);

    // 2. Chạy Linear Search trên mẫu chọn (Single query)
    LinearSearch linearSearch;
    auto linearStart = high_resolution_clock::now();
    const Student *linearResult = linearSearch.search(*this->studentsPtr, targetId);
    auto linearEnd = high_resolution_clock::now();
    double linearSingleTimeMs = duration<double, milli>(linearEnd - linearStart).count();
    size_t linearSingleComparisons = linearSearch.getComparisons();

    // 3. Chạy Hash Search trên mẫu chọn
    auto hashStart = high_resolution_clock::now();
    const Student *hashResult = hashTable.search(targetId);
    auto hashEnd = high_resolution_clock::now();
    double hashSingleTimeMs = duration<double, milli>(hashEnd - hashStart).count();

    // 4. Chạy Benchmark khối lượng lớn (Workload 1000 queries) để có góc nhìn tổng quát
    const int BENCHMARK_QUERIES = 1000;
    
    // Test 1000 truy vấn trên Linear Search
    auto linearMultiStart = high_resolution_clock::now();
    size_t totalLinearComparisons = 0;
    for (int i = 0; i < BENCHMARK_QUERIES; ++i)
    {
        string queryId = (*this->studentsPtr)[(i * 37) % this->studentsPtr->size()].id;
        linearSearch.search(*this->studentsPtr, queryId);
        totalLinearComparisons += linearSearch.getComparisons();
    }
    auto linearMultiEnd = high_resolution_clock::now();
    double linearMultiTimeMs = duration<double, milli>(linearMultiEnd - linearMultiStart).count();

    // Test 1000 truy vấn trên Hash Table
    auto hashMultiStart = high_resolution_clock::now();
    size_t totalHashProbes = 0;
    for (int i = 0; i < BENCHMARK_QUERIES; ++i)
    {
        string queryId = (*this->studentsPtr)[(i * 37) % this->studentsPtr->size()].id;
        hashTable.search(queryId);
        totalHashProbes += hashTable.getProbes();
    }
    auto hashMultiEnd = high_resolution_clock::now();
    double hashMultiTimeMs = duration<double, milli>(hashMultiEnd - hashMultiStart).count();

    // 5. Hiển thị thông tin sinh viên tìm thấy
    cout << "=========================================================================================\n";
    cout << "                         KET QUA TIM KIEM SINH VIEN THEO MSSV                           \n";
    cout << "=========================================================================================\n";
    if (linearResult != nullptr)
    {
        cout << "  - MSSV       : " << linearResult->id << '\n';
        cout << "  - Ho va Ten  : " << linearResult->name << '\n';
        cout << "  - Lop        : " << linearResult->classId << '\n';
        cout << "  - Diem GPA   : " << fixed << setprecision(2) << linearResult->gpa << '\n';
    }
    else
    {
        cout << "  [!] Khong tim thay sinh vien co MSSV: " << targetId << '\n';
    }
    cout << "-----------------------------------------------------------------------------------------\n";

    // 6. In bảng Benchmark đối chiếu toàn diện (1 query & Tổng quát 1000 queries)
    cout << "              BENCHMARK SO SANH THUAT TOAN TIM KIEM (LINEAR VS HASH TABLE)               \n";
    cout << "-----------------------------------------------------------------------------------------\n";
    cout << "  - So sinh vien trong CSDL  : " << this->studentsPtr->size() << " sinh vien\n";
    cout << "  - Tap thu nghiem tổng quát : " << BENCHMARK_QUERIES << " truy van (Workload)\n";
    cout << "-----------------------------------------------------------------------------------------\n";
    cout << left << setw(32) << "TIEU CHI SO SANH"
         << right << setw(25) << "LINEAR SEARCH (Baseline)"
         << setw(25) << "HASH TABLE (Optimized)" << "\n";
    cout << "-----------------------------------------------------------------------------------------\n";
    cout << left << setw(32) << "Do phuc tap ly thuyet"
         << right << setw(25) << "O(N)"
         << setw(25) << "O(1) average" << "\n";
    cout << left << setw(32) << "Thoi gian tim 1 SV vua chon"
         << right << setw(20) << fixed << setprecision(4) << linearSingleTimeMs << " ms"
         << setw(20) << fixed << setprecision(4) << hashSingleTimeMs << " ms" << "\n";
    cout << left << setw(32) << "Tong thoi gian 1,000 queries"
         << right << setw(20) << fixed << setprecision(4) << linearMultiTimeMs << " ms"
         << setw(20) << fixed << setprecision(4) << hashMultiTimeMs << " ms" << "\n";
    cout << left << setw(32) << "So phep so sanh / 1 SV chon"
         << right << setw(25) << linearSingleComparisons
         << setw(25) << hashTable.getProbes() << "\n";
    cout << left << setw(32) << "So phep so sanh / 1,000 queries"
         << right << setw(25) << totalLinearComparisons
         << setw(25) << totalHashProbes << "\n";
    cout << left << setw(32) << "So va cham (Collisions)"
         << right << setw(25) << "0"
         << setw(25) << hashTable.getCollisions() << "\n";
    cout << "=========================================================================================\n";
}
