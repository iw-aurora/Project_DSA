#include "../../interface/interface_mytra/Benchmark.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Benchmark::printComparison(
    const string &classId,
    const BenchmarkResult &baseline,
    const BenchmarkResult &optimized)
{
     const int TEST_LOOPS = 1000;

     // 1. Khung thông tin truy vấn
     cout << "=========================================================================================\n";
     cout << "                            THONG TIN TRUY VAN LOC THEO LOP                              \n";
     cout << "=========================================================================================\n";
     cout << "  - Ma lop can loc           : " << classId << "\n";
     cout << "  - So sinh vien tim thay    : " << optimized.resultCount << " sinh vien\n";
     cout << "  - Tap thu nghiem tong quat : " << TEST_LOOPS << " lan lap (Workload)\n";
     cout << "-----------------------------------------------------------------------------------------\n";

     // 2. Khung bảng Benchmark so sánh hiệu năng
     cout << "                     BENCHMARK SO SANH THUAT TOAN LOC THEO LOP                           \n";
     cout << "-----------------------------------------------------------------------------------------\n";
     cout << left << setw(32) << "TIEU CHI SO SANH"
          << right << setw(25) << "BASELINE LINEAR"
          << setw(25) << "OPTIMIZED LINEAR" << "\n";
     cout << "-----------------------------------------------------------------------------------------\n";
     cout << left << setw(32) << "Do phuc tap ly thuyet"
          << right << setw(25) << "O(N)"
          << setw(25) << "O(N)" << "\n";
     cout << left << setw(32) << "Thoi gian 1 lan loc"
          << right << setw(20) << fixed << setprecision(4) << baseline.timeMs << " ms"
          << setw(20) << fixed << setprecision(4) << optimized.timeMs << " ms" << "\n";
     cout << left << setw(32) << "Tong thoi gian " + to_string(TEST_LOOPS) + " lan"
          << right << setw(20) << fixed << setprecision(4) << (baseline.timeMs * TEST_LOOPS) << " ms"
          << setw(20) << fixed << setprecision(4) << (optimized.timeMs * TEST_LOOPS) << " ms" << "\n";
     cout << left << setw(32) << "So phep so sanh / 1 lan"
          << right << setw(25) << baseline.comparisons
          << setw(25) << optimized.comparisons << "\n";
     cout << left << setw(32) << "Tong so sanh " + to_string(TEST_LOOPS) + " lan"
          << right << setw(25) << (baseline.comparisons * TEST_LOOPS)
          << setw(25) << (optimized.comparisons * TEST_LOOPS) << "\n";
     cout << left << setw(32) << "Cap phat bo nho (Memory)"
          << right << setw(25) << (to_string(baseline.resultCount) + " Student Objects")
          << setw(25) << (to_string(optimized.resultCount) + " Indexes (int)") << "\n";
     cout << left << setw(32) << "So luong ket qua tim thay"
          << right << setw(25) << baseline.resultCount
          << setw(25) << optimized.resultCount << "\n";
     cout << "=========================================================================================\n";
}

// In danh sách sinh viên tìm được với UI bảng chuẩn (rút gọn nếu danh sách dài)
void Benchmark::printStudents(
    const vector<Student> &students,
    const vector<int> &indexes)
{
     cout << "\n=========================================================================================\n";
     cout << "                             DANH SACH SINH VIEN TRONG LOP                              \n";
     cout << "=========================================================================================\n";

     if (indexes.empty())
     {
          cout << "  [!] Khong co sinh vien nao thuoc lop nay.\n";
          cout << "=========================================================================================\n";
          return;
     }

     cout << left << setw(6) << "STT"
          << " | " << setw(12) << "MSSV"
          << " | " << setw(26) << "HO VA TEN"
          << " | " << setw(10) << "LOP"
          << " | " << setw(6) << "GPA" << "\n";
     cout << "-----------------------------------------------------------------------------------------\n";

     size_t total = indexes.size();
     if (total <= 10)
     {
          int stt = 1;
          for (int index : indexes)
          {
               const Student &student = students[index];
               cout << left << setw(6) << stt++
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
               const Student &student = students[indexes[i]];
               cout << left << setw(6) << (i + 1)
                    << " | " << setw(12) << student.id
                    << " | " << setw(26) << student.name
                    << " | " << setw(10) << student.classId
                    << " | " << right << setw(5) << fixed << setprecision(2) << student.gpa << "\n";
          }

          // 2. Dòng dấu chấm rút gọn hiển thị số lượng sinh viên được ẩn
          cout << left << setw(6) << "..."
               << " | " << setw(12) << "..."
               << " | " << setw(26) << ("... (an " + to_string(total - 10) + " SV) ...")
               << " | " << setw(10) << "..."
               << " | " << right << setw(5) << "..." << "\n";

          // 3. In sinh viên cuối cùng
          const Student &lastStudent = students[indexes.back()];
          cout << left << setw(6) << total
               << " | " << setw(12) << lastStudent.id
               << " | " << setw(26) << lastStudent.name
               << " | " << setw(10) << lastStudent.classId
               << " | " << right << setw(5) << fixed << setprecision(2) << lastStudent.gpa << "\n";
     }

     cout << "=========================================================================================\n";
}
