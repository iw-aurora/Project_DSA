#ifndef FIND_STUDENT_BY_CLASS_ID_H
#define FIND_STUDENT_BY_CLASS_ID_H
#include <string>
#include <vector>
#include "../student.h"
#include "LinearFilter.h"
#include "OptimizedLinearFilter.h"
#include "Benchmark.h"
using namespace std;
class FindStudentByClassId
{
private:
    const vector<Student> *studentsPtr;
    vector<string> getAvailableClassIds() const;

public:
    explicit FindStudentByClassId(
        const vector<Student> &students);

    // Giao diện chọn mã lớp tương tác bằng phím mũi tên Lên/Xuống, ESC để hủy
    string selectClassIdInteractive() const;

    void filterBaseline();
};
#endif
