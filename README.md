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
│   └── run_tests.bat                 # Script chạy toàn bộ test tự động
│
├── benchmark/                        # ⚡ CÔNG CỤ BENCHMARK & KẾT QUẢ ĐO ĐẠC
│   ├── bench_core.cpp                # Engine đo đạc hiệu năng C++ độc lập
│   ├── run_benchmark.bat             # Script chạy benchmark 1-click
│   ├── bench_10m_fast.txt            # Báo cáo kết quả đo đạc 10.000.000 sinh viên (Fast Mode)
│   └── bench_10m_strict.txt          # Báo cáo kết quả đo đạc 10.000.000 sinh viên (Strict Mode)
│
├── web_dsa/                          # 🌐 ỨNG DỤNG WEB DASHBOARD (FULLSTACK)
│   ├── src/                          # Frontend Next.js 16 + React 19 + Ant Design + Tailwind CSS
│   │   ├── app/                      # App Router (page.tsx, layout.tsx)
│   │   └── components/               # BenchmarkTab, FinalSolutionTab, CrudTab, v.v.
│   ├── server.js                     # Backend API Express.js giao tiếp Resident C++ Engine (Port 5000)
│   ├── start_web.bat                 # Script khởi chạy toàn bộ Web Dashboard (Backend + Frontend)
│   ├── test_speed.js                 # Script kiểm thử API Latency
│   ├── nodemon.json                  # Cấu hình hot-reload cho Backend
│   └── package.json                  # Hợp nhất toàn bộ dependencies của ứng dụng Web
│
├── scripts/                          # 🐍 CÁC SCRIPT TIỆN ÍCH & RUNNERS
│   ├── run_cli.bat                   # Script khởi chạy ứng dụng Console CLI C++
│   └── generate_data.py              # Script tự động sinh N bản ghi sinh viên vào data/database.json
│
├── bao_cao/                          # 📄 BÁO CÁO HỌC PHẦN (LATEX & PDF)
│   ├── main.tex                      # Mã nguồn LaTeX báo cáo chính thức
│   ├── main.pdf                      # Bản báo cáo PDF hoàn chỉnh
│   └── image/                        # Hình ảnh sơ đồ, biểu đồ benchmark
│
├── dsa_bridge.cpp                    # Resident C++ Core Engine kết nối Web qua IPC Stdin/Stdout
├── main.cpp                          # Ứng dụng Console CLI tương tác C++ chính
└── README.md                         # Tài liệu hướng dẫn sử dụng chi tiết
```

---

## 🛠️ Yêu Cầu Môi Trường (Prerequisites)

1. **Trình biên dịch C++ (`g++`)**: Hỗ trợ chuẩn **C++17** hoặc **C++20** (MinGW-w64 / GCC) và đã có trong biến môi trường `PATH`.
2. **Node.js**: Phiên bản **>= 18.x** (khuyên dùng LTS, tải tại [nodejs.org](https://nodejs.org/)).
3. **Python 3.x** *(Tùy chọn)*: Dùng khi cần chạy script sinh thêm dữ liệu ngẫu nhiên.

---

## 🧪 Hướng Dẫn Chạy Bộ Test Tự Động (Automated Test Suite)

Bộ kiểm thử tự động được viết **100% bằng C++** tại [test/automated_test.cpp](test/automated_test.cpp), kiểm tra tính đúng đắn logic của cả **6 nhóm chức năng (28 Test Cases)** với đối soát Ground Truth và các trường hợp biên:

### 1. Danh Sách 6 Test Suites
- **Suite 1 (MC1 — Closed Hash Table):** Kiểm tra tra cứu đầu/giữa/cuối dải, tìm khóa không tồn tại trả về `nullptr`, xử lý chuỗi rỗng và kiểm soát va chạm (Linear Probing).
- **Suite 2 (MC2 — Custom Max-Heap):** Kiểm tra tìm đúng sinh viên điểm GPA cao nhất ($10.0$), kiểm tra quy tắc **Tie-break** (ưu tiên MSSV nhỏ hơn khi cùng GPA), kiểm tra Heap 1 phần tử.
- **Suite 3 (RQ1 — Class Index View):** Kiểm tra lọc theo mã lớp, lọc mã lớp không tồn tại trả về danh sách rỗng, đối chiếu số lượng kết quả khớp 100% với Linear Scan Ground Truth.
- **Suite 4 (RQ2 — Sorted GPA Filter):** Kiểm tra lọc khoảng $[8.0, 9.5]$, đối chiếu số lượng khớp 100% với Ground Truth, xử lý an toàn khoảng ngược $[9.0, 7.0]$, khoảng ngoài biên $[11.0, 12.0]$ và toàn dải $[0.0, 10.0]$.
- **Suite 5 (CRUD Operations):** Kiểm tra thêm mới hợp lệ, từ chối thêm trùng MSSV, từ chối GPA $> 10.0$, cập nhật thông tin thành công, xóa sinh viên, báo lỗi khi sửa/xóa MSSV không tồn tại.
- **Suite 6 (Edge Cases & Boundaries):** Kiểm tra xử lý an toàn tập dữ liệu rỗng ($N = 0$) trên cả 4 cấu trúc dữ liệu, kiểm tra trường hợp tất cả sinh viên đồng điểm GPA.

### 2. Cách Chạy Test

#### Cách A: 1-Click bằng Batch Script (Khuyên dùng trên Windows)
Nhấp đúp chuột vào file **`test\run_tests.bat`** (hoặc chạy từ terminal):
```cmd
cd test
run_tests.bat
```

#### Cách B: Biên dịch và chạy thủ công bằng `g++`
Từ thư mục gốc dự án:
```bash
g++ -O2 -std=c++17 -I. test/automated_test.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp -o test/automated_test.exe
.\test\automated_test.exe
```

### 3. Kết Quả Kiểm Thử Thực Tế (Output)
```text
====================================================================
    HE THONG TEST TU DONG PHU MOI YEU CAU (AUTOMATED TEST SUITE)   
    Do an Mon hoc: Cau truc Du lieu va Giai thuat - Nhom 12        
