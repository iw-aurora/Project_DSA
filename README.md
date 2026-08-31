# Dự Án Quản Lý Sinh Viên (Student Database System)

Chương trình đọc và hiển thị danh sách sinh viên từ file **JSON** viết bằng ngôn ngữ **C++**, sử dụng thư viện `nlohmann/json` và quản lý biên dịch bằng **Makefile**.

---

## 📁 Cấu Trúc Dự Án (Project Structure)

```text
trang/
├── data/                         # Thư mục chứa dữ liệu đầu vào
│   └── database.json            # File dữ liệu sinh viên định dạng JSON
│
├── interface/                    # Thư mục chứa các file khai báo Header (.h)
│   ├── student.h                # Định nghĩa cấu trúc Student
│   └── StudentDatabase.h        # Khai báo lớp & phương thức displayStudentsFromJson
│
├── src/                          # Thư mục chứa các file cài đặt nguồn (.cpp)
│   └── StudentDatabase.cpp      # Xử lý đọc file JSON và hiển thị danh sách
│
├── nlohmann/                     # Thư viện JSON cho C++ (Header-only)
│   └── json.hpp
│
├── main.cpp                      # File chính khởi chạy ứng dụng
├── Makefile                      # File cấu hình tự động biên dịch
├── README.md                     # Tài liệu hướng dẫn dự án
└── main.exe                      # File thực thi chương trình
```

---

## 🛠️ Hướng Dẫn Cài Đặt Môi Trường Cho Đội Nhóm (Windows Setup Guide)

Dành cho các thành viên trong nhóm chưa có trình biên dịch C++ (`g++`) và công cụ Make trên máy:

### Bước 1: Tải trình biên dịch MinGW-w64 (GCC + Make)
1. Truy cập trang web chính thức: **[WinLibs (winlibs.com)](https://winlibs.com/)**
2. Tại mục **GCC release**, tải bản: **Win64 (64-bit)** ➡️ Chọn gói zip (ví dụ: `Release ... UCRT ... zip`).
3. Giải nén file vừa tải về vào ổ `C:\` (Đường dẫn sau khi giải nén sẽ là `C:\mingw64`).

### Bước 2: Thêm MinGW vào biến môi trường (PATH) của Windows
1. Nhấn phím `Windows` ➡️ Tìm từ khóa **Environment Variables** (Hoặc *Edit the system environment variables*).
2. Nhấp vào nút **Environment Variables...** ở góc dưới.
3. Trong ô **System variables** (hoặc **User variables**), tìm dòng **`Path`** và nhấp **Edit**.
4. Nhấn nút **New** ➡️ Nhập đường dẫn: `C:\mingw64\bin`
5. Nhấn **OK** ở tất cả các cửa sổ để lưu cài đặt.

### Bước 3: Kiểm tra cài đặt trong Terminal
Mở cửa sổ **PowerShell** hoặc **CMD** mới và kiểm tra:

```bash
g++ --version
mingw32-make --version
```
*(Nếu màn hình in ra thông tin phiên bản là bạn đã cài đặt thành công!)*

---

## 🚀 Hướng Dẫn Biên Dịch & Chạy Chương Trình

### 1. Tạo Alias lệnh `make` (Tuỳ chọn cho PowerShell)
Mặc định trên Windows, công cụ Make có tên là `mingw32-make`. Để gõ nhanh chữ `make`, bạn chạy lệnh này 1 lần trong PowerShell:

```powershell
Set-Alias make mingw32-make
```

### 2. Biên dịch dự án
Mở Terminal tại thư mục dự án và chạy:

```bash
make
```
*(Hoặc gõ `mingw32-make` nếu chưa cài Alias)*

### 3. Chạy chương trình
```bash
.\main.exe
```

### 4. Dọn dẹp file thực thi (Clean build)
```bash
mingw32-make clean
```
