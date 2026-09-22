# 🎓 HỆ THỐNG QUẢN LÝ SINH VIÊN & ĐO LƯỜNG HIỆU NĂNG THUẬT TOÁN (DSA)

> Dự án đồ án môn học **Cấu Trúc Dữ Liệu & Giải Thuật (Data Structures & Algorithms - DASA230179)**.  
> Ứng dụng quản lý, tra cứu và đánh giá đối sánh hiệu năng thuật toán tối ưu trên tập dữ liệu lớn (**500,000+ sinh viên**) với 2 nền tảng: **Console CLI C++** và **Giao diện Web Dashboard (Next.js + C++ Engine)**.

---

## 🌟 Tính Năng & Các Module Thuật Toán

| Module | Tên Chức Năng | Giải Thuật Gốc (Baseline) | Giải Thuật Tối Ưu (Final Solution) | Độ Phức Tạp Tối Ưu |
| :--- | :--- | :--- | :--- | :--- |
| **MC1** *(Bắt buộc)* | Tra cứu theo Mã Sinh Viên (MSSV) | Duyệt tuyến tính (Linear Scan) | **Hash Table** (`std::unordered_map`) | **$\mathcal{O}(1)$** |
| **MC2** *(Bắt buộc)* | Tìm sinh viên có GPA cao nhất | Quét toàn bộ danh sách | **Custom Max-Heap** (Phần tử gốc) | **$\mathcal{O}(1)$** |
| **RQ1** *(Tự chọn)* | Lọc danh sách sinh viên theo Lớp | Quét từng phần tử mảng | **Chỉ mục bộ nhớ (Inverted Index Filter)** | **$\mathcal{O}(K)$** |
| **RQ2** *(Tự chọn)* | Lọc sinh viên theo khoảng GPA | Quét tuyến tính $O(N)$ | **Mảng sắp xếp + Tìm kiếm nhị phân (Binary Search)** | **$\mathcal{O}(\log N + K)$** |
| **CRUD** | Thêm, sửa, xóa, tìm kiếm sinh viên | Thao tác trên bộ nhớ | **Cập nhật đồng bộ RAM & Database JSON** | $\mathcal{O}(1)$ / $\mathcal{O}(N)$ |
| **RAM Injector** | Nạp & cộng dồn dữ liệu lớn vào RAM | N/A | **In-Memory High-Speed Generator (500k records trong vài ms)** | Instant |

---

## 📁 Cấu Trúc Thư Mục Dự Án (Project Structure)

```text
code_dsa/
├── data/
│   └── database.json                 # File cơ sở dữ liệu gốc (500,000 sinh viên)
│
├── interface/                        # Thư mục chứa các Header khai báo (.h)
│   ├── student.h                     # Cấu trúc dữ liệu Sinh viên (Student)
│   ├── StudentDatabase.h             # Đọc / Ghi file JSON
│   ├── FindStudentById.h             # Module MC1 (Tra cứu theo MSSV)
│   ├── FindStudentByMaxGpa.h         # Module MC2 (Sinh viên GPA cao nhất)
│   ├── FindStudentByClassId.h        # Module RQ1 (Lọc theo Mã Lớp)
│   ├── FindStudentByGpaRange.h       # Module RQ2 (Lọc khoảng điểm GPA)
│   ├── StudentCRUD.h                 # Module CRUD (Thêm, sửa, xóa)
│   └── TerminalUtils.h               # Tiện ích giao diện Console tương tác ANSI
│
├── src/                              # Cài đặt mã nguồn C++ của từng thành viên
│   ├── core_minhanh/                 # Cài đặt module MC1
│   ├── core_mytra/                   # Cài đặt module MC2
│   ├── core_phat/                    # Cài đặt module RQ1
│   ├── core_trang/                   # Cài đặt module RQ2
│   ├── core_tra/                     # Cài đặt module CRUD
│   └── *.cpp                         # Các file triển khai hệ thống
│
├── web_dsa/                          # 🌐 Ứng dụng Web Dashboard Hiện Đại
│   ├── src/
│   │   ├── app/                      # Next.js 16 App Router (page.tsx, layout.tsx)
│   │   ├── components/               # UI Components (BenchmarkTab, FinalSolutionTab, CrudTab, v.v.)
│   │   └── types.ts                  # TypeScript Interfaces
│   ├── package.json
│   └── next.config.mjs
│
├── dsa_bridge.cpp                    # C++ High-Performance Engine API Bridge
├── server.js                         # Node.js Express REST API Server (Port 5000)
├── main.cpp                          # Ứng dụng Console C++ chính
├── start_web.bat                     # 🚀 1-Click khởi chạy toàn bộ Web Dashboard
├── run.bat                           # 🚀 1-Click biên dịch & chạy ứng dụng Console C++
└── README.md                         # Tài liệu hướng dẫn sử dụng
```

