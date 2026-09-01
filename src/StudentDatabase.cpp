#include "../interface/StudentDatabase.h"
#include "../interface/student.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "../nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// JSON => Student (Chuyển đổi từng phần tử JSON sang struct Student)
void from_json(const json& j, Student& student) {
    student.id = j.value("id", "");
    student.name = j.value("name", "");
    student.classId = j.value("classId", "");
    student.gpa = j.value("gpa", 0.0);
}

// 1. Nạp dữ liệu từ file JSON vào RAM (vector<Student> students)
bool StudentDatabase::loadFromJson(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "[Loi] Khong the mo file: " << filename << '\n';
        return false;
    }

    try {
        json data;
        file >> data;

        if (data.contains("students") && data["students"].is_array()) {
            students = data["students"].get<vector<Student>>();
            cout << "[RAM Success] Da nap " << students.size() << " sinh vien vao bo nho RAM!\n";
            return true;
        } else {
            cerr << "[Loi] File JSON khong dung dinh dang (thieu mang 'students').\n";
            return false;
        }
    }
    catch (const json::exception& e) {
        cerr << "[Loi JSON] " << e.what() << '\n';
        return false;
    }
}

// 2. Lấy danh sách sinh viên đang lưu trong RAM
const vector<Student>& StudentDatabase::getStudents() const {
    return students;
}

// 3. Hiển thị danh sách sinh viên trực tiếp từ RAM
void StudentDatabase::displayStudents() const {
    if (students.empty()) {
        cout << "[Thong bao] Bo nho RAM hien tai khong co sinh vien nao.\n";
        return;
    }

    for (const Student& student : students) {
        cout << "ID: " << student.id 
             << " | Ten: " << student.name 
             << " | Lop: " << student.classId 
             << " | GPA: " << student.gpa << '\n';
    }
}
