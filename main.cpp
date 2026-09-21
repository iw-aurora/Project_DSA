// ============================================================================
// CHƯƠNG TRÌNH CHÍNH - HỆ THỐNG QUẢN LÝ SINH VIÊN (DASA230179)
// ============================================================================

#include "interface/interface_minhanh/FindStudentByGpaRange.h" // [Module Minh Anh: Lọc theo GPA]
#include "interface/interface_mytra/FindStudentByClassId.h"   // [Module Mỹ Trà: Lọc theo lớp]
#include "interface/interface_phat/StudentCRUD.h"             // [Module Phát: Thêm - Sửa - Xóa]
#include "interface/interface_tra/FindStudentByMaxGpa.h"      // [Module Thanh Tra: Tìm GPA cao nhất]
#include "interface/interface_trang/FindStudentById.h"        // [Module Trang: Tìm kiếm MSSV tương tác]
#include "interface/interface_trang/Benchmark.h"              // [Module Trang: Benchmark tìm kiếm MSSV]
#include "interface/interface_trang/HashTable.h"              // [Module Trang: Hash Search]
#include "interface/interface_trang/LinearSearch.h"           // [Module Trang: Linear Search]
#include "interface/student.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include <iomanip>

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

// ============================================================================
// MENU CHÍNH TƯƠNG TÁC BẰNG PHÍM MŨI TÊN (ARROW KEYS MAIN MENU)
// ============================================================================
int selectMainMenuInteractive()
{
    const vector<string> menuOptions = {
        "Module 1: Loc sinh vien theo khoang GPA (Minh Anh)",
        "Module 2: Loc sinh vien theo Lop        (My Tra)",
        "Module 3: Quan ly Them - Sua - Xoa CRUD (Phat)",
        "Module 4: Tim kiem sinh vien theo MSSV  (Trang)",
        "Module 5: Tim sinh vien co GPA cao nhat (Thanh Tra)",
        "Thoat chuong trinh"
    };

    int currentIndex = 0;
    int totalOptions = static_cast<int>(menuOptions.size());

    while (true)
    {
        clearScreen();
        cout << "=========================================================================================\n";
        cout << "                  HE THONG QUAN LY SINH VIEN - DASA230179                                \n";
        cout << "=========================================================================================\n";
        cout << " [HUONG DAN]: Dung phim Mui ten Len/Xuong de chon, Enter de thuc thi, Esc de thoat      \n";
        cout << "-----------------------------------------------------------------------------------------\n";

        for (int i = 0; i < totalOptions; i++)
        {
            if (i == currentIndex)
            {
                cout << "  -->  [ " << (i == totalOptions - 1 ? 0 : i + 1) << " ]  "
                     << left << setw(60) << menuOptions[i]
                     << "  <== [DANG CHON]\n";
            }
            else
            {
                cout << "       [ " << (i == totalOptions - 1 ? 0 : i + 1) << " ]  "
                     << left << setw(60) << menuOptions[i] << "\n";
            }
        }

        cout << "=========================================================================================\n";

#ifdef _WIN32
        int ch = _getch();
        if (ch == 0 || ch == 224) // Phím mũi tên
        {
            int arrow = _getch();
            if (arrow == 72) // Mũi tên LÊN (UP)
            {
                if (currentIndex > 0)
                    currentIndex--;
                else
                    currentIndex = totalOptions - 1;
            }
            else if (arrow == 80) // Mũi tên XUỐNG (DOWN)
            {
                if (currentIndex < totalOptions - 1)
                    currentIndex++;
                else
                    currentIndex = 0;
            }
        }
        else if (ch == 13) // Phím ENTER
        {
            if (currentIndex == totalOptions - 1)
                return 0; // Thoát
            return currentIndex + 1;
        }
        else if (ch == 27 || ch == '0') // Phím ESC hoặc 0
        {
            return 0; // Thoát
        }
        else if (ch >= '1' && ch <= '5') // Phím tắt số 1 - 5
        {
            return ch - '0';
        }
#else
        cout << "Chon chuc nang (0-5): ";
        int choice;
        if (cin >> choice)
            return choice;
        return 0;
#endif
    }
}

int main()
{
    vector<Student> students;
    try
    {
        students = loadStudentsData("data/database.json");
    }
    catch (const exception &e)
    {
        cerr << "[Loi] Khong the nap du lieu: " << e.what() << '\n';
        return 1;
    }

    FindStudentByGpaRange gpaFilter(students);  // Module Minh Anh
    FindStudentByClassId classFilter(students); // Module Mỹ Trà
    StudentCRUD studentCrud(students);          // Module Phát
    FindStudentById studentFinder(students);    // Module Trang
    FindStudentByMaxGpa maxGpaFinder(students); // Module Thanh Tra

    while (true)
    {
        int choice = selectMainMenuInteractive();

        if (choice == 0)
        {
            clearScreen();
            cout << "\n=========================================================================================\n";
            cout << "                 CAM ON BAN DA SU DUNG HE THONG QUAN LY SINH VIEN!                       \n";
            cout << "=========================================================================================\n\n";
            break;
        }

        switch (choice)
        {
        case 1:
            clearScreen();
            gpaFilter.runComparison();
            pauseScreen();
            break;

        case 2:
            clearScreen();
            classFilter.filterBaseline();
            pauseScreen();
            break;

        case 3:
            clearScreen();
            studentCrud.runCRUDMenu();
            break;

        case 4:
            clearScreen();
            studentFinder.runInteractiveSearch();
            pauseScreen();
            break;

        case 5:
            clearScreen();
            maxGpaFinder.runCompleteBenchmarkSuite();
            pauseScreen();
            break;

        default:
            break;
        }
    }

    return 0;
}
