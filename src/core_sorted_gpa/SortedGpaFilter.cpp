#include "../../interface/core_sorted_gpa/SortedGpaFilter.h"
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// SortedGPA index:
// - Build stores pointers to the original Student vector, then sorts those
//   pointers by GPA. It does not duplicate Student records.
// - Query returns a view [startIndex, endIndex) into that sorted pointer array.
//   It does not copy K Student records into a result vector.

void SortedGpaFilter::build(const vector<Student> &students)
{
    sortedStudents.clear();
    sortedStudents.reserve(students.size());

    for (const Student &student : students)
    {
        sortedStudents.push_back(&student);
    }

    sort(sortedStudents.begin(), sortedStudents.end(),
         [](const Student *a, const Student *b)
         {
             if (a->gpa != b->gpa)
             {
                 return a->gpa < b->gpa;
             }
             return a->id < b->id;
         });
}

GpaRangeViewResult SortedGpaFilter::filter(double minGpa, double maxGpa)
{
    GpaRangeViewResult result;
    result.sortedStudents = &sortedStudents;

    auto start = high_resolution_clock::now();

    int left = 0;
    int right = static_cast<int>(sortedStudents.size());

    while (left < right)
    {
        int mid = left + (right - left) / 2;
        result.comparisons++;

        if (sortedStudents[mid]->gpa < minGpa)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }
    result.startIndex = static_cast<size_t>(left);

    left = 0;
    right = static_cast<int>(sortedStudents.size());

    while (left < right)
    {
        int mid = left + (right - left) / 2;
        result.comparisons++;

        if (sortedStudents[mid]->gpa <= maxGpa)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }
    result.endIndex = static_cast<size_t>(left);

    auto end = high_resolution_clock::now();
    result.queryTimeMs = duration_cast<microseconds>(end - start).count() / 1000.0;
    result.totalTimeMs = result.queryTimeMs;

    return result;
}
