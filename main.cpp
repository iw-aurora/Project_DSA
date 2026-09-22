// ============================================================================
// CHUONG TRINH CHINH - HE THONG QUAN LY SINH VIEN (DASA230179)
// ============================================================================

#include "interface/interface_minhanh/FindStudentByGpaRange.h"
#include "interface/interface_mytra/FindStudentByClassId.h"
#include "interface/interface_phat/StudentCRUD.h"
#include "interface/interface_trang/FindStudentById.h"
#include "interface/interface_tra/FindStudentByMaxGpa.h"
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
        throw runtime_error("File JSON khong dung dinh dang (thieu mang students).");
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

static int selectMenuInteractive(const string &title, const vector<string> &options)
{
    int currentIndex = 0;
    int totalOptions = static_cast<int>(options.size());

    while (true)
    {
        clearScreen();
        cout << "=========================================================================================\n";
        cout << "                  " << title << "\n";
        cout << "=========================================================================================\n";
        cout << " [HUONG DAN]: Dung phim Mui ten Len/Xuong de chon, Enter de thuc thi, Esc de thoat      \n";
        cout << "-----------------------------------------------------------------------------------------\n";

        for (int i = 0; i < totalOptions; i++)
        {
            int displayNum = (i == totalOptions - 1) ? 0 : (i + 1);
            if (i == currentIndex)
            {
                cout << "  -->  [ " << displayNum << " ]  "
                     << left << setw(65) << options[i]
                     << "  <== [DANG CHON]\n";
            }
            else
            {
                cout << "       [ " << displayNum << " ]  "
                     << left << setw(65) << options[i] << "\n";
            }
        }

        cout << "=========================================================================================\n";

#ifdef _WIN32
        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            int arrow = _getch();
            if (arrow == 72)
            {
                if (currentIndex > 0)
                    currentIndex--;
                else
                    currentIndex = totalOptions - 1;
            }
            else if (arrow == 80)
            {
                if (currentIndex < totalOptions - 1)
                    currentIndex++;
                else
                    currentIndex = 0;
            }
        }
        else if (ch == 13)
        {
            if (currentIndex == totalOptions - 1)
                return 0;
            return currentIndex + 1;
        }
        else if (ch == 27 || ch == '0')
        {
            return 0;
        }
        else if (ch >= '1' && ch <= '0' + totalOptions - 1)
        {
            return ch - '0';
        }
#else
        cout << "Chon chuc nang: ";
        int choice;
        if (cin >> choice)
            return choice;
        return 0;
#endif
    }
}

int selectMainMenuInteractive()
{
    const vector<string> menuOptions = {
        "MODE 1: SO SANH THUAT TOAN (BENCHMARK SUITE)",
        "MODE 2: GIAI THUAT TOI UU   (FINAL SOLUTION / PRODUCTION)",
        "MODE 3: HE THONG QUAN LY SINH VIEN (CRUD)",
        "Thoat chuong trinh"
    };

    return selectMenuInteractive("HE THONG QUAN LY SINH VIEN - DASA230179", menuOptions);
}

int selectBenchmarkMenuInteractive()
{
    const vector<string> benchmarkOptions = {
        "Module 1: So sanh Loc GPA      (Linear Scan vs Binary Search)",
        "Module 2: So sanh Loc theo Lop (Linear Filter vs Optimized Index)",
        "Module 4: So sanh Tim MSSV     (Linear Search vs Hash Table)",
        "Module 5: So sanh Tim GPA Max  (Linear Max Scan vs Custom Max Heap)",
        "Quay lai Menu Chinh"
    };

    return selectMenuInteractive("MODE 1: SO SANH THUAT TOAN (BENCHMARK SUITE)", benchmarkOptions);
}

int selectFinalSolutionMenuInteractive()
{
    const vector<string> finalOptions = {
        "Module 1: Loc sinh vien theo khoang GPA (Sorted + Binary Search)",
        "Module 2: Loc sinh vien theo Lop        (Optimized Index Filter)",
        "Module 4: Tim kiem sinh vien theo MSSV  (Hash Table O(1))",
        "Module 5: Tim sinh vien co GPA cao nhat (Custom Max Heap O(1))",
        "Quay lai Menu Chinh"
    };

    return selectMenuInteractive("MODE 2: GIAI THUAT TOI UU (FINAL SOLUTION)", finalOptions);
}

void runBenchmarkSuite(FindStudentByGpaRange &gpaFilter,
                       FindStudentByClassId &classFilter,
                       FindStudentById &studentFinder,
                       FindStudentByMaxGpa &maxGpaFinder)
{
    while (true)
    {
        int choice = selectBenchmarkMenuInteractive();
        if (choice == 0)
            break;

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
            studentFinder.runInteractiveSearch();
            pauseScreen();
            break;

        case 4:
            clearScreen();
            maxGpaFinder.runCompleteBenchmarkSuite();
            pauseScreen();
            break;

        default:
            break;
        }
    }
}

void runFinalSolutionSuite(FindStudentByGpaRange &gpaFilter,
                          FindStudentByClassId &classFilter,
                          FindStudentById &studentFinder,
                          FindStudentByMaxGpa &maxGpaFinder)
{
    while (true)
    {
        int choice = selectFinalSolutionMenuInteractive();
        if (choice == 0)
            break;

        switch (choice)
        {
        case 1:
            clearScreen();
            gpaFilter.runFinalSolution();
            pauseScreen();
            break;

        case 2:
            clearScreen();
            classFilter.filterFinalSolution();
            pauseScreen();
            break;

        case 3:
            clearScreen();
            studentFinder.runFinalSearch();
            pauseScreen();
            break;

        case 4:
            clearScreen();
            maxGpaFinder.runFinalSolution();
            pauseScreen();
            break;

        default:
            break;
        }
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

    FindStudentByGpaRange gpaFilter(students);
    FindStudentByClassId classFilter(students);
    StudentCRUD studentCrud(students);
    FindStudentById studentFinder(students);
    FindStudentByMaxGpa maxGpaFinder(students);

    while (true)
    {
        int modeChoice = selectMainMenuInteractive();

        if (modeChoice == 0)
        {
            clearScreen();
            cout << "\n=========================================================================================\n";
            cout << "                 CAM ON BAN DA SU DUNG HE THONG QUAN LY SINH VIEN!                       \n";
            cout << "=========================================================================================\n\n";
            break;
        }

        switch (modeChoice)
        {
        case 1:
            runBenchmarkSuite(gpaFilter, classFilter, studentFinder, maxGpaFinder);
            break;

        case 2:
            runFinalSolutionSuite(gpaFilter, classFilter, studentFinder, maxGpaFinder);
            break;

        case 3:
            clearScreen();
            studentCrud.runCRUDMenu();
            break;

        default:
            break;
        }
    }

    return 0;
}