====================================================================

[TEST SUITE 1] MC1: Closed Hash Table (Tra cứu theo MSSV)
  [PASS] 1.1 Tra cuu chinh xac phan tu dau dai
  [PASS] 1.2 Tra cuu chinh xac phan tu giua dai
  [PASS] 1.3 Tra cuu chinh xac phan tu cuoi dai
  [PASS] 1.4 Tra cuu MSSV khong ton tai tra ve nullptr an toan
  [PASS] 1.5 Tra cuu MSSV rong tra ve nullptr an toan
  [PASS] 1.6 Bang bam khoi tao hop le voi collisions co kiem soat

[TEST SUITE 2] MC2: Custom Max-Heap (Truy xuất GPA cao nhất)
  [PASS] 2.1 Tim dung sinh vien co GPA cao nhat = 10.0
  [PASS] 2.2 Xu ly Tie-break uu tien MSSV nho hon khi cung GPA
  [PASS] 2.3 Heap hoat dong chinh xac voi tap du lieu 1 phan tu

[TEST SUITE 3] RQ1: Class Index View (Lọc sinh viên theo Mã Lớp)
  [PASS] 3.1 Loc chinh xac tat ca sinh vien thuoc lop 25DTH1
  [PASS] 3.2 Loc ma lop khong ton tai tra ve danh sach rong (0 SV)
  [PASS] 3.3 So luong ket qua trung khop 100% voi Linear Scan Ground Truth

[TEST SUITE 4] RQ2: Sorted GPA Range Filter (Lọc theo khoảng GPA)
  [PASS] 4.1 Loc dung tat ca sinh vien trong khoang [8.0, 9.5]
  [PASS] 4.2 So luong khop 100% voi Linear Ground Truth
  [PASS] 4.3 Khoang nguoc [9.0, 7.0] xu ly an toan va tra ve 0 ket qua
  [PASS] 4.4 Khoang ngoai bien [11.0, 12.0] tra ve 0 ket qua
  [PASS] 4.5 Toan dai [0.0, 10.0] tra ve dung toan bo 1000 sinh vien

[TEST SUITE 5] CRUD: Create - Update - Delete Operations
  [PASS] 5.1 Them moi sinh vien hop le thanh cong
  [PASS] 5.2 Tu choi them trung MSSV va bao loi hop le
  [PASS] 5.3 Tu choi them GPA > 10.0 va bao loi hop le
  [PASS] 5.4 Cap nhat thong tin sinh vien thanh cong
  [PASS] 5.5 Bao loi khi cap nhat MSSV khong ton tai
  [PASS] 5.6 Xoa sinh vien hop le thanh cong
  [PASS] 5.7 Bao loi khi xoa MSSV khong ton tai

