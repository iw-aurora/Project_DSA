#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "../../interface/interface_minhanh/FindStudentByGpaRange.h"

using namespace std;

class Benchmark
{
public:
    // In kết quả benchmark của Baseline và Final Solution
    static void printComparison( double minGpa, double maxGpa, const FilterGpaResult &baseline, const FilterGpaResult &optimized);
};

#endif