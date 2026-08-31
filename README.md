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
│   └── StudentDatabase.cpp      # Xử lý đọc file JSON và hiển thị danh sách
│
├── nlohmann/                     # Thư viện ngoài xử lý JSON cho C++
│   └── json.hpp
│
├── main.cpp                      # File chương trình chính
├── run.bat                       # Script biên dịch và chạy ứng dụng 1-click
├── README.md                     # Tài liệu hướng dẫn dự án
└── main.exe                      # File thực thi chương trình
```

---

## ⚡ Hướng Dẫn Chạy Chương Trình

Dự án sử dụng file script **`run.bat`** để tự động tìm và biên dịch tất cả các file `.cpp` trong thư mục `src/`. Bạn có 2 cách chạy vô cùng đơn giản:

### Cách 1: Nhấp đúp chuột (Khuyên dùng)
* Mở thư mục dự án trên Windows và **nhấp đúp chuột vào file `run.bat`**.
* Chương trình sẽ tự động biên dịch và hiển thị kết quả ngay lập tức!

### Cách 2: Chạy từ Terminal / PowerShell
Mở Terminal tại thư mục dự án và gõ:

```bash
.\run.bat
```

---

## ⚙️ Yêu Cầu Môi Trường (Cho Đồng Đội)

Đồng đội chỉ cần cài đặt trình biên dịch C++ (`g++`) cơ bản trên Windows (Ví dụ từ **[WinLibs](https://winlibs.com/)** hoặc **MinGW-w64**) và thêm đường dẫn `bin` vào biến môi trường `PATH`. 

Sau khi có `g++`, chỉ cần chạy `run.bat` là xong!
