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
  // Chạy đồng thời Linear và Custom Max Heap để đối chứng số liệu
  LinearMaxScanGpaFinder baseline(*studentsPtr);
  CustomMaxHeapGpaFinder finalSol(*studentsPtr);
  auto resBase = baseline.findMaxGPA();
  auto resFinal = finalSol.findMaxGPA();

  cout << "\n=================================================================="
          "==\n";
  cout << "          KET QUA PHAN TICH & BENCHMARK (GPA CAO NHAT)\n";
  cout << "===================================================================="
          "\n";

  if (resBase.found) {
    cout << "[SINH VIEN DAT GPA CAO NHAT (Baseline Result)]:\n";
    cout << "  - MSSV: " << resBase.student.id << "\n";
    cout << "  - Ho ten: " << resBase.student.name << "\n";
    cout << "  - Lop: " << resBase.student.classId << "\n";
    cout << "  - GPA: " << fixed << setprecision(2) << resBase.student.gpa
         << "\n";
  } else {
    cout << "[KET QUA]: No student data available.\n";
  }

  cout << "\n------------------------------------------------------------------"
          "--\n";
  cout << left << setw(32) << "CHI SO THONG KE" << setw(15) << "BASELINE"
       << setw(15) << "FINAL SOLUTION\n";
  cout << "--------------------------------------------------------------------"
          "\n";
  cout << left << setw(32) << "1. Query Time (ms):" << setw(15)
       << resBase.queryTimeMs << setw(15) << resFinal.queryTimeMs << "\n";
  cout << left << setw(32) << "2. So phep so sanh (Comp):" << setw(15)
       << resBase.comparisons << setw(15) << resFinal.comparisons << "\n";
  cout << "===================================================================="
          "\n\n";
}