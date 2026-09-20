#include "../../interface/interface_mytra/FindStudentByClassId.h"
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

FindStudentByClassId::FindStudentByClassId(
    const vector<Student> &students)
{
    studentsPtr = &students;
}

vector<string> FindStudentByClassId::getAvailableClassIds() const
{
    vector<string> classIds;
    for (const Student &student : *studentsPtr)
    {
        bool exists = false;

        for (const string &id : classIds)
        {
            if (id == student.classId)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            classIds.push_back(student.classId);
        }
    }

    return classIds;
}
// HÀM: filter()
// Quy trình:
//Hiển thị danh sách classId -> Nhập classId -> chạy Baseline -> chạy Final Solution -> Benchmark -> hiển thị kết quả.

void FindStudentByClassId::filterBaseline()
{
    // 1. LẤY DANH SÁCH CLASS ID

    vector<string> classIds = getAvailableClassIds();

    // 2. HIỂN THỊ CÁC LỚP HIỆN CÓ

    cout << "\n========== CAC LOP HIEN CO ==========\n";

    for (const string &id : classIds)
    {
        cout << "- " << id << '\n';
    }

    // 3. NHẬN CLASS ID TỪ NGƯỜI DÙNG

    string classId;

    cout << "\nNhap ma lop can tim: ";
    cin >> classId;

    // 4. CHẠY BASELINE

    auto startBaseline = high_resolution_clock::now();

    LinearFilterResult baselineResult =
        LinearFilter::filter(
            *studentsPtr,
            classId);

    auto endBaseline = high_resolution_clock::now();

    // Tính thời gian Baseline.
    double baselineTime =
        duration<double, milli>(
            endBaseline - startBaseline)
            .count();

    // Đưa kết quả Baseline vào BenchmarkResult

    BenchmarkResult baselineBenchmark;

    baselineBenchmark.timeMs = baselineTime;

    baselineBenchmark.comparisons =
        baselineResult.comparisons;

    baselineBenchmark.resultCount =
        baselineResult.students.size();

    // 5. CHẠY FINAL SOLUTION

    auto startOptimized = high_resolution_clock::now();

    OptimizedFilterResult optimizedResult =
        OptimizedLinearFilter::filter(
            *studentsPtr,
            classId);

    auto endOptimized = high_resolution_clock::now();

    // Tính thời gian Final.
    double optimizedTime =
        duration<double, milli>(
            endOptimized - startOptimized)
            .count();

    // Đưa kết quả Final vào BenchmarkResult

    BenchmarkResult optimizedBenchmark;

    optimizedBenchmark.timeMs = optimizedTime;

    optimizedBenchmark.comparisons =
        optimizedResult.comparisons;

    optimizedBenchmark.resultCount =
        optimizedResult.indexes.size();

    // 6. HIỂN THỊ BENCHMARK

    Benchmark::printComparison(
        classId,
        baselineBenchmark,
        optimizedBenchmark);

    // 7. HIỂN THỊ DANH SÁCH SINH VIÊN

    Benchmark::printStudents(
        *studentsPtr,
        optimizedResult.indexes);
}