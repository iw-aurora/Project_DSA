#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <vector>
#include "../student.h"

using namespace std;

struct BenchmarkResult
{
    double timeMs;         // Thời gian thực thi thuật toán (ms)
    long long comparisons; // Số lần so sánh classId
    int resultCount;       // Số sinh viên tìm được
    // Constructor:
    BenchmarkResult()
    {
        timeMs = 0.0;
        comparisons = 0;
        resultCount = 0;
        // Vì Linear Filter không xây dựng secondary data structure, nên không áp dụng build time
    }
};

class Benchmark
{
public:
    // In kết quả benchmark của Baseline và Final Solution
    static void printComparison(
        const string &classId,
        const BenchmarkResult &baseline,
        const BenchmarkResult &optimized);
    // In danh sách sinh viên thuộc lớp cần tìm và nhận danh sách index thay vì copy lại các Student.
    static void printStudents(
        const vector<Student> &students,
        const vector<int> &indexes);
};
#endif