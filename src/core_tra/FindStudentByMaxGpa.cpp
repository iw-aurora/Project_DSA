#include "../../interface/interface_tra/FindStudentByMaxGpa.h"
#include "../../interface/interface_tra/BenchmarkMaxGpa.h"
#include "../../interface/interface_tra/CustomMaxHeapGpaFinder.h"
#include "../../interface/interface_tra/LinearMaxScanGpaFinder.h"
#include <iostream>
#include <iomanip>

using namespace std;

FindStudentByMaxGpa::FindStudentByMaxGpa(const std::vector<Student> &students) {
  this->studentsData = students;
  this->currentMode = "BENCHMARK";
}

void FindStudentByMaxGpa::setExecutionMode(const std::string &mode) {
  this->currentMode = mode;
}

void FindStudentByMaxGpa::executeQuery() const {

  // =========================
  // LINEAR / BASELINE
  // =========================
  if (this->currentMode == "LINEAR" || this->currentMode == "BASELINE") {

    cout << "\n[MC2] Dang chay o che do: LINEAR (Linear Max Scan O(N))\n";

    LinearMaxScanGpaFinder baseline(
        this->studentsData); // Sử dụng tên class mới
    MC2Result res = baseline.findMaxGPA();

    if (res.found) {
      cout << "-> Tim thay sinh vien GPA cao nhat: " << res.student.id << " - "
           << res.student.name << " (GPA: " << res.student.gpa << ")\n";
      cout << "-> Thoi gian: " << res.queryTimeMs
           << " ms | So phep so sanh: " << res.comparisons << "\n";
    } else {
      cout << "-> Khong co du lieu sinh vien.\n";
    }
  }

  // =========================
  // CUSTOM MAX HEAP / FINAL SOLUTION
  // =========================
  else if (this->currentMode == "OPTIMIZED" || this->currentMode == "FINAL" ||
           this->currentMode == "CUSTOM_HEAP") {

    cout << "\n[MC2] Dang chay o che do: FINAL SOLUTION (Custom Max Heap)\n";

    CustomMaxHeapGpaFinder finalSol(this->studentsData);
    MC2Result res = finalSol.findMaxGPA();

    if (res.found) {
      cout << "-> Tim thay sinh vien GPA cao nhat: " << res.student.id << " - "
           << res.student.name << " (GPA: " << res.student.gpa << ")\n";
      cout << "-> Thoi gian: " << res.queryTimeMs
           << " ms | So phep so sanh: " << res.comparisons << "\n";
    } else {
      cout << "-> Khong co du lieu sinh vien.\n";
    }
  }

  // =========================
  // BENCHMARK
  // =========================
  else {
    this->runCompleteBenchmarkSuite();
  }
}

void FindStudentByMaxGpa::runCompleteBenchmarkSuite() const {
  cout << "\n[MC2] Dang chay toan bo bo benchmark so sanh giua Linear Max Scan "
          "va Custom Max Heap...\n";
  BenchmarkMaxGpa benchmark(this->studentsData);
  benchmark.runComparison();
}

void FindStudentByMaxGpa::runFinalSolution() const {
  CustomMaxHeapGpaFinder finalSol(this->studentsData);
  MC2Result res = finalSol.findMaxGPA();

  cout << "=========================================================================================\n";
  cout << "         TIM SINH VIEN CO GPA CAO NHAT (GIAI THUAT TOI UU: CUSTOM MAX HEAP)              \n";
  cout << "=========================================================================================\n";
  if (res.found) {
    cout << "  - MSSV                : " << res.student.id << "\n";
    cout << "  - Ho va Ten           : " << res.student.name << "\n";
    cout << "  - Lop                 : " << res.student.classId << "\n";
    cout << "  - Diem GPA            : " << fixed << setprecision(2) << res.student.gpa << "\n";
    cout << "-----------------------------------------------------------------------------------------\n";
    cout << "  - Do phuc tap ly thuyet: O(1) peek / O(N) heapify\n";
    cout << "  - Thoi gian truy van  : " << fixed << setprecision(4) << res.queryTimeMs << " ms\n";
    cout << "  - So phep so sanh     : " << res.comparisons << " phep so sanh\n";
  } else {
    cout << "  [!] Khong tim thay sinh vien nao trong CSDL.\n";
  }
  cout << "=========================================================================================\n";
}