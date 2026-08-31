# Dự Án Quản Lý Sinh Viên (Student Database System)

Chương trình đọc và hiển thị danh sách sinh viên từ file **JSON** viết bằng ngôn ngữ **C++**, sử dụng thư viện `nlohmann/json`.

---

## 📁 Cấu Trúc Dự Án (Project Structure)

Dự án được tổ chức theo chuẩn phân tách giao diện khai báo (Interfaces/Headers) và cài đặt logic (Implementation/Src):

```text
trang/
├── data/                         # Thư mục chứa dữ liệu
│   └── database.json            # File dữ liệu sinh viên JSON
│
├── interface/                    # Thư mục chứa các file khai báo Header (.h)
│   ├── student.h                # Khai báo cấu trúc Student
│   └── StudentDatabase.h        # Khai báo phương thức displayStudentsFromJson
│
├── src/                          # Thư mục chứa các file cài đặt nguồn (.cpp)
│   └── StudentDatabase.cpp      # Cài đặt chi tiết phương thức đọc và hiển thị
│
├── nlohmann/                     # Thư viện ngoài xử lý JSON cho C++
│   └── json.hpp
│
├── main.cpp                      # File chương trình chính
├── README.md                     # Tài liệu hướng dẫn dự án
└── main.exe                      # File thực thi sau khi biên dịch
```

---

## 🛠️ Trách Nhiệm Của Từng Thư Mục & File

1. **`interface/StudentDatabase.h`**: Khai báo duy nhất 1 phương thức:
   `void displayStudentsFromJson(const std::string& filename);`
2. **`src/StudentDatabase.cpp`**: Đọc file JSON, ép kiểu dữ liệu về danh sách `Student` và hiển thị danh sách sinh viên ra màn hình console.
3. **`main.cpp`**: Khởi tạo và gọi hàm `displayStudentsFromJson("data/database.json")`.

---

## 🚀 Hướng Dẫn Biên Dịch & Chạy Chương Trình

```bash
# Biên dịch
g++ main.cpp src/StudentDatabase.cpp -o main.exe

# Chạy chương trình
.\main.exe
```
