#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "../student.h"
#include "HashTable.h"
#include "LinearSearch.h"
#include <cstddef>
#include <string>
#include <vector>

using namespace std;

class Benchmark
{
public:
    // Lưu kết quả đo hiệu năng và độ chính xác của hai phương pháp tìm kiếm
    struct Result {
        size_t datasetSize; // Số lượng sinh viên trong dataset
        size_t queryCount; // Số lượng truy vấn benchmark
        double linearQueryTimeMs;  // Thời gian tìm kiếm bằng Linear Search
        double hashBuildTimeMs; // Thời gian xây dựng Hash Index
        double hashQueryTimeMs; // Thời gian tìm kiếm bằng Hash Index
        double hashTotalTimeMs; // Tổng thời gian xây dựng + tìm kiếm
        size_t linearComparisons; // Số phép so sánh trong Linear Search
        size_t hashProbes; // Số lần truy cập ô(probe) trong Hash Index
        size_t hashCollisions; // Số collision trong quá trình build Hash Index
        size_t linearCorrect; // Số truy vấn cho kết quả đúng của Linear Search
        size_t hashCorrect; // Số truy vấn cho kết quả đúng của Hash Index
    };

private:
    // Tạo danh sách truy vấn từ database(dùng chung cho cả 2 thuật toán)
    vector<string> createQueries(
        const vector<Student>& students) const;

public:
    // Chạy benchmark cho cả 2 thuật toán
    Result run(const vector<Student>& students, size_t queryCount = 1000) const;
    void printResult(const Result& result) const;
};

#endif
