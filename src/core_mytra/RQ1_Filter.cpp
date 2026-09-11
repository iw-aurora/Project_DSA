#include "../../interface/interface_mytra/RQ1_Filter.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
using namespace std;
using namespace std::chrono;

// 1. Constructor khởi tạo rõ ràng bằng con trỏ 'this->' trong thân hàm {}
RQ1_Filter::RQ1_Filter(const vector<Student> &students)
{
    this->studentsPtr = &students;
}

// 2.Baseline Solution:Linear Filtering
FilterResult RQ1_Filter::filterBaseline(const string &classId) const
{
    FilterResult result;
    result.buildTimeMs = 0.0; // No build time for baseline
    auto start = high_resolution_clock::now();
    if (this->studentsPtr != nullptr)
    {
        for (size_t i = 0; i < this->studentsPtr->size(); i++)
        {
            result.comparisons++;
            if (this->studentsPtr->at(i).classId == classId)
            {
                appendStudents(result.students, this->studentsPtr->at(i));
            }
        }
    }
    auto end = high_resolution_clock::now();
    result.queryTimeMs = duration_cast<microseconds>(end - start).count();
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;
    return result;
}



void RQ1_Filter::runBenchmark(const string &targetId, const FilterResult &result)
{
    cout << "\n======================================================================\n";
    cout << "KET QUA BRENDMARK(LOC SINH VIEN THEO LOP)" << endl;
    cout << "======================================================================\n";
    cout << "Lop can loc: " << targetId << endl;
    if (this->studentsPtr == nullptr)
    {
        cout << "Danh sach sinh vien rong." << endl;
        return;
    }
    else
    {
        cout << "So luong sinh vien trong lop: " << result.students.size() << endl;
        cout << "MSSV: " << setw(10) << "Ho va ten" << setw(20) << "Lop" << setw(10) << "GPA" << endl;
        for (const auto &student : result.students)
        {
            cout << student.id << setw(20) << student.name << setw(10) << student.classId << setw(10) << fixed << setprecision(2) << student.gpa << endl;
        }
    }
    cout << "======================================================================\n";
    cout << setw(32) << "CHI SO BENCHMARK" << endl;
    cout << "======================================================================\n";
    cout << "Build time: " << result.buildTimeMs << " ms" << endl;
    cout << "Query time: " << result.queryTimeMs << " ms" << endl;
    cout << "Total time: " << result.totalTimeMs << " ms" << endl;
    cout << "So lan so sanh: " << result.comparisons << endl;
    cout << "======================================================================\n";
}