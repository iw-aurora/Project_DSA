#include "interface/student.h"
#include "interface/MC1_Search.h"
#include "interface/interface_mytra/FindStudentByClassId.h"
// #include "interface/interface_minhanh/FindStudentByGpaRange.h" // [Tam khoa module Minh Anh]
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;
using json = nlohmann::json;

// Ham rieng nap du lieu tu file JSON va tra ve mang dynamic array (vector<Student>)
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

void showMenu() {
    cout << "====================================================\n";
    cout << "      HE THONG QUAN LY SINH VIEN - DASA230179\n";
    cout << "====================================================\n";
    cout << "1. Hien thi so luong sinh vien trong RAM\n";
    cout << "2. MC1: Tra cuu theo MSSV (Linear Search vs Hash Index)\n";
    cout << "3. Chay thu nghiem MC1 mau (Dau, Giua, Cuoi danh sach)\n";
    cout << "4. Module My Tra: Loc sinh vien theo Lop (FindStudentByClassId)\n";
    cout << "0. Thoat chuong trinh\n";
    cout << "----------------------------------------------------\n";
    cout << "Chon chuc nang [0-4]: ";
}

int main()
{
    // ========================================================================
    // BUOC NAP DU LIEU DUNG CHUNG CHO TOAN DU AN (SHARED DATA LOADING)
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

    // Khoi tao cac module
    MC1_Search mc1(students);
    FindStudentByClassId classFilter(students);

    int choice = -1;
    while (choice != 0) {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Loi] Vui long nhap so tu 0 den 4.\n\n";
            continue;
        }

        switch (choice) {
            case 1:
                cout << "\n--- THONG TIN SINH VIEN TRONG RAM ---\n";
                cout << "Tong so sinh vien: " << students.size() << "\n\n";
                break;

            case 2: {
                string targetId;
                cout << "\nNhap MSSV can tra cuu (Vi du: 25150050): ";
                cin >> targetId;
                mc1.runBenchmark(targetId);
                break;
            }

            case 3:
                cout << "\n=== TU DONG CHAY BENCHMARK MC1 MAU ===\n";
                cout << "\n>>> TEST 1: MSSV O DAU DANH SACH (25150001)\n";
                mc1.runBenchmark("25150001");

                cout << "\n>>> TEST 2: MSSV O GIUA DANH SACH (25150050)\n";
                mc1.runBenchmark("25150050");

                cout << "\n>>> TEST 3: MSSV O CUOI DANH SACH (25150100)\n";
                mc1.runBenchmark("25150100");

                cout << "\n>>> TEST 4: MSSV KHONG TON TAI (99999999)\n";
                mc1.runBenchmark("99999999");
                break;

            case 4:
                cout << "\n--- CHAY MODULE LOC THEO LOP (MY TRA) ---\n";
                classFilter.filterBaseline();
                break;

            case 0:
                cout << "\nCam on ban da su dung chuong trinh!\n";
                break;

            default:
                cout << "\n[Loi] Lua chon khong hop le.\n\n";
                break;
        }
    }

    return 0;
}