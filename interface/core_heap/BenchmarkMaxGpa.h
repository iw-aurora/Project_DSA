#ifndef BENCHMARK_MAX_GPA_H
#define BENCHMARK_MAX_GPA_H

#include "../student.h"
#include "CustomMaxHeapGpaFinder.h"
#include "LinearMaxScanGpaFinder.h"
#include <string>
#include <vector>

using namespace std;

class BenchmarkMaxGpa {
private:
  const vector<Student> *studentsPtr;

public:
  explicit BenchmarkMaxGpa(const vector<Student> &students);

  // Chạy thực nghiệm so sánh chi tiết giữa Linear và Custom Max Heap ở các mức
  // dữ liệu
  void runComparison(int numIterations = 1000) const;
};

#endif