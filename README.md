# Dự Án Quản Lý Sinh Viên (Student Database System)

Chương trình đọc và hiển thị danh sách sinh viên từ file **JSON** viết bằng ngôn ngữ **C++**, sử dụng thư viện `nlohmann/json`.

---

## 📁 Cấu Trúc Dự Án (Project Structure)

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
├── run.bat                       # File script chạy nhanh dự án 1-click
├── README.md                     # Tài liệu hướng dẫn dự án
└── main.exe                      # File thực thi sau khi biên dịch
```

---

## ⚡ Hướng Dẫn Chạy Chương Trình Siêu Đơn Giản

Không cần nhớ câu lệnh phức tạp, bạn có 2 cách chạy siêu nhanh:

### Cách 1: Nhấp đúp chuột (Siêu dễ - Khuyên dùng)
* Mở thư mục dự án trên Windows và **nhấp đúp chuột vào file `run.bat`**.
* File script sẽ **tự động biên dịch** và **tự động chạy chương trình** cho bạn!

### Cách 2: Chạy từ Terminal / PowerShell
Mở Terminal tại thư mục dự án và gõ:

```bash
.\run.bat
```

---

## ⚙️ Yêu Cầu Môi Trường (Cho Đồng Đội Mới)

Đồng đội chỉ cần cài đặt trình biên dịch C++ (`g++`) cơ bản trên Windows (Ví dụ từ **[WinLibs](https://winlibs.com/)** hoặc **MinGW-w64**) và thêm vào biến môi trường `PATH`. 

Sau khi có `g++`, chỉ cần chạy file `run.bat` là xong!
