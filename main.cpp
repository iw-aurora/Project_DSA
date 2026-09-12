// ============================================================================
// CHƯƠNG TRÌNH CHÍNH - HỆ THỐNG QUẢN LÝ SINH VIÊN (DASA230179)
// ============================================================================

#include "interface/interface_minhanh/FindStudentByGpaRange.h" // [Module Minh Anh: Lọc theo GPA]
#include "interface/interface_mytra/FindStudentByClassId.h"    // [Module Mỹ Trà: Lọc theo lớp]
#include "interface/interface_phat/StudentCRUD.h"              // [Module Phát: Thêm - Sửa - Xóa]
#include "interface/student.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>

#ifdef _WIN32
#include <conio.h>
#endif

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

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen()
{
#ifdef _WIN32
    cout << "\nNhan phim bat ky de tiep tuc...";
    _getch();
#else
    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

void showMainMenu()
{
    cout << "====================================================\n";
    cout << "      HE THONG QUAN LY SINH VIEN - DASA230179       \n";
    cout << "====================================================\n";
    cout << "1. Module Minh Anh: Loc sinh vien theo khoang GPA   \n";
    cout << "2. Module My Tra  : Loc sinh vien theo Lop          \n";
    cout << "3. Module Phat    : Quan ly Them - Sua - Xoa (CRUD)  \n";
    cout << "0. Thoat chuong trinh                              \n";
    cout << "----------------------------------------------------\n";
    cout << "Chon chuc nang [0-3]: ";
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

    // ========================================================================
    // KHỞI TẠO CÁC MODULE DÙNG CHUNG MẢNG SINH VIÊN
    // ========================================================================
    FindStudentByGpaRange gpaFilter(students);  // Module Minh Anh
    FindStudentByClassId classFilter(students); // Module Mỹ Trà
    StudentCRUD studentCrud(students);          // Module Phát

    int choice = -1;
    while (choice != 0)
    {
        showMainMenu();
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            clearScreen();
            cout << "[Loi] Vui long nhap so tu 0 den 3.\n\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            clearScreen();
            // Chạy module Lọc theo khoảng GPA (Minh Anh)
            gpaFilter.filterBaseline();
            pauseScreen();
            clearScreen();
            break;

        case 2:
            clearScreen();
            // Chạy module Lọc theo lớp (Mỹ Trà)
            classFilter.filterBaseline();
            pauseScreen();
            clearScreen();
            break;

        case 3:
            clearScreen();
            // Chạy module Thêm - Sửa - Xóa (Phát)
            studentCrud.runCRUDMenu();
            clearScreen();
            break;

        case 0:
            clearScreen();
            cout << "\nCam on ban da su dung chuong trinh!\n";
            break;

        default:
            clearScreen();
            cout << "[Loi] Lua chon khong hop le. Vui long chon lai!\n\n";
            break;
        }
    }

    return 0;
}