[TEST SUITE 6] Edge Cases & System Boundary Conditions
  [PASS] 6.1 Hash Table xu ly an toan tap du lieu rong (N = 0)
  [PASS] 6.2 Sorted GPA xu ly an toan tap du lieu rong (N = 0)
  [PASS] 6.3 Class Index xu ly an toan tap du lieu rong (N = 0)
  [PASS] 6.4 Sorted GPA loc chinh xac 100% khi tat ca SV dong GPA

====================================================================
                      KET QUA KIEM THU TONG HOP                     
====================================================================
  Tong so Test Cases : 28
  So Test Dat [PASS] : 28
  So Test Loi [FAIL] : 0
  Ty le thanh cong   : 100%
====================================================================
```

---

## ⚡ Hướng Dẫn Chạy Đo Lường Hiệu Năng (Benchmark Engine)

Công cụ đo lường độc lập [benchmark/bench_core.cpp](benchmark/bench_core.cpp) được tối ưu hóa tối đa với cờ `-O3 -flto` để đánh giá đối sánh hiệu năng thực tế trên tập dữ liệu lên đến **10,000,000 sinh viên**.

### 1. Cách Chạy Benchmark

#### Cách A: 1-Click bằng Batch Script (Khuyên dùng trên Windows)
Nhấp đúp chuột vào file **`benchmark\run_benchmark.bat`** (hoặc chạy từ terminal):
```cmd
cd benchmark
run_benchmark.bat
```

#### Cách B: Biên dịch và chạy thủ công bằng `g++`
Từ thư mục gốc dự án:
```bash
g++ -O3 -march=native -funroll-loops -flto -DNDEBUG -std=c++17 -I. benchmark/bench_core.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp -o benchmark/bench_core.exe
.\benchmark\bench_core.exe
```

### 2. Các Tệp Kết Quả Benchmark Đã Đo Đạc
- [benchmark/bench_10m_fast.txt](benchmark/bench_10m_fast.txt): Kết quả đo nhanh 100 queries trên tập 10 triệu bản ghi.
- [benchmark/bench_10m_strict.txt](benchmark/bench_10m_strict.txt): Kết quả đo chuẩn mực (1,000 queries, 5 lần warm-up, 9 lần lấy mẫu thống kê `min`/`median`/`mean`/`max`).

### 3. Bảng Số Liệu Đối Sánh Chi Tiết ($N = 10,000,000$ Sinh Viên)

| Phép Thử Nghiệm | Giải Thuật Tuyến Tính (Baseline) | Giải Thuật Tối Ưu (Final Solution) | Số Phép So Sánh (Baseline vs Final) | Tỷ Lệ Cải Thiện Hiệu Năng |
| :--- | :---: | :---: | :---: | :---: |
| **MC1: Tra cứu 1000 MSSV** | $26,424.51\text{ ms}$ | **$0.024\text{ ms}$** | $5,227,717,750 \to 1,564\text{ probes}$ | **Tăng tốc ~ 1,080,000 lần** |
| **MC2: Truy xuất 100 lần Max GPA** | $3,819.02\text{ ms}$ | **$0.016\text{ ms}$** | $999,999,900 \to 1\text{ (Peek Root)}$ | **Tăng tốc ~ 238,000 lần** |
| **RQ1: Lọc theo Mã Lớp** | $77.90\text{ ms}$ | **$0.0014\text{ ms}$** | $10,000,000 \to 1\text{ (Index View)}$ | **Tăng tốc ~ 55,000 lần** |
| **RQ2: Lọc khoảng GPA** | $331.48\text{ ms}$ | **$0.017\text{ ms}$** | $13,343,566 \to 46\text{ (Binary Search)}$ | **Tăng tốc ~ 19,000 lần** |

---

## 🚀 Hướng Dẫn Khởi Chạy Toàn Bộ Dự Án (Quick Start & Launch Guide)

Dự án cung cấp 2 phương thức vận hành hoàn chỉnh: **Ứng dụng Console CLI C++** (Tương tác dòng lệnh mượt mà) và **Ứng dụng Web Dashboard Fullstack** (Giao diện trực quan hiện đại).

```text
                                 KIẾN TRÚC VẬN HÀNH DỰ ÁN
    ┌─────────────────────────────────────────────────────────────────────────────────┐
    │                                                                                 │
    │  [CÁCH 1] CONSOLE CLI C++               [CÁCH 2] WEB DASHBOARD FULLSTACK        │
    │  ┌───────────────────────┐              ┌────────────────────────────────────┐  │
    │  │ main.cpp              │              │ Web App Frontend (Next.js / React) │  │
    │  │ (Direct C++ In-Memory)│              │ Port: 3000                         │  │
    │  │ 4 Modes Điều Khiển    │              └─────────────────┬──────────────────┘  │
    │  └───────────────────────┘                                │ HTTP / JSON         │
    │                                         ┌─────────────────▼──────────────────┐  │
    │                                         │ Backend API Server (Express.js)    │  │
    │                                         │ Port: 5000                         │  │
    │                                         └─────────────────┬──────────────────┘  │
    │                                                           │ IPC (Stdin/Stdout)  │
    │                                         ┌─────────────────▼──────────────────┐  │
    │                                         │ Resident C++ Engine (dsa_bridge)   │  │
    │                                         │ 0ms Latency - In-Memory Daemon     │  │
    │                                         └────────────────────────────────────┘  │
    └─────────────────────────────────────────────────────────────────────────────────┘
