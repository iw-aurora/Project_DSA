// ============================================================================
// FILE MẪU (DÙNG ĐỂ TEST ĐỌC FILE JSON THÔ RA CONSOLE)
// Lưu ý: File này không dùng trong luồng chính của chương trình.
// ============================================================================

#ifndef STUDENT_DATABASE_H
#define STUDENT_DATABASE_H

#include <string>
using namespace std;

class StudentDatabase
{
public:
    // Phương thức duy nhất: Đọc file JSON và hiển thị danh sách sinh viên
    void displayStudentsFromJson(const string &filename);
};

#endif
