// ============================================================================
// MODULE: TÌM KIẾM SINH VIÊN THEO MSSV (TÁC GIẢ: TRANG)
// ============================================================================

#ifndef FIND_STUDENT_BY_ID_H
#define FIND_STUDENT_BY_ID_H

#include "../student.h"
#include "LinearSearch.h"
#include "HashTable.h"
#include "Benchmark.h"
#include <vector>
#include <string>

using namespace std;

class FindStudentById
{
private:
    const vector<Student> *studentsPtr;

    // Giao diện chọn sinh viên tương tác bằng phím mũi tên Lên/Xuống, phân trang Trái/Phải, ESC để hủy
    int selectStudentInteractive() const;

public:
    explicit FindStudentById(const vector<Student> &students);

    // Thực thi tìm kiếm tương tác: Chọn SV -> Chạy Linear Search & Hash Search -> Xuất kết quả + Benchmark
    void runInteractiveSearch();

    // Thực thi tìm kiếm chỉ bằng giải thuật tối ưu (Hash Table O(1))
    void runFinalSearch();
};

#endif