```

---

### 💻 1. Khởi Chạy Ứng Dụng Console CLI C++ (`main.cpp`)

Ứng dụng Console CLI hỗ trợ điều hướng tương tác menu chuyên nghiệp bằng bàn phím (Phím mũi tên `↑` `↓`, `Enter` để chọn, `Esc` để quay lại/hủy bỏ, hoặc bấm trực tiếp phím số `1`, `2`, `3`, `4`, `0`).

#### 🔹 Cách A: Khởi chạy 1-Click (Khuyên dùng)
Nhấp đúp chuột vào file **`scripts\run_cli.bat`** (hoặc chạy lệnh từ terminal):
```cmd
.\scripts\run_cli.bat
```

#### 🔹 Cách B: Biên dịch và chạy thủ công qua Terminal
Từ thư mục gốc dự án:
```bash
# 1. Biên dịch toàn bộ mã nguồn C++ với tối ưu O2
g++ -O2 -std=c++17 -I. main.cpp src/*.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp -o main.exe

# 2. Khởi chạy chương trình
.\main.exe
```

#### 🎮 Các Chế Độ Hoạt Động (4 Modes trong CLI Menu):
1. **`[ 1 ] MODE 1: GIAI THUAT TOI UU (FINAL SOLUTION / PRODUCTION)`**
   - Vận hành trực tiếp các cấu trúc dữ liệu tối ưu trên RAM:
     - Lọc khoảng GPA qua Sorted Pointer Array + Binary Search $\mathcal{O}(\log N)$.
     - Lọc Mã Lớp qua Class Index View $\mathcal{O}(1)$.
     - Tra cứu sinh viên theo MSSV qua Closed Hash Table $\mathcal{O}(1)$.
     - Truy xuất sinh viên điểm GPA cao nhất qua Custom Binary Max-Heap $\mathcal{O}(1)$.
2. **`[ 2 ] MODE 2: HE THONG QUAN LY SINH VIEN (CRUD)`**
   - Thêm sinh viên mới (Tự động cấp phát MSSV tăng dần, không trùng lặp).
   - Sửa thông tin sinh viên (Chọn tương tác bằng phím mũi tên `↑` `↓`, `Enter`, hủy bỏ bằng `Esc`).
   - Xóa hồ sơ sinh viên với hộp thoại xác nhận an toàn.
   - Hiển thị danh sách và đồng bộ dữ liệu vào file `data/database.json`.
3. **`[ 3 ] MODE 3: SO SANH THUAT TOAN TUNG MODULE (BENCHMARK SUITE)`**
   - Menu đối sánh độc lập từng bài toán: Module 1 (GPA), Module 2 (Lớp), Module 4 (MSSV), Module 5 (Max GPA).
   - In bảng phân tích so sánh trực quan giữa **Baseline tuyến tính** và **Cấu trúc tối ưu**.
4. **`[ 4 ] MODE 4: BENCHMARK TOAN DIEN & XUAT FILE (AUTO BENCHMARK & EXPORT TXT)`**
   - **Chọn nguồn dữ liệu:** Database mặc định ($500\text{K}$ SV), Tự động sinh $100\text{K}$, $500\text{K}$, $1\text{M}$, $10\text{M}$ hoặc số lượng tùy chỉnh $N$.
   - **Tự động đo đạc 1-click** toàn bộ 4 module (Xây dựng cấu trúc dữ liệu, chạy thử nghiệm nhiều mẫu, tính toán Speedup $\times$ lần).
   - **Tự động xuất báo cáo ra file `.txt`** được đánh dấu thời gian thực trong thư mục `benchmark/` theo định dạng `benchmark/bench_report_YYYYMMDD_HHMMSS.txt`.
0. **`[ 0 ] Thoat chuong trinh`**

---

### 🌐 2. Khởi Chạy Ứng Dụng Web Dashboard Fullstack (`web_dsa`)

Giao diện Web Dashboard hiện đại được xây dựng bằng **Next.js 16 (App Router) + React 19 + Tailwind CSS + Ant Design**, kết nối với **Express.js API Server** (Port 5000) và **C++ Resident Core Engine Daemon** (`dsa_bridge.exe`) thông qua giao thức IPC siêu tốc (Độ trễ xử lý $\approx 0\text{ms}$).

#### 🔹 Bước 1: Cài đặt Dependencies (Chỉ thực hiện lần đầu tiên)
Mở terminal tại thư mục gốc và chạy:
```bash
cd web_dsa
npm install
cd ..
```

#### 🔹 Bước 2: Khởi chạy Ứng dụng Web

##### Cách A: Khởi chạy 1-Click bằng Batch Script (Khuyên dùng)
Nhấp đúp chuột vào file **`web_dsa\start_web.bat`** (hoặc chạy từ terminal):
```cmd
.\web_dsa\start_web.bat
```
> **Cơ chế tự động của `start_web.bat`:**
> 1. Tự động kiểm tra và biên dịch `dsa_bridge.cpp` thành `dsa_bridge.exe`.
> 2. Khởi động Backend Express API Server tại `http://localhost:5000` và nạp sẵn dữ liệu sinh viên vào RAM.
> 3. Khởi động Frontend Next.js Dev Server tại `http://localhost:3000`.

##### Cách B: Khởi chạy thủ công từng phần bằng Terminal

* **Cửa sổ Terminal 1 (Biên dịch C++ Engine & Khởi động Backend API):**
  ```bash
  # 1. Biên dịch C++ Bridge Engine
  g++ -O2 -std=c++17 -I. -o dsa_bridge.exe dsa_bridge.cpp src/core_sorted_gpa/*.cpp src/core_class_filter/*.cpp src/core_crud/*.cpp src/core_hash/*.cpp src/core_heap/*.cpp

  # 2. Khởi chạy Backend API Server
  cd web_dsa
  npm run server
  ```

* **Cửa sổ Terminal 2 (Khởi động Frontend Next.js):**
  ```bash
  cd web_dsa
  npm run dev
  ```

#### 🔹 Bước 3: Truy cập và Sử dụng Web Dashboard
Mở trình duyệt web bất kỳ và truy cập vào:
- 🌐 **Frontend Web Dashboard**: [http://localhost:3000](http://localhost:3000)
- 🔌 **Backend RESTful API**: [http://localhost:5000](http://localhost:5000)

#### 🌟 Các Tính Năng Nổi Bật Trên Web Dashboard:
- **Tab Benchmark Suite:** Biểu đồ đối sánh hiệu năng trực quan, theo dõi thời gian thực thi (Latency $\text{ms}$) và số phép toán thu nhỏ giữa Baseline và Thuật toán tối ưu.
- **Tab Final Solution:** Tra cứu hồ sơ theo MSSV tức thì, tìm sinh viên GPA cao nhất, lọc danh sách theo lớp và khoảng điểm GPA với phân trang mượt mà.
- **Tab Quản lý Hồ sơ (CRUD):** Thêm mới, chỉnh sửa và xóa hồ sơ sinh viên trực tiếp với thông báo thành công tức thì và đồng bộ bộ nhớ.
- **In-Memory Scale Generator:** Thanh điều khiển sinh dữ liệu tức thì từ **500,000 đến 10,000,000+ sinh viên** nạp thẳng vào RAM chỉ mất vài trăm mili-giây.

---

### 🐍 3. Sinh Dữ Liệu Tự Động (Python Data Generator)

Để tạo mới hoặc khôi phục file cơ sở dữ liệu mẫu `data/database.json` ($500,000$ bản ghi sinh viên chuẩn):
```bash
python scripts/generate_data.py
```

---

## 📄 Tài Liệu Báo Cáo Học Phần

Bản báo cáo học thuật định dạng LaTeX chuẩn chỉ, bố cục hình ảnh và bảng biểu cân đối:
* **Mã nguồn LaTeX:** [bao_cao/main.tex](bao_cao/main.tex)
* **Bản PDF xuất bản:** [bao_cao/main.pdf](bao_cao/main.pdf)

---
*© 2026 - Nhóm 12 | Đồ án Môn học Cấu Trúc Dữ Liệu và Giải Thuật (HCMUTE)*

