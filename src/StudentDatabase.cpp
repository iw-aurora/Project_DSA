// ============================================================================
// FILE MẪU (DÙNG ĐỂ TEST ĐỌC FILE JSON THÔ RA CONSOLE)
// Lưu ý: File này không dùng trong luồng chính của chương trình.
// ============================================================================

#include "../interface/StudentDatabase.h"
#include "../interface/student.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "../nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// JSON => Student
void from_json(const json& j, Student& student) {
    student.id = j.value("id", "");
    student.name = j.value("name", "");
    student.classId = j.value("classId", "");
    student.gpa = j.value("gpa", 0.0);
}

// Đọc file JSON và hiển thị danh sách sinh viên
void StudentDatabase::displayStudentsFromJson(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "[Loi] Khong the mo file: " << filename << '\n';
        return;
    }

    try {
        json data;
        file >> data;

        if (data.contains("students") && data["students"].is_array()) {
            vector<Student> students = data["students"].get<vector<Student>>();
            
            for (const Student& student : students) {
                cout << "ID: " << student.id 
                     << " | Ten: " << student.name 
                     << " | Lop: " << student.classId 
                     << " | GPA: " << student.gpa << '\n';
            }
        } else {
            cerr << "[Loi] File JSON khong dung dinh dang (thieu mang 'students').\n";
        }
    }
    catch (const json::exception& e) {
        cerr << "[Loi JSON] " << e.what() << '\n';
    }
}
