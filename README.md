# Dự Án Quản Lý Sinh Viên (Student Database System)

Chương trình đọc và hiển thị danh sách sinh viên từ file **JSON** viết bằng ngôn ngữ **C++**, sử dụng thư viện `nlohmann/json` và tự động quản lý biên dịch bằng **Makefile**.

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
├── Makefile                      # File cấu hình tự động biên dịch dự án
├── README.md                     # Tài liệu hướng dẫn dự án
└── main.exe                      # File thực thi sau khi biên dịch
```

---

## 🛠️ Trách Nhiệm Của Từng Thư Mục & File

1. **`interface/StudentDatabase.h`**: Khai báo phương thức `displayStudentsFromJson(const string& filename);`.
2. **`src/StudentDatabase.cpp`**: Đọc file JSON, dịch dữ liệu về mảng `vector<Student>` và hiển thị ra màn hình console.
3. **`main.cpp`**: Gọi phương thức `displayStudentsFromJson("data/database.json")`.
4. **`Makefile`**: Tự động gom tất cả các file nguồn (`main.cpp`, `src/StudentDatabase.cpp`) để biên dịch chỉ với 1 lệnh đơn giản.

---

## 🚀 Hướng Dẫn Biên Dịch & Chạy Chương Trình

### 1. Biên dịch dự án bằng Makefile (Windows MinGW)
Trên Windows, câu lệnh `make` có tên đầy đủ trong MinGW là **`mingw32-make`**:

```bash
mingw32-make
```

*(Nếu muốn gõ tắt là `make` trên PowerShell, chạy lệnh: `Set-Alias make mingw32-make`)*

### 2. Chạy chương trình
```bash
.\main.exe
```

### 3. Dọn dẹp file thực thi (Clean)
```bash
mingw32-make clean
```
