#include "../interface/MC2_MaxGPA_Tra.h"
#include <chrono>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace std::chrono;

MC2_MaxGPA::MC2_MaxGPA(const vector<Student> &students) {
  this->studentsPtr = &students;
}

MC2Result MC2_MaxGPA::findMaxGPA() const {
  MC2Result result;

  if (this->studentsPtr == nullptr || this->studentsPtr->empty()) {
    result.found = false;
    return result;
  }

  auto start = high_resolution_clock::now();

  // Khởi tạo ứng cử viên ban đầu là sinh viên đầu tiên
  const Student *bestStudent = &((*this->studentsPtr)[0]);
  result.comparisons = 0;

  for (size_t i = 1; i < this->studentsPtr->size(); ++i) {
    result.comparisons++;
    const Student &current = (*this->studentsPtr)[i];

    // Quy tắc Tie-break: GPA cao hơn hoặc (GPA bằng nhau và ID nhỏ hơn)
    if (current.gpa > bestStudent->gpa) {
      bestStudent = &current;
    } else if (current.gpa == bestStudent->gpa) {
      if (current.id < bestStudent->id) {
        bestStudent = &current;
      }
    }
  }

  auto end = high_resolution_clock::now();

  result.found = true;
  result.student = *bestStudent;
  result.queryTimeMs = duration<double, milli>(end - start).count();

  return result;
}

void MC2_MaxGPA::runBenchmark() {
  MC2Result res = this->findMaxGPA();

  cout << "\n=================================================================="
          "====\n";
  cout << "          KET QUA PHAN TICH & BENCHMARK MC2 (GPA CAO NHAT)\n";
  cout << "===================================================================="
          "==\n";

  if (res.found) {
    cout << "[SINH VIEN DAT GPA CAO NHAT]:\n";
    cout << "  - MSSV: " << res.student.id << "\n";
    cout << "  - Ho ten: " << res.student.name << "\n";
    cout << "  - Lop: " << res.student.classId << "\n";
    cout << "  - GPA: " << fixed << setprecision(2) << res.student.gpa << "\n";
  } else {
    cout << "[KET QUA]: No student data available.\n";
  }

  cout << "\n------------------------------------------------------------------"
          "----\n";
  cout << left << setw(32) << "CHI SO THONG KE" << "GIA TRI\n";
  cout << "--------------------------------------------------------------------"
          "--\n";
  cout << left << setw(32)
       << "1. Query Time (Thoi gian tim):" << res.queryTimeMs << " ms\n";
  cout << left << setw(32) << "2. So phep so sanh (Comp):" << res.comparisons
       << " phep\n";
  cout << "===================================================================="
          "==\n\n";
}