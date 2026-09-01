#ifndef STUDENT_DATABASE_H
#define STUDENT_DATABASE_H

#include <string>
#include <vector>
#include "student.h"

using namespace std;

class StudentDatabase {
private:
    vector<Student> students; // Cấu trúc dữ liệu lưu trữ danh sách sinh viên trong bộ nhớ RAM

public:
    // Nạp dữ liệu từ file JSON vào RAM
    bool loadFromJson(const string& filename);

    // Lấy tham chiếu danh sách sinh viên đang lưu trong RAM
    const vector<Student>& getStudents() const;

    // Hiển thị danh sách sinh viên hiện có trong RAM
    void displayStudents() const;
};

#endif
