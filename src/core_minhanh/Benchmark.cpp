#include "../../interface/interface_minhanh/Benchmark.h"

#include <iostream>
#include <iomanip>

using namespace std;

void Benchmark::printComparison(double minGpa,double maxGpa, const FilterGpaResult& baseline, const FilterGpaResult& optimized)
{
    cout << "\n=============================================\n";
    cout << "   BENCHMARK - LOC SINH VIEN THEO KHOANG GPA\n";
    cout << "=============================================\n";

    cout << "Khoang GPA: ["
         << fixed << setprecision(2)
         << minGpa << " - " << maxGpa << "]\n";

    cout << "So sinh vien tim thay: "
         << optimized.students.size() << "\n\n";

    cout << left
         << setw(25) << "Phuong phap"
         << setw(15) << "Build (ms)"
         << setw(15) << "Query (ms)"
         << setw(15) << "Total (ms)"
         << setw(15) << "Comparisons"
         << '\n';

    cout << "-----------------------------------------------------------------------\n";

    cout << fixed << setprecision(6);

    cout << left
         << setw(25) << "Baseline Linear"
         << setw(15) << baseline.buildTimeMs
         << setw(15) << baseline.queryTimeMs
         << setw(15) << baseline.totalTimeMs
         << setw(15) << baseline.comparisons
         << '\n';

    cout << left
         << setw(25) << "Optimized Binary"
         << setw(15) << optimized.buildTimeMs
         << setw(15) << optimized.queryTimeMs
         << setw(15) << optimized.totalTimeMs
         << setw(15) << optimized.comparisons
         << '\n';

    cout << "=============================================\n";
}