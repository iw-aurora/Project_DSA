#include "../../interface/interface_minhanh/LinearGpaFilter.h"
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

FilterGpaResult LinearGpaFilter::filter(const vector<Student> &students, double minGpa, double maxGpa)
{
    FilterGpaResult result;
    result.buildTimeMs = 0.0;

    auto start =std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < students.size(); i++)
    {
        result.comparisons++;

        // Lấy sinh viên bằng tham chiếu hằng (const Student &)
        // - Dùng & (tham chiếu): Tránh sao chép (copy) toàn bộ object Student vào bộ nhớ RAM mỗi vòng lặp.
        // - Dùng const: Đảm bảo dữ liệu sinh viên gốc không bị chỉnh sửa nhầm trong quá trình lọc.
        const Student &student = students[i];
        if (student.gpa >= minGpa)
        {
            result.comparisons++;
            if (student.gpa <= maxGpa)
            {
                result.students.push_back(student);
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    result.queryTimeMs = duration_cast<microseconds>(end - start).count() / 1000.0;
    result.totalTimeMs = result.buildTimeMs + result.queryTimeMs;

    return result;
}