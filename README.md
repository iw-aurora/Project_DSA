# 🎓 HỆ THỐNG QUẢN LÝ HỒ SƠ SINH VIÊN & ENGINE ĐỐI SÁNH HIỆU NĂNG THUẬT TOÁN (DSA)
### 🏛️ RECORDS-AND-DECISION ENGINE — C++ IN-MEMORY CORE & WEB DASHBOARD

> **Học phần:** Cấu trúc Dữ liệu và Giải thuật (Data Structures & Algorithms — DASA230179_03)  
> **Trường:** Đại học Sư phạm Kỹ thuật TP. Hồ Chí Minh (HCMUTE)  
> **Giảng viên hướng dẫn:** ThS. Bảo Vũ Đình (T-Bao)  
> **Nhóm thực hiện:** Nhóm 12  
> **Quy mô dữ liệu thực nghiệm:** Thiết kế và tối ưu đáp ứng tức thì từ **500,000** đến **10,000,000+ sinh viên**.

---

## 👥 Danh Sách Thành Viên & Phân Công Nhiệm Vụ

| STT | Họ và tên | MSSV | Vai trò & Thành phần kỹ thuật phụ trách | Cấu trúc dữ liệu tối ưu |
| :---: | :--- | :---: | :--- | :--- |
| 1 | **Bùi Tấn Phát** | **25110289** | **Trưởng nhóm** — Phụ trách yêu cầu **MC1** (Tra cứu hồ sơ theo MSSV) | **Closed Hash Table** (FNV-1a 64-bit, Linear Probing, $\alpha \approx 0.50$) $\to \mathcal{O}(1)$ |
| 2 | **Nguyễn Phan Thành Tiến** | **25110361** | Thành viên — Phụ trách yêu cầu **MC2** (Truy xuất sinh viên có GPA cao nhất) | **Custom Binary Max-Heap** (Floyd Build Heap $\mathcal{O}(N)$, Tie-break theo MSSV) $\to \mathcal{O}(1)$ |
| 3 | **Ngô Đức Thành** | **25110335** | Thành viên — Phụ trách yêu cầu tự phát hiện **RQ1** (Lọc sinh viên theo Mã Lớp) | **Class Index View** (`unordered_map<string, vector<int>>`) $\to \mathcal{O}(1)$ lookup / $\mathcal{O}(1)$ view |
| 4 | **Đặng Đoàn Minh Khang** | **25110227** | Thành viên — Phụ trách yêu cầu tự phát hiện **RQ2** (Lọc theo khoảng GPA), Thiết kế Kiến trúc 3 tầng & API Bridge | **Sorted Pointer Array + Binary Search Range View** $\to \mathcal{O}(\log N)$ view |

---

## 🌟 Tổng Quan Các Module Thuật Toán & Độ Phức Tạp

| Module | Tên Nghiệp Vụ | Giải Thuật Cơ Sở (Baseline) | Giải Thuật Tối Ưu (Final Solution) | Độ Phức Tạp Lý Thuyết | Tăng Tốc Thực Nghiệm ($N=10\text{M}$) |
| :--- | :--- | :--- | :--- | :---: | :---: |
| **MC1** | Tra cứu hồ sơ theo MSSV | Quét tuyến tính (Linear Scan) | **Closed Hash Table (FNV-1a + Linear Probing)** | $\mathcal{O}(1)$ | **> 1,000,000x** |
| **MC2** | Tìm sinh viên có GPA cao nhất | Quét tuyến tính tìm Max | **Custom Binary Max-Heap (Peek Root)** | $\mathcal{O}(1)$ | **> 200,000x** |
| **RQ1** | Lọc sinh viên theo Mã Lớp | Quét mảng + Sao chép bản ghi | **Class Index View (Zero-copy Index Array)** | $\mathcal{O}(1)$ view / $\mathcal{O}(K)$ duyệt | **> 50,000x** |
| **RQ2** | Lọc sinh viên theo khoảng GPA | Quét kiểm tra điều kiện $O(N)$ | **Mảng con trỏ sắp xếp + Binary Search 2 đầu biên** | $\mathcal{O}(\log N)$ view / $\mathcal{O}(K)$ duyệt | **> 15,000x** |
| **CRUD** | Thêm, sửa, xóa hồ sơ | Thao tác trên bộ nhớ | **Cập nhật đồng bộ RAM & Database JSON** | $\mathcal{O}(1) \to \mathcal{O}(N)$ | Tức thì |

---

## 📁 Cấu Trúc Thư Mục Dự Án (Project Structure)

