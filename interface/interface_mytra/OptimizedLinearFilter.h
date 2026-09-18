#ifndef OPTIMIZED_LINEAR_FILTER_H
#define OPTIMIZED_LINEAR_FILTER_H

#include <string>
#include <vector>
#include "../student.h"

using namespace std;

// ============================================================================
// STRUCT: OptimizedFilterResult
// ============================================================================
// Chức năng:
// - Lưu kết quả của Optimized Linear Filter.
// - Thay vì lưu bản sao của từng Student,
//   chỉ lưu vị trí (index) của Student trong vector dữ liệu gốc.
// - Lưu số phép so sánh classId.
//
// Lưu ý:
// - indexes chỉ là kết quả tạm thời của một lần truy vấn.
// - Không lưu indexes lâu dài.
// - Nếu database thay đổi, lần truy vấn tiếp theo sẽ tạo indexes mới.
// ============================================================================

struct OptimizedFilterResult
{
    vector<int> indexes;
    long long comparisons;

    // ------------------------------------------------------------------------
    // Constructor
    // Chức năng:
    // - Khởi tạo số phép so sánh bằng 0.
    // ------------------------------------------------------------------------

    OptimizedFilterResult()
    {
        comparisons = 0;
    }
};

// ============================================================================
// CLASS: OptimizedLinearFilter
// ============================================================================
// Chức năng:
// - Thực hiện Final Solution của RQ1.
// - Vẫn sử dụng Linear Filter, tức là duyệt tuần tự toàn bộ dữ liệu.
// - Cải tiến cách lưu kết quả:
//      Baseline  : lưu bản sao Student.
//      Final     : chỉ lưu index của Student.
//
// Độ phức tạp:
// - Time : O(N)
// - Space: O(k)
//
// N: tổng số sinh viên.
// k: số sinh viên thuộc lớp cần tìm.
// ============================================================================

class OptimizedLinearFilter
{
public:
    // ------------------------------------------------------------------------
    // Hàm: filter()
    //
    // Chức năng:
    // - Tìm tất cả sinh viên thuộc classId cần tìm.
    // - Duyệt toàn bộ vector students.
    // - Nếu classId trùng nhau, lưu index của Student.
    //
    // Input:
    // - students : danh sách sinh viên gốc.
    // - classId  : mã lớp cần tìm.
    //
    // Output:
    // - Trả về OptimizedFilterResult gồm:
    //      + indexes
    //      + số phép so sánh.
    // ------------------------------------------------------------------------

    static OptimizedFilterResult filter(
        const vector<Student> &students,
        const string &classId);
};

#endif // OPTIMIZED_LINEAR_FILTER_H