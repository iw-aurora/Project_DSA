#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include "../student.h"
#include <cstddef>
#include <string>
#include <vector>

using namespace std;

class LinearSearch
{
private:
    size_t comparisons;
public:
    LinearSearch();
    // Tìm sinh viên theo MSSV bằng cách duyệt tuần tự.
    const Student* search(const vector<Student>& students, const string& targetId);
    // Lấy số phép so sánh của lần search gần nhất
    size_t getComparisons() const;
};

#endif
