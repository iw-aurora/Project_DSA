#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include <chrono>
#include <utility>
#include "../student.h"
#include "../../interface/interface_minhanh/FindStudentByGpaRange.h" 

class Benchmark
{
public:

// In bang so sanh nhieu thuat toan cung luc (dung khi co >= 2 ket qua de doi chieu)
void runBenchmarkComparison(double minGpa, double maxGpa, const FilterGpaResult& baseline, const FilterGpaResult& optimized);
};
#endif 