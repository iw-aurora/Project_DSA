#ifndef FIND_STUDENT_BY_MAX_GPA_H
#define FIND_STUDENT_BY_MAX_GPA_H

#include "../student.h"
#include "BenchmarkMaxGpa.h"
#include "CustomMaxHeapGpaFinder.h"
#include "LinearMaxScanGpaFinder.h"
#include <string>
#include <vector>

class FindStudentByMaxGpa
{
private:
  std::vector<Student> studentsData;
  std::string currentMode;

public:
  // Khởi tạo controller với tập dữ liệu truyền vào từ tầng Repository/RAM
  explicit FindStudentByMaxGpa(const std::vector<Student> &students);

  // Thiết lập chế độ chạy cho hệ thống
  void setExecutionMode(const std::string &mode);

  // Điều hướng thực thi nghiệp vụ dựa trên chế độ đã chọn
  void executeQuery() const;

  // Kích hoạt toàn bộ chu trình kiểm thử và báo cáo benchmark
  void runCompleteBenchmarkSuite() const;

  // Thực thi trực tiếp giải thuật tối ưu (Custom Max Heap O(1) peek)
  void runFinalSolution() const;
};

#endif