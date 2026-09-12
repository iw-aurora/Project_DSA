// ============================================================================
// FILE MẪU (DÙNG ĐỂ TEST ĐỌC FILE JSON THÔ RA CONSOLE)
// Lưu ý: File này không dùng trong luồng chính của chương trình.
// ============================================================================

#ifndef STUDENT_DATABASE_H
#define STUDENT_DATABASE_H

#include <string>
#include <vector>
#include "student.h"

using namespace std;

class StudentDatabase {
private:
    vector<Student> students; // Cấu trúc dữ liệu chính lưu trong bộ nhớ RAM

public:
    // Nạp dữ liệu từ file JSON vào RAM
    bool loadFromJson(const string& filename);

    // Lấy tham chiếu danh sách sinh viên đang lưu trong RAM
    const vector<Student>& getStudents() const;

    // Hiển thị danh sách sinh viên trong RAM
    void displayStudents() const;
};

#endif
