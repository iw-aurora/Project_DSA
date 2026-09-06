#include "../../interface/interface_minhanh/FindStudentByGpaRange.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>


using namespace std;
using namespace std::chrono;

FindStudentByGpaRange::FindStudentByGpaRange(const vector<Student> &students)
{
    this->studentsPtr = &students;
}

FilterGpaResult FindStudentByGpaRange::filterBaseline(double minGpa, double maxGpa) const
{
    FilterGpaResult result;
    result.buildTimeMs = 0.0;

    auto start = high_resolution_clock::now();

    if (this->studentsPtr != nullptr)
    {
        for (size_t i = 0; i < this->studentsPtr->size(); i++)
        {
            result.comparisons++;
            Student student = this->studentsPtr->at(i);
            if (student.gpa >= minGpa)
            {
                result.comparisons++;
                if (this->studentsPtr->at(i).gpa <= maxGpa)
                {
                    result.students.push_back(student);                
                }
            }
        }
    }

    auto end = high_resolution_clock::now();

    result.queryTimeMs = duration_cast<microseconds>(end - start).count();
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;

    return result;
}

void FindStudentByGpaRange::runBenchmark(double minGpa, double maxGpa, const FilterGpaResult &result) const
{
    cout << "\n======================================================================\n";
    cout << "          KET QUA BENCHMARK (LOC SINH VIEN THEO KHOANG GPA)           \n";
    cout << "======================================================================\n";
    cout << "Khoang GPA can loc: [" << fixed << setprecision(2) << minGpa << " - " << maxGpa << "]\n";

    if (this->studentsPtr == nullptr || result.students.empty())
    {
        cout << "Khong tim thay sinh vien nao trong khoang GPA nay.\n";
    }
    else
    {
        cout << "So luong sinh vien thoa dieu kien gpa: " << result.students.size() << "\n\n";
        cout << left << setw(12) << "MSSV" 
             << setw(25) << "Ho va ten" 
             << setw(15) << "Lop" 
             << setw(10) << "GPA" << endl;
        cout << "----------------------------------------------------------------------\n";

        for (const auto &student : result.students)
        {
            cout << left << setw(12) << student.id 
                 << setw(25) << student.name 
                 << setw(15) << student.classId 
                 << setw(10) << fixed << setprecision(2) << student.gpa << endl;
        }
    }

    cout << "======================================================================\n";
    cout << "                          CHI SO BENCHMARK                            \n";
    cout << "======================================================================\n";
    cout << "Build time : " << result.buildTimeMs << " ms\n";
    cout << "Query time : " << result.queryTimeMs << " ms\n";
    cout << "Total time : " << result.totalTimeMs << " ms\n";
    cout << "So lan so sanh: " << result.comparisons << "\n";
    cout << "======================================================================\n";
}