---

## 🛠️ Yêu Cầu Môi Trường (Prerequisites)

1. **Trình biên dịch C++ (`g++`)**: Hỗ trợ chuẩn **C++17** hoặc **C++20** (Cài đặt qua [WinLibs](https://winlibs.com/) hoặc MinGW-w64) và đã được thêm vào biến môi trường `PATH`.
2. **Node.js**: Phiên bản **>= 18.x** (tải tại [nodejs.org](https://nodejs.org/)).

---

## 🚀 Hướng Dẫn Chạy Ứng Dụng

### Cách 1: Chạy Giao Diện Web Dashboard (Khuyên dùng)

Giao diện trực quan, bảng so sánh đồ thị benchmark chi tiết (tốc độ chạy 1,000 workloads đo bằng microsecond / nanosecond), thực thi thuật toán $O(1)$ và quản lý CRUD đầy đủ.

#### Bước 1: Cài đặt thư viện (Chỉ thực hiện lần đầu)
Mở Terminal tại thư mục gốc `code_dsa`:
```bash
# Cài đặt thư viện cho Backend API
npm install

# Cài đặt thư viện cho Frontend Web
cd web_dsa
npm install
cd ..
```

#### Bước 2: Khởi động hệ thống (1-Click)
* **Cách A (Nhanh nhất)**: Nhấp đúp chuột vào file **`start_web.bat`**.
* **Cách B (Terminal)**: Chạy lệnh:
  ```bash
  .\start_web.bat
  ```

File script sẽ tự động biên dịch `dsa_bridge.cpp`, bật Backend Server (`localhost:5000`) và mở giao diện Web tại:
👉 **`http://localhost:3000`**

---

### Cách 2: Chạy Ứng Dụng Console CLI (C++)

Giao diện dòng lệnh tương tác trực tiếp với menu điều hướng phím mũi tên:

* **Cách A (Nhanh nhất)**: Nhấp đúp chuột vào file **`run.bat`**.
* **Cách B (Terminal)**: Chạy lệnh:
  ```bash
  .\run.bat
  ```
* **Cách C (Biên dịch thủ công bằng lệnh `g++`)**:
  ```bash
  g++ -O2 -std=c++17 main.cpp src/*.cpp src/core_minhanh/*.cpp src/core_mytra/*.cpp src/core_phat/*.cpp src/core_trang/*.cpp src/core_tra/*.cpp -o main.exe
  .\main.exe
  ```

---

## 📊 Các Chế Độ Hoạt Động

### 1. Mode 1: Benchmark Suite (So Sánh Hiệu Năng)
* Chạy kiểm thử **1,000 workload liên tiếp** để đo đạc thời gian thực thi (Microseconds / Nanoseconds).
* Đo lường thời gian trung bình, thời gian ngắn nhất, dài nhất và số lần truy cập bộ nhớ / so sánh dữ liệu.
* So sánh tỷ lệ tăng tốc (**Speedup Factor** từ **5,000x đến 50,000x** khi dùng cấu trúc dữ liệu tối ưu).

### 2. Mode 2: Final Solution (Giải Thuật Tối Ưu Tức Thì)
* Truy vấn trực tiếp với thuật toán tối ưu $O(1)$ và $O(\log N + K)$.
* Thời gian phản hồi **~ 0.00 ms**.

### 3. Mode 3: Quản Lý Sinh Viên (CRUD)
* Thêm mới sinh viên, tìm kiếm linh hoạt theo mã/tên/lớp, chỉnh sửa thông tin điểm số và xóa sinh viên.
* Đồng bộ hóa với file `database.json`.

---

## 👥 Thành Viên Nhóm Thực Hiện
- **Nhóm 15** - Đồ Án Môn Học Cấu Trúc Dữ Liệu & Giải Thuật (DSA)
- **Công nghệ sử dụng**: C++20, Next.js 16 (Turbopack), React 19, Ant Design, Tailwind CSS, Express.js.
