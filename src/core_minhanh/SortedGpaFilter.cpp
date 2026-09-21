#include "../../interface/interface_minhanh/SortedGpaFilter.h"
#include <chrono>
#include <algorithm> 

using namespace std;
using namespace std::chrono;

// Thuật toán lọc GPA bằng Sắp xếp và Tìm kiếm nhị phân.
//
// BUILD:
// - Sao chép và sắp xếp sinh viên theo GPA tăng dần.
// - Độ phức tạp: O(N log N).
//
// QUERY:
// - Binary Search tìm startIndex: O(log N).
// - Binary Search tìm endIndex: O(log N).
// - Lấy K sinh viên: O(K).
//
// => QUERY: O(log N + K)
// => BUILD + QUERY: O(N log N + K).

FilterGpaResult SortedGpaFilter::filter(const vector<Student> &students, double minGpa, double maxGpa)
{
    FilterGpaResult result;

    // BUILD: Tạo bản sao và Sắp xếp danh sách sinh viên theo GPA tăng dần
    auto buildStart = high_resolution_clock::now();
    vector<Student> sortedStudents = students;
    sort(sortedStudents.begin(), sortedStudents.end(), [](const Student &a, const Student &b) {
        return a.gpa < b.gpa;
    });

    auto buildEnd = high_resolution_clock::now();
    result.buildTimeMs = duration_cast<microseconds>(buildEnd - buildStart).count() / 1000.0;

    // QUERY: Tìm kiếm nhị phân (Binary Search)
    auto start = high_resolution_clock::now();

    int left = 0;
    int right = sortedStudents.size();

    while (left < right)
    {
        int mid = left + (right - left) / 2;
        result.comparisons++;

        if (sortedStudents[mid].gpa < minGpa)
            left = mid + 1;
        else
            right = mid;
    }
    int startIndex = left;

    left = 0;
    right = sortedStudents.size();

    while (left < right)
    {
        int mid = left + (right - left) / 2;
        result.comparisons++;

        if (sortedStudents[mid].gpa <= maxGpa)
            left = mid + 1;
        else
            right = mid;
    }
    int endIndex = left;

    for (int i = startIndex; i < endIndex; i++)
    {
        result.students.push_back(sortedStudents[i]);
    }

    auto end = high_resolution_clock::now();

    result.queryTimeMs = duration_cast<microseconds>(end - start).count() / 1000.0;
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;

    return result;
}