```text
code_dsa/
├── data/
│   └── database.json                 # Cơ sở dữ liệu JSON mẫu (500,000 bản ghi sinh viên)
│
├── interface/                        # Khai báo Header C++ (.h)
│   ├── student.h                     # Struct Student và định nghĩa bản ghi
│   ├── StudentDatabase.h             # Giao tiếp I/O đọc/ghi database JSON
│   ├── core_hash/                    # Header module MC1 (Bảng băm đóng)
│   ├── core_heap/                    # Header module MC2 (Max-Heap)
│   ├── core_class_filter/            # Header module RQ1 (Lọc mã lớp)
│   ├── core_sorted_gpa/              # Header module RQ2 (Lọc khoảng GPA)
│   └── core_crud/                    # Header module CRUD (Thêm/Sửa/Xóa)
│
├── src/                              # Triển khai mã nguồn C++ (.cpp)
│   ├── StudentDatabase.cpp
│   ├── core_hash/
│   ├── core_heap/
│   ├── core_class_filter/
│   ├── core_sorted_gpa/
│   └── core_crud/
│
├── test/                             # 🧪 BỘ TEST TỰ ĐỘNG (AUTOMATED TEST SUITE)
│   ├── automated_test.cpp            # Mã nguồn kiểm thử C++ 28 Test Cases (100% PASS)
│   └── run_test.bat                  # Script chạy test tiện lợi trong thư mục test
│
├── benchmark/                        # ⚡ CÔNG CỤ BENCHMARK & KẾT QUẢ ĐO ĐẠC
│   ├── bench_core.cpp                # Engine đo đạc hiệu năng C++ độc lập
│   ├── run_benchmark.bat             # Script chạy benchmark 1-click
│   ├── bench_10m_fast.txt            # Báo cáo kết quả đo đạc 10.000.000 sinh viên (Fast)
│   └── bench_10m_strict.txt          # Báo cáo kết quả đo đạc 10.000.000 sinh viên (Strict)
│
├── web_dsa/                          # 🌐 ỨNG DỤNG WEB DASHBOARD (FULLSTACK)
│   ├── src/                          # Frontend Next.js 16 + React 19 + Ant Design + Tailwind CSS
│   │   ├── app/                      # App Router (page.tsx, layout.tsx)
│   │   └── components/               # BenchmarkTab, FinalSolutionTab, CrudTab, v.v.
│   ├── server.js                     # Backend API Express.js giao tiếp Resident C++ Engine (Port 5000)
│   ├── test_speed.js                 # Script kiểm thử API Latency
│   ├── nodemon.json                  # Cấu hình hot-reload cho Backend
│   └── package.json                  # Hợp nhất toàn bộ dependencies của ứng dụng Web
│
├── scripts/                          # 🐍 CÁC SCRIPT PYTHON PHỤ TRỢ
│   └── generate_data.py              # Script tự động sinh N bản ghi sinh viên vào data/database.json
│
├── bao_cao/                          # 📄 BÁO CÁO HỌC PHẦN (LATEX & PDF)
│   ├── main.tex                      # Mã nguồn LaTeX báo cáo chính thức
│   ├── main.pdf                      # Bản báo cáo PDF hoàn chỉnh
│   └── image/                        # Hình ảnh sơ đồ, biểu đồ benchmark
│
├── dsa_bridge.cpp                    # Resident C++ Core Engine kết nối Web qua IPC Stdin/Stdout
├── main.cpp                          # Ứng dụng Console CLI tương tác C++ chính
├── start_web.bat                     # 🚀 1-Click khởi chạy toàn bộ Web App (Backend + Frontend)
├── run.bat                           # 🚀 1-Click biên dịch & chạy Console CLI C++
├── run_tests.bat                     # 🚀 1-Click chạy toàn bộ 28 Ca Test Tự Động
└── README.md                         # Tài liệu hướng dẫn sử dụng chi tiết
```

---

## 🛠️ Yêu Cầu Môi Trường (Prerequisites)

1. **Trình biên dịch C++ (`g++`)**: Hỗ trợ chuẩn **C++17** hoặc **C++20** (MinGW-w64 / GCC) và đã có trong biến môi trường `PATH`.
2. **Node.js**: Phiên bản **>= 18.x** (khuyên dùng LTS, tải tại [nodejs.org](https://nodejs.org/)).
3. **Python 3.x** *(Tùy chọn)*: Dùng khi cần chạy script sinh thêm dữ liệu ngẫu nhiên.

---

## 🚀 Hướng Dẫn Khởi Chạy (Quick Start Guide)

### 1. Chạy Giao Diện Web Dashboard (Khuyên Dùng)
Giao diện trực quan, biểu đồ benchmark thời gian thực, chế độ nạp tức thì 500.000 đến 10.000.000 bản ghi vào RAM với độ trễ phản hồi **~0.00 ms**.

* **Bước 1: Cài đặt thư viện (chỉ cần chạy lần đầu)**:
  ```bash
  cd web_dsa
  npm install
  cd ..
  ```
* **Bước 2: Khởi chạy 1-Click**:
  * **Cách A**: Nhấp đúp chuột vào file **`start_web.bat`**.
  * **Cách B**: Chạy từ terminal:
    ```bash
    .\start_web.bat
    ```
* **Truy cập ứng dụng**:
  * **Frontend Web**: [http://localhost:3000](http://localhost:3000)
  * **Backend API**: [http://localhost:5000](http://localhost:5000)

---

### 2. Chạy Ứng Dụng Console CLI (C++)
Chương trình giao diện dòng lệnh tương tác Menu trực quan bằng bàn phím (phím mũi tên, Enter, Esc).

* **Cách A (1-Click)**: Nhấp đúp chuột vào file **`run.bat`**.
* **Cách B (Terminal)**:
  ```bash
  .\run.bat
  ```
* **Cách C (Biên dịch thủ công bằng `g++`)**:
  ```bash
  g++ -O2 -std=c++17 -I. main.cpp src/*.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp -o main.exe
  .\main.exe
  ```

---

### 3. Chạy Bộ Test Tự Động (Automated Test Suite)
Bộ kiểm thử tự động toàn diện kiểm tra tính đúng đắn logic của cả 6 thành phần hệ thống với 28 test cases độc lập:

* **Cách A (1-Click từ thư mục gốc)**:
  ```bash
  .\run_tests.bat
  ```
* **Cách B (Từ thư mục `test/`)**:
  ```bash
  cd test
  .\run_test.bat
  ```
* **Cách C (Biên dịch trực tiếp bằng `g++`)**:
  ```bash
  g++ -O2 -std=c++17 -I. test/automated_test.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp -o test/automated_test.exe
  .\test\automated_test.exe
  ```

**Kết quả kiểm thử thực tế:** `100% PASS (28/28 Ca kiểm thử thành công)`.

---

### 4. Chạy Đo Lường Hiệu Năng (Benchmark Engine)
Công cụ đo lường chuyên sâu đối sánh Baseline vs Final Solution trên tập dữ liệu lên đến **10,000,000 sinh viên**:

* **Cách A (1-Click)**:
  ```bash
  cd benchmark
  .\run_benchmark.bat
  ```
* **Cách B (Biên dịch thủ công với cờ tối ưu hóa cao nhất `-O3 -flto`)**:
  ```bash
  g++ -O3 -march=native -funroll-loops -flto -DNDEBUG -std=c++17 -I. benchmark/bench_core.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp -o benchmark/bench_core.exe
  .\benchmark\bench_core.exe
  ```
* Xem kết quả chi tiết đã ghi nhận tại:
  - [benchmark/bench_10m_fast.txt](file:///c:/Users/Admin/Documents/Workspace/DSA_15/code_dsa/benchmark/bench_10m_fast.txt)
  - [benchmark/bench_10m_strict.txt](file:///c:/Users/Admin/Documents/Workspace/DSA_15/code_dsa/benchmark/bench_10m_strict.txt)

---

### 5. Sinh Dữ Liệu Tự Động (Data Generator)
Để tạo mới file `data/database.json` với số lượng bản ghi tùy chọn:
```bash
python scripts/generate_data.py
```

---

## 📊 Kết Quả Thực Nghiệm Tiêu Biểu ($N = 10,000,000$ Sinh Viên)

| Phép Thử Nghiệm | Giải Thuật Tuyến Tính (Baseline) | Giải Thuật Tối Ưu (Final Solution) | Tỷ Lệ Cải Thiện Hiệu Năng |
| :--- | :---: | :---: | :---: |
| **MC1 (Tra cứu 1000 MSSV)** | $26,424.51\text{ ms}$ (~26.4 giây) | **$0.024\text{ ms}$** (~24 $\mu\text{s}$) | **Tăng tốc ~ 1,080,000 lần** |
| **MC2 (Truy xuất 100 lần Max GPA)** | $3,819.02\text{ ms}$ (~3.8 giây) | **$0.016\text{ ms}$** (~16 $\mu\text{s}$) | **Tăng tốc ~ 238,000 lần** |
| **RQ1 (Lọc theo Mã Lớp)** | $77.90\text{ ms}$ | **$0.0014\text{ ms}$** (~1.4 $\mu\text{s}$) | **Tăng tốc ~ 55,000 lần** |
| **RQ2 (Lọc khoảng GPA)** | $331.48\text{ ms}$ | **$0.017\text{ ms}$** (~17 $\mu\text{s}$) | **Tăng tốc ~ 19,000 lần** |

---

## 📄 Tài Liệu Báo Cáo
Bản báo cáo học thuật định dạng LaTeX chuẩn chỉ, không công thức rườm rà, bố cục hình ảnh và bảng biểu cân đối được lưu trữ tại:
* **Mã nguồn LaTeX:** [bao_cao/main.tex](file:///c:/Users/Admin/Documents/Workspace/DSA_15/code_dsa/bao_cao/main.tex)
* **Bản PDF xuất bản:** [bao_cao/main.pdf](file:///c:/Users/Admin/Documents/Workspace/DSA_15/code_dsa/bao_cao/main.pdf)

---
*© 2026 - Nhóm 12 | Đồ án Môn học Cấu Trúc Dữ Liệu và Giải Thuật (HCMUTE)*
