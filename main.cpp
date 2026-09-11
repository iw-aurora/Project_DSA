#include "interface/interface_minhanh/FindStudentByGpaRange.h"
#include "interface/student.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;
using json = nlohmann::json;

// Hàm riêng nạp dữ liệu từ file JSON và trả về mảng dynamic array (vector<Student>)
vector<Student> loadStudentsData(const string &filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        throw runtime_error("Khong the mo file: " + filePath);
    }

    json data;
    file >> data;

    if (!data.contains("students") || !data["students"].is_array())
    {
        throw runtime_error("File JSON khong dung dinh dang (thieu mang 'students').");
    }

    vector<Student> students;
    for (const auto &item : data["students"])
    {
        students.push_back({item.value("id", ""),
                            item.value("name", ""),
                            item.value("classId", ""),
                            item.value("gpa", 0.0)});
    }

    if (students.empty())
    {
        throw runtime_error("Danh sach sinh vien trong file JSON bi rong!");
    }

    return students;
}

int main()
{
    // ========================================================================
    // BƯỚC NẠP DỮ LIỆU DÙNG CHUNG CHO TOÀN DỰ ÁN (SHARED DATA LOADING)
    // - Đọc dữ liệu sinh viên từ database.json vào mảng động vector<Student>.
    // - Tất cả các thuật toán/module của các thành viên sẽ cùng tái sử dụng
    //   mảng dữ liệu này để thực hiện tìm kiếm, sắp xếp, lọc, benchmark,...
    // ========================================================================
    vector<Student> students;
    try
    {
        students = loadStudentsData("data/database.json");
        cout << "[Thanh cong] Da tai " << students.size() << " sinh vien tu data/database.json.\n\n";
    }
    catch (const exception &e)
    {
        cerr << "[Loi] Khong the nap du lieu: " << e.what() << '\n';
        return 1;
    }

    // 2. Chạy module lọc sinh viên theo khoảng GPA (Minh Anh)
    FindStudentByGpaRange finder(students);
    finder.filterBaseline();

    return 0;
}