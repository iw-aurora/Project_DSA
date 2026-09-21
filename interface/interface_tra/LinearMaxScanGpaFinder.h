#ifndef LINEAR_MAX_SCAN_GPA_FINDER_H
#define LINEAR_MAX_SCAN_GPA_FINDER_H

#include "../student.h"
#include <vector>

// Định nghĩa cấu trúc kết quả nếu chưa có trong tệp chung
struct MC2Result {
  Student student;
  bool found;
  double queryTimeMs;
  long long comparisons;
};

class LinearMaxScanGpaFinder {
private:
  const std::vector<Student> *studentsPtr;

public:
  explicit LinearMaxScanGpaFinder(const std::vector<Student> &students);

  // Thuật toán quét tuyến tính thuần túy để tìm GPA cao nhất kèm theo đếm số
  // phép so sánh (Nếu hàm trả về kiểu kết quả tùy chỉnh của bạn, hãy giữ nguyên
  // tên kiểu đó) Giả sử kết quả trả về sử dụng kiểu chung hoặc MC2Result
  MC2Result findMaxGPA() const;
};

#endif