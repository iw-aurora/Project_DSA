#include "../../interface/interface_tra/BenchmarkMaxGpa.h"
#include "../../interface/interface_tra/CustomMaxHeapGpaFinder.h"
#include "../../interface/interface_tra/LinearMaxScanGpaFinder.h"
#include <iomanip>
#include <iostream>

using namespace std;

BenchmarkMaxGpa::BenchmarkMaxGpa(const std::vector<Student> &students) {
  this->studentsPtr = &students;
}

void BenchmarkMaxGpa::runComparison(int numIterations) const {
  if (studentsPtr == nullptr || studentsPtr->empty()) {
    std::cout << "\n[Benchmark] Khong co du lieu de benchmark.\n";
    return;
  }

  if (numIterations <= 0) {
    std::cout << "\n[Benchmark] So lan lap phai lon hon 0.\n";
    return;
  }

  LinearMaxScanGpaFinder linear(*studentsPtr);
  CustomMaxHeapGpaFinder heap(*studentsPtr);

  MC2Result sampleResult = heap.findMaxGPA();

  double totalLinearTime = 0.0;
  double totalHeapTime = 0.0;

  long long totalLinearComparisons = 0;
  long long totalHeapComparisons = 0;

  MC2Result linearResult;
  MC2Result heapResult;

  for (int i = 0; i < numIterations; ++i) {
    linearResult = linear.findMaxGPA();
    heapResult = heap.findMaxGPA();

    totalLinearTime += linearResult.queryTimeMs;
    totalHeapTime += heapResult.queryTimeMs;

    totalLinearComparisons += linearResult.comparisons;
    totalHeapComparisons += heapResult.comparisons;
  }

  const double avgLinearTime = totalLinearTime / numIterations;
  const double avgHeapTime = totalHeapTime / numIterations;

  const double avgLinearComparisons =
      static_cast<double>(totalLinearComparisons) / numIterations;

  const double avgHeapComparisons =
      static_cast<double>(totalHeapComparisons) / numIterations;

  std::cout << "\n========== BENCHMARK MAX GPA ==========\n";

  if (sampleResult.found) {
    std::cout << "[SINH VIEN DAT GPA CAO NHAT]:\n";
    std::cout << "  - MSSV: " << sampleResult.student.id << "\n";
    std::cout << "  - Ho ten: " << sampleResult.student.name << "\n";
    std::cout << "  - Lop: " << sampleResult.student.classId << "\n";
    std::cout << "  - GPA: " << std::fixed << std::setprecision(2)
              << sampleResult.student.gpa << "\n\n";
  }

  std::cout << "So lan lap: " << numIterations << "\n";
  std::cout << "So sinh vien: " << studentsPtr->size() << "\n\n";

  std::cout << "Baseline - Linear Scan\n";
  std::cout << "  Query Time trung binh: " << avgLinearTime << " ms\n";
  std::cout << "  Comparisons trung binh: " << avgLinearComparisons << "\n";

  std::cout << "\nOptimized - Custom Max Heap\n";
  std::cout << "  Query Time trung binh: " << avgHeapTime << " ms\n";
  std::cout << "  Comparisons trung binh: " << avgHeapComparisons << "\n";

  std::cout << "=======================================\n";
}