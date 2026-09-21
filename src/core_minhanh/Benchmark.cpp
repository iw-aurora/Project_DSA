#include "../../interface/interface_minhanh/Benchmark.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Benchmark::printComparison(double minGpa, double maxGpa, const FilterGpaResult& baseline, const FilterGpaResult& optimized)
{
    cout << "=========================================================================================\n";
    cout << "                 BENCHMARK SO SANH THUAT TOAN LOC THEO KHOANG GPA                        \n";
    cout << "=========================================================================================\n";
    cout << "  - Khoang GPA can loc       : [" << fixed << setprecision(2) << minGpa << " - " << maxGpa << "]\n";
    cout << "  - So sinh vien tim thay    : " << optimized.students.size() << " sinh vien\n";
    cout << "-----------------------------------------------------------------------------------------\n";
    cout << left << setw(32) << "TIEU CHI SO SANH"
         << right << setw(25) << "BASELINE LINEAR"
         << setw(25) << "SORTED + BINARY SEARCH" << "\n";
    cout << "-----------------------------------------------------------------------------------------\n";
    cout << left << setw(32) << "Do phuc tap ly thuyet"
         << right << setw(25) << "O(N)"
         << setw(25) << "O(log N) query" << "\n";
    cout << left << setw(32) << "Thoi gian truy van (Query)"
         << right << setw(20) << fixed << setprecision(4) << baseline.queryTimeMs << " ms"
         << setw(20) << fixed << setprecision(4) << optimized.queryTimeMs << " ms" << "\n";
    cout << left << setw(32) << "Tong thoi gian (Total)"
         << right << setw(20) << fixed << setprecision(4) << baseline.totalTimeMs << " ms"
         << setw(20) << fixed << setprecision(4) << optimized.totalTimeMs << " ms" << "\n";
    cout << left << setw(32) << "So phep so sanh"
         << right << setw(25) << baseline.comparisons
         << setw(25) << optimized.comparisons << "\n";
    cout << left << setw(32) << "So luong ket qua"
         << right << setw(25) << baseline.students.size()
         << setw(25) << optimized.students.size() << "\n";
    cout << "=========================================================================================\n";
}
