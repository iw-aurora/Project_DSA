#ifndef LINEAR_FILTER_H
#define LINEAR_FILTER_H

#include <string>
#include <vector>
#include "../student.h"

using namespace std;


// ============================================================================
// STRUCT: LinearFilterResult
// ============================================================================
// Chức năng:
// - Lưu kết quả sau khi Baseline Linear Filter thực hiện.
// - Lưu danh sách Student tìm được.
// - Lưu số phép so sánh classId.
// ============================================================================

struct LinearFilterResult
{
    vector<Student> students;
    long long comparisons;
    LinearFilterResult()
    {
        comparisons = 0;
    }
};
class LinearFilter
{
public:
    static LinearFilterResult filter(
        const vector<Student>& students,
        const string& classId
    );
};
#endif