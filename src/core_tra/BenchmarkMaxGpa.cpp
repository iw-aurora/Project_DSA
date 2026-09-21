#include "../../interface/interface_tra/BenchmarkMaxGpa.h"
#include "../../interface/interface_tra/CustomMaxHeapGpaFinder.h"
#include "../../interface/interface_tra/LinearMaxScanGpaFinder.h"
#include <iomanip>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

BenchmarkMaxGpa::BenchmarkMaxGpa(const std::vector<Student> &students) {
  this->studentsPtr = &students;
}

void BenchmarkMaxGpa::runComparison(int numIterations) const {
  if (studentsPtr == nullptr || studentsPtr->empty()) {
    std::cout << "\n[Thong bao] Khong co du lieu de thuc hien benchmark.\n";
    return;
  }

  if (numIterations <= 0) {
    numIterations = 1000;
  }

  // 1. Khởi tạo Linear Scan (Baseline không tốn thời gian build)
  // 1. Khởi tạo các bộ tìm kiếm
  LinearMaxScanGpaFinder linear(*studentsPtr);

  auto buildStart = high_resolution_clock::now();
  CustomMaxHeapGpaFinder heap(*studentsPtr);
  auto buildEnd = high_resolution_clock::now();

  // 3. Chạy 1 lần thực thi đơn lẻ (Single query)
  // 2. Chạy 1 lần thực thi đơn lẻ (Single query)
  MC2Result singleLinear = linear.findMaxGPA();
  MC2Result singleHeap = heap.findMaxGPA();

  // 3. Chạy đo lường trên tập lặp lớn (Workload iterations)
  double totalLinearTime = 0.0;
  double totalHeapTime = 0.0;
  long long totalLinearComparisons = 0;
  long long totalHeapComparisons = 0;

  for (int i = 0; i < numIterations; ++i) {
    MC2Result linRes = linear.findMaxGPA();
    MC2Result hpRes = heap.findMaxGPA();

    totalLinearTime += linRes.queryTimeMs;
    totalHeapTime += hpRes.queryTimeMs;

    totalLinearComparisons += linRes.comparisons;
    totalHeapComparisons += hpRes.comparisons;
  }

  // 4. Hiển thị thông tin sinh viên có GPA cao nhất
  cout << "=========================================================================================\n";
  cout << "                     THONG TIN SINH VIEN CO DIEM GPA CAO NHAT                            \n";
  cout << "=========================================================================================\n";
  if (singleLinear.found) {
    cout << "  - MSSV       : " << singleLinear.student.id << "\n";
    cout << "  - Ho va Ten  : " << singleLinear.student.name << "\n";
    cout << "  - Lop        : " << singleLinear.student.classId << "\n";
    cout << "  - Diem GPA   : " << fixed << setprecision(2) << singleLinear.student.gpa << "\n";
  } else {
    cout << "  [!] Khong tim thay sinh vien.\n";
  }
  cout << "-----------------------------------------------------------------------------------------\n";

  // 5. In bảng Benchmark đối chiếu toàn diện chuẩn UI DSA
  cout << "              BENCHMARK SO SANH THUAT TOAN TIM MAX GPA (LINEAR SCAN VS MAX HEAP)         \n";
  cout << "-----------------------------------------------------------------------------------------\n";
  cout << "  - So sinh vien trong CSDL  : " << studentsPtr->size() << " sinh vien\n";
  cout << "  - Tap thu nghiem tong quat : " << numIterations << " lan lap (Workload)\n";
  cout << "-----------------------------------------------------------------------------------------\n";
  cout << left << setw(32) << "TIEU CHI SO SANH"
       << right << setw(25) << "LINEAR SCAN (Baseline)"
       << setw(25) << "CUSTOM MAX HEAP (Optimized)" << "\n";
  cout << "-----------------------------------------------------------------------------------------\n";
  cout << left << setw(32) << "Do phuc tap ly thuyet"
       << right << setw(25) << "O(N)"
       << setw(25) << "O(1) peek / O(N) build" << "\n";
  cout << left << setw(32) << "Thoi gian 1 lan tim Max"
       << right << setw(20) << fixed << setprecision(4) << singleLinear.queryTimeMs << " ms"
       << setw(20) << fixed << setprecision(4) << singleHeap.queryTimeMs << " ms" << "\n";
  cout << left << setw(32) << "Tong thoi gian " + to_string(numIterations) + " lan"
       << right << setw(20) << fixed << setprecision(4) << totalLinearTime << " ms"
       << setw(20) << fixed << setprecision(4) << totalHeapTime << " ms" << "\n";
  cout << left << setw(32) << "So phep so sanh / 1 lan"
       << right << setw(25) << singleLinear.comparisons
       << setw(25) << singleHeap.comparisons << "\n";
  cout << left << setw(32) << "Tong so sanh " + to_string(numIterations) + " lan"
       << right << setw(25) << totalLinearComparisons
       << setw(25) << totalHeapComparisons << "\n";
  cout << "=========================================================================================\n";
}
