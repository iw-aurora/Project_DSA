#include "interface/StudentDatabase.h"
#include "interface/MC1_Search.h"
#include <iostream>
#include <string>

using namespace std;

void showMenu() {
    cout << "====================================================\n";
    cout << "      HE THONG QUAN LY SINH VIEN - DASA230179\n";
    cout << "====================================================\n";
    cout << "1. Hien thi danh sach sinh vien trong RAM\n";
    cout << "2. MC1: Tra cuu theo MSSV (Linear Search vs Hash Index)\n";
    cout << "3. Chay thu nghiem MC1 mau (Dau, Giua, Cuoi danh sach)\n";
    cout << "0. Thoat chuong trinh\n";
    cout << "----------------------------------------------------\n";
    cout << "Chon chuc nang [0-3]: ";
}

int main() {
    StudentDatabase database;

    // Nạp dữ liệu vào RAM
    if (!database.loadFromJson("data/database.json")) {
        cout << "[Loi] Khong the nap du lieu sinh vien.\n";
        return 1;
    }

    // Khởi tạo Module MC1 với dữ liệu trong RAM
    MC1_Search mc1(database.getStudents());

    int choice = -1;
    while (choice != 0) {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Loi] Vui long nhap so tu 0 den 3.\n\n";
            continue;
        }

        switch (choice) {
            case 1:
                cout << "\n--- DANH SACH SINH VIEN TRONG RAM ---\n";
                database.displayStudents();
                cout << "\n";
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