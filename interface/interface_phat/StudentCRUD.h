// ============================================================================
// MODULE: QUẢN LÝ THÊM - SỬA - XÓA SINH VIÊN (TÁC GIẢ: PHÁT)
// ============================================================================
// [MÔ TẢ VÀ KIẾN TRÚC THIẾT KẾ]:
// 1. TÁI SỬ DỤNG DỮ LIỆU DÙNG CHUNG:
//    - Nhận trực tiếp tham chiếu mảng `vector<Student>&` từ `main.cpp`
//      (Dữ liệu đã được nạp sẵn qua hàm `loadStudentsData`).
// 2. CÁC THAO TÁC CHÍNH (CREATE - UPDATE - DELETE):
//    - Thêm (Create): Thêm sinh viên mới (kiểm tra không trùng ID, GPA hợp lệ từ 0.0 đến 10.0).
//    - Sửa  (Update): Cập nhật lại Họ tên, Mã lớp, Điểm GPA theo MSSV.
//    - Xóa  (Delete): Xóa sinh viên ra khỏi danh sách theo MSSV.
// 3. TÍNH NĂNG GHI FILE (PERSISTENCE):
//    - Cung cấp hàm `saveToFile` để ghi ngược các thay đổi (Thêm/Sửa/Xóa)
//      từ RAM vào file `data/database.json`.
// ============================================================================

#ifndef STUDENT_CRUD_H
#define STUDENT_CRUD_H

#include <string>
#include <vector>
#include "../student.h"

using namespace std;

// Cấu trúc lưu trữ kết quả trả về của các thao tác Thêm / Sửa / Xóa
struct CRUDResult
{
    bool success;    // Kết quả thực hiện: true (thành công) / false (thất bại)
    string message;  // Thông báo chi tiết kết quả (thành công hoặc lý do lỗi)
    Student student; // Dữ liệu sinh viên liên quan đến thao tác

    // Constructor mặc định khởi tạo giá trị ban đầu rõ ràng
    CRUDResult()
    {
        this->success = false;
        this->message = "";
        this->student.id = "";
        this->student.name = "";
        this->student.classId = "";
        this->student.gpa = 0.0;
    }
};

class StudentCRUD
{
private:
    vector<Student> *studentsPtr; // Con trỏ trỏ đến mảng sinh viên dùng chung trong RAM
    string dbFilePath;            // Đường dẫn file JSON cơ sở dữ liệu để ghi khi lưu

    // Hàm phụ trợ: Tìm vị trí (index) của sinh viên trong vector theo MSSV
    // Trả về: vị trí index từ 0 đến N-1 nếu tìm thấy, hoặc trả về -1 nếu không tìm thấy
    int findIndexById(const string &studentId) const;

    // Hàm phụ trợ: Tự động sinh mã sinh viên (MSSV) tăng tự động (Auto-increment)
    // Ví dụ: 25150100 -> 25150101
    string generateNextStudentId() const;

public:
    // ========================================================================
    // KHỞI TẠO VÀ CẤU HÌNH
    // ========================================================================

    // Constructor: Nhận tham chiếu mảng sinh viên từ main.cpp và đường dẫn file JSON
    explicit StudentCRUD(vector<Student> &students, const string &jsonFilePath = "data/database.json");

    // ========================================================================
    // TÍNH NĂNG GHI FILE JSON (LƯU DỮ LIỆU)
    // ========================================================================

    // Ghi toàn bộ danh sách sinh viên hiện tại trong RAM ngược lại file database.json
    // Trả về: true nếu ghi file thành công, false nếu ghi file thất bại
    bool saveToFile(const string &filePath = "") const;

    // ========================================================================
    // 1. CREATE (THÊM MỚI SINH VIÊN)
    // ========================================================================

    // Thêm một sinh viên mới vào mảng dữ liệu trong RAM
    // - newStudent: đối tượng sinh viên cần thêm mới
    // - autoSave: nếu bằng true thì sẽ tự động ghi dữ liệu vào file JSON sau khi thêm
    CRUDResult createStudent(const Student &newStudent, bool autoSave = true);

    // Giao diện nhập thông tin từ bàn phím để thêm sinh viên
    void createStudentInteractive();

    // ========================================================================
    // 2. UPDATE (CẬP NHẬT / SỬA THÔNG TIN)
    // ========================================================================

    // Cập nhật thông tin của sinh viên theo MSSV
    // - studentId: MSSV của sinh viên cần chỉnh sửa
    // - newName: Họ và tên mới
    // - newClassId: Mã lớp mới
    // - newGpa: Điểm GPA mới
    // - autoSave: nếu bằng true thì sẽ tự động ghi vào file JSON sau khi sửa
    CRUDResult updateStudent(const string &studentId, const string &newName, const string &newClassId, double newGpa, bool autoSave = true);

    // Giao diện nhập thông tin từ bàn phím để chỉnh sửa sinh viên
    void updateStudentInteractive();

    // ========================================================================
    // 3. DELETE (XÓA SINH VIÊN)
    // ========================================================================

    // Xóa một sinh viên ra khỏi danh sách theo MSSV
    // - studentId: MSSV của sinh viên cần xóa
    // - autoSave: nếu bằng true thì sẽ tự động ghi vào file JSON sau khi xóa
    CRUDResult deleteStudentById(const string &studentId, bool autoSave = true);

    // Giao diện xác nhận và nhập MSSV từ bàn phím để xóa sinh viên
    void deleteStudentInteractive();

    // ========================================================================
    // TÍNH NĂNG CHỌN SINH VIÊN BẰNG PHÍM ĐIỀU HƯỚNG (ARROWS NAVIGATION)
    // ========================================================================

    // Giao diện chọn sinh viên trực quan bằng phím mũi tên:
    // - Phím Lên (↑) / Xuống (↓): Di chuyển chọn từng sinh viên
    // - Phím Trái (←) / Phải (→): Chuyển Trang trước / Trang sau
    // - Phím Enter: Xác nhận chọn sinh viên
    // - Phím Esc hoặc q: Hủy bỏ thao tác
    // Trả về: index (0 đến N-1) của sinh viên được chọn, hoặc -1 nếu hủy bỏ
    int selectStudentInteractive(const string &title = "DANH SACH SINH VIEN") const;

    // ========================================================================
    // MENU ĐIỀU KHIỂN TƯƠNG TÁC CONSOLE
    // ========================================================================

    // Chạy menu tương tác các chức năng Thêm - Sửa - Xóa cho người dùng
    void runCRUDMenu();
};

#endif // STUDENT_CRUD_H
