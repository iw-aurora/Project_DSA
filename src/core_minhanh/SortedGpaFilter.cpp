#include "../../interface/interface_minhanh/SortedGpaFilter.h"
#include <iostream>
#include <chrono>
#include <algorithm> 

using namespace std;
using namespace std::chrono;

// Thuật toán lọc GPA bằng Sắp xếp và Tìm kiếm nhị phân (Binary Search).
//
// MỤC ĐÍCH:
// - Giai đoạn BUILD: Sắp xếp danh sách sinh viên theo GPA tăng dần.
//   Độ phức tạp: O(N log N).
//
// - Giai đoạn QUERY:
//   + Tìm startIndex bằng Binary Search: O(log N).
//   + Tìm endIndex bằng Binary Search: O(log N).
//   + Trích xuất K sinh viên: O(K).
//
// => Tổng độ phức tạp QUERY:
//    O(log N) + O(log N) + O(K)
//    = O(log N + K).
//
// => Tổng BUILD + QUERY:
//    O(N log N + log N + K)
//    = O(N log N + K).
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

    // BƯỚC 1: Tìm vị trí ĐẦU TIÊN có GPA >= minGpa (Tương đương std::lower_bound)
    // GIẢI THÍCH:
    // - Thuật toán chia đôi khoảng tìm kiếm liên tục để tìm phần tử bên trái nhất thỏa mãn.
    // - Độ phức tạp: O(log N).
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

    // BƯỚC 2: Tìm vị trí ĐẦU TIÊN có GPA > maxGpa (Tương đương std::upper_bound)
    // GIẢI THÍCH:
    // - Tiếp tục dùng Binary Search để xác định vị trí đầu tiên vượt quá khoảng maxGpa.
    // - Gán lại left = 0, right = size để tìm trên toàn mảng.
    // - Độ phức tạp: O(log N).
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

    // BƯỚC 3: Trích xuất tập kết quả
    // GIẢI THÍCH: Chỉ duyệt đúng đoạn [startIndex -> endIndex) thay vì duyệt qua N phần tử.
    for (int i = startIndex; i < endIndex; i++)
    {
        result.students.push_back(sortedStudents[i]);
    }

    auto end = high_resolution_clock::now();

    result.queryTimeMs = duration_cast<microseconds>(end - start).count() / 1000.0;
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;

    return result;
}