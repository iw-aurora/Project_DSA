#include "interface/StudentDatabase.h"
#include <iostream>

using namespace std;

int main() {
    StudentDatabase database;

    // Nạp dữ liệu từ file JSON vào bộ nhớ RAM
    if (!database.loadFromJson("data/database.json")) {
        cout << "[Loi] Khong the nap du lieu sinh vien.\n";
        return 1;
    }

    // Hiển thị dữ liệu từ RAM ra màn hình
    database.displayStudents();

    return 0;
}