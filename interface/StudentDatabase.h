#ifndef STUDENT_DATABASE_H
#define STUDENT_DATABASE_H

#include <string>
using namespace std;

class StudentDatabase {
public:
    // Phương thức duy nhất: Đọc file JSON và hiển thị danh sách sinh viên
    void displayStudentsFromJson(const string& filename);
};

#endif
