#include "../../interface/interface_tra/LinearMaxScanGpaFinder.h"
#include <chrono>

using namespace std::chrono;

LinearMaxScanGpaFinder::LinearMaxScanGpaFinder(
    const std::vector<Student> &students) {
  this->studentsPtr = &students;
}

MC2Result LinearMaxScanGpaFinder::findMaxGPA() const {
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