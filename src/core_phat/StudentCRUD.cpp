// ============================================================================
// MODULE: CÀI ĐẶT THAO TÁC THÊM - SỬA - XÓA SINH VIÊN (TÁC GIẢ: PHÁT)
// ============================================================================
// [CHI TIẾT VÀ NGUYÊN LÝ HOẠT ĐỘNG]:
// 1. TƯƠNG TÁC TRÊN BỘ NHỚ RAM:
//    - Toàn bộ thao tác Thêm/Sửa/Xóa đều xử lý trực tiếp trên `vector<Student>`
//      thông qua con trỏ `this->studentsPtr` để đảm bảo tốc độ cao nhất.
// 2. TỰ ĐỘNG SINH MÃ SINH VIÊN (AUTO-INCREMENT ID):
//    - Khi thêm mới, hệ thống tự động quét mã lớn nhất (Max ID) hiện có trong CSDL
//      và tăng lên 1 (VD: 25150100 -> 25150101), người dùng không cần nhập MSSV thủ công.
// 3. VÀO THẲNG DANH SÁCH CHỌN MŨI TÊN KHI SỬA / XÓA (STREAMLINED FLOW):
//    - Khi chọn Update hoặc Delete, hệ thống mở trực tiếp bảng chọn bằng phím
//      mũi tên `↑` `↓` `←` `→`, bấm `Enter` để chọn, bấm `ESC` để hủy tức thì.
// 4. HỖ TRỢ PHÍM ESC ĐỂ HỦY BỎ BẤT KỲ LÚC NÀO (CANCEL WITH ESC):
//    - Trong quá trình nhập Họ tên, Lớp, GPA hoặc xác nhận Xóa/Sửa, người dùng
//      chỉ cần bấm phím `ESC` là hệ thống sẽ lập tức hủy bỏ thao tác và quay về menu.
// ============================================================================

#include "../../interface/interface_phat/StudentCRUD.h"
#include "../../nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;
using json = nlohmann::json;

// Hàm phụ trợ xóa màn hình Terminal
static void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Hàm phụ trợ tạm dừng màn hình (chỉ cần bấm 1 phím bất kỳ là tiếp tục ngay)
static void pauseScreen()
{
#ifdef _WIN32
    cout << "\nNhan phim bat ky de tiep tuc...";
    _getch();
#else
    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

// ============================================================================
// HÀM NHẬP DỮ LIỆU TƯƠNG TÁC CÓ BẮT PHÍM ESC ĐỂ HỦY BỎ (ESC TO CANCEL)
// ============================================================================
// Trả về: true nếu người dùng hoàn tất nhập bằng Enter, false nếu người dùng bấm ESC để hủy
static bool inputStringWithEsc(const string &prompt, string &outStr, bool allowEmpty = false, const string &defaultValue = "")
{
    cout << prompt;
    outStr = "";

#ifdef _WIN32
    while (true)
    {
        int ch = _getch();
        if (ch == 27) // Phím ESC
        {
            cout << " [Da huy bo]\n";
            return false;
        }
        else if (ch == 13) // Phím ENTER
        {
            cout << "\n";
            if (outStr.empty() && !defaultValue.empty())
            {
                outStr = defaultValue;
            }
            if (allowEmpty || !outStr.empty())
            {
                return true;
            }
            cout << "[Loi] Khong duoc de trong. " << prompt;
        }
        else if (ch == 8) // Phím BACKSPACE
        {
            if (!outStr.empty())
            {
                outStr.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch == 0 || ch == 224) // Phím chức năng / mũi tên
        {
            _getch(); // Bỏ qua mã thứ 2
        }
        else if (ch >= 32 && ch <= 126) // Ký tự in được
        {
            outStr.push_back(static_cast<char>(ch));
            cout << static_cast<char>(ch);
        }
    }
#else
    getline(cin, outStr);
    if (outStr == "0" || outStr == "q" || outStr == "Q")
    {
        return false;
    }
    return true;
#endif
}

// ============================================================================
// 1. KHỞI TẠO (CONSTRUCTOR)
// ============================================================================
StudentCRUD::StudentCRUD(vector<Student> &students, const string &jsonFilePath)
{
    this->studentsPtr = &students;
    this->dbFilePath = jsonFilePath;
}

// ============================================================================
// HÀM PHỤ TRỢ NỘI BỘ: TÌM INDEX THEO MSSV
// ============================================================================
int StudentCRUD::findIndexById(const string &studentId) const
{
    if (this->studentsPtr == nullptr)
    {
        return -1;
    }

    for (size_t i = 0; i < this->studentsPtr->size(); i++)
    {
        if ((*this->studentsPtr)[i].id == studentId)
        {
            return static_cast<int>(i);
        }
    }
    return -1;
}

// ============================================================================
// HÀM PHỤ TRỢ NỘI BỘ: TỰ ĐỘNG SINH MSSV TĂNG DẦN (AUTO-INCREMENT)
// ============================================================================
string StudentCRUD::generateNextStudentId() const
{
    if (this->studentsPtr == nullptr || this->studentsPtr->empty())
    {
        return "25150001";
    }

    long long maxId = 0;
    for (const auto &st : *this->studentsPtr)
    {
        try
        {
            long long val = stoll(st.id);
            if (val > maxId)
            {
                maxId = val;
            }
        }
        catch (...)
        {
            // Bỏ qua nếu có ID không phải là số thuần
        }
    }

    if (maxId == 0)
    {
        return "25150001";
    }

    return to_string(maxId + 1);
}

// ============================================================================
// TÍNH NĂNG GHI FILE JSON (LƯU THAY ĐỔI VÀO CSDL)
// ============================================================================
bool StudentCRUD::saveToFile(const string &filePath) const
{
    string targetPath = filePath.empty() ? this->dbFilePath : filePath;

    if (this->studentsPtr == nullptr)
    {
        cerr << "[Loi] Con tro du lieu sinh vien khong ton tai!\n";
        return false;
    }

    ofstream file(targetPath);
    if (!file.is_open())
    {
        cerr << "[Loi] Khong the mo file de ghi: " << targetPath << '\n';
        return false;
    }

    try
    {
        json root;
        json studentsArray = json::array();

        for (const auto &st : *this->studentsPtr)
        {
            json item;
            item["id"] = st.id;
            item["name"] = st.name;
            item["classId"] = st.classId;
            item["gpa"] = st.gpa;
            studentsArray.push_back(item);
        }

        root["students"] = studentsArray;

        // Ghi vào file với thụt lề 4 dấu cách để file JSON đẹp và dễ đọc
        file << setw(4) << root << endl;
        file.close();

        return true;
    }
    catch (const exception &e)
    {
        cerr << "[Loi JSON khi ghi file] " << e.what() << '\n';
        return false;
    }
}

// ============================================================================
// CHỌN SINH VIÊN TƯƠNG TÁC BẰNG CÁC PHÍM MŨI TÊN (ARROW KEYS NAVIGATION)
// ============================================================================
int StudentCRUD::selectStudentInteractive(const string &title) const
{
    if (this->studentsPtr == nullptr || this->studentsPtr->empty())
    {
        cout << "[Thong bao] Danh sach sinh vien trong RAM hien tai dang rong!\n";
        return -1;
    }

    const int PAGE_SIZE = 10;
    int totalStudents = static_cast<int>(this->studentsPtr->size());
    int totalPages = (totalStudents + PAGE_SIZE - 1) / PAGE_SIZE;
    int currentIndex = 0;

    while (true)
    {
        clearScreen();
        int currentPage = currentIndex / PAGE_SIZE;
        int startIdx = currentPage * PAGE_SIZE;
        int endIdx = min(startIdx + PAGE_SIZE, totalStudents);

        cout << "=========================================================================================\n";
        cout << "               " << title << " (Trang " << (currentPage + 1) << "/" << totalPages << ")\n";
        cout << "=========================================================================================\n";
        cout << left << setw(6)  << " CHON"
             << " | " << setw(5)  << "STT"
             << " | " << setw(10) << "MSSV"
             << " | " << setw(26) << "HO VA TEN"
             << " | " << setw(10) << "LOP"
             << " | " << setw(6)  << "GPA" << "\n";
        cout << "-----------------------------------------------------------------------------------------\n";

        for (int i = startIdx; i < endIdx; i++)
        {
            const Student &st = (*this->studentsPtr)[i];
            if (i == currentIndex)
            {
                cout << "  -->  | "
                     << right << setw(4) << (i + 1) << " | "
                     << setw(10) << st.id << " | "
                     << left << setw(26) << st.name << " | "
                     << setw(10) << st.classId << " | "
                     << right << setw(5) << fixed << setprecision(2) << st.gpa
                     << "  <== [DANG CHON]\n";
            }
            else
            {
                cout << "       | "
                     << right << setw(4) << (i + 1) << " | "
                     << setw(10) << st.id << " | "
                     << left << setw(26) << st.name << " | "
                     << setw(10) << st.classId << " | "
                     << right << setw(5) << fixed << setprecision(2) << st.gpa << "\n";
            }
        }

        cout << "=========================================================================================\n";
        cout << " [HUONG DAN DIEU KHIEN]:                                                                 \n";
        cout << "   [ Phim Len / Xuong ] : Di chuyen con tro chon sinh vien                               \n";
        cout << "   [ Phim Trai / Phai ] : Chuyen Trang truoc / Trang sau                                 \n";
        cout << "   [ Phim Enter ]       : XAC NHAN CHON SINH VIEN                                        \n";
        cout << "   [ Phim Esc / q ]     : HUY BO VA QUAY LAI MENU                                        \n";
        cout << "=========================================================================================\n";

#ifdef _WIN32
        int ch = _getch();
        if (ch == 0 || ch == 224) // Phím mũi tên đặc biệt trên Windows
        {
            int arrow = _getch();
            if (arrow == 72) // Mũi tên LÊN (UP)
            {
                if (currentIndex > 0)
                    currentIndex--;
                else
                    currentIndex = totalStudents - 1; // Vòng xuống cuối
            }
            else if (arrow == 80) // Mũi tên XUỐNG (DOWN)
            {
                if (currentIndex < totalStudents - 1)
                    currentIndex++;
                else
                    currentIndex = 0; // Vòng lên đầu
            }
            else if (arrow == 75) // Mũi tên TRÁI (LEFT - Trang trước)
            {
                currentIndex = max(0, currentIndex - PAGE_SIZE);
            }
            else if (arrow == 77) // Mũi tên PHẢI (RIGHT - Trang sau)
            {
                currentIndex = min(totalStudents - 1, currentIndex + PAGE_SIZE);
            }
        }
        else if (ch == 13) // Phím ENTER
        {
            return currentIndex;
        }
        else if (ch == 27 || ch == 'q' || ch == 'Q') // Phím ESC hoặc Q
        {
            return -1;
        }
#else
        cout << "Nhap STT sinh vien (1-" << totalStudents << ") hoac 0 de thoat: ";
        int num;
        if (cin >> num && num >= 1 && num <= totalStudents)
        {
            return num - 1;
        }
        return -1;
#endif
    }
}

// ============================================================================
// 2. CREATE (THÊM MỚI SINH VIÊN - MSSV TỰ ĐỘNG TĂNG, HỖ TRỢ ESC ĐỂ HỦY)
// ============================================================================
CRUDResult StudentCRUD::createStudent(const Student &newStudent, bool autoSave)
{
    CRUDResult result;

    if (this->studentsPtr == nullptr)
    {
        result.success = false;
        result.message = "Con tro du lieu chua duoc khoi tao.";
        return result;
    }

    Student stToAdd = newStudent;

    // 1. Tự động sinh ID nếu chưa có
    if (stToAdd.id.empty())
    {
        stToAdd.id = this->generateNextStudentId();
    }

    // 2. Kiểm tra trùng khóa chính MSSV
    if (this->findIndexById(stToAdd.id) != -1)
    {
        result.success = false;
        result.message = "MSSV '" + stToAdd.id + "' da ton tai trong he thong.";
        return result;
    }

    // 3. Kiểm tra tính hợp lệ của điểm GPA (0.0 đến 10.0)
    if (stToAdd.gpa < 0.0 || stToAdd.gpa > 10.0)
    {
        result.success = false;
        result.message = "Diem GPA khong hop le (GPA phai tu 0.0 den 10.0).";
        return result;
    }

    // 4. Thêm vào mảng RAM
    this->studentsPtr->push_back(stToAdd);
    result.student = stToAdd;

    // 5. Tự động lưu vào file JSON nếu được yêu cầu
    if (autoSave)
    {
        if (!this->saveToFile())
        {
            result.success = true;
            result.message = "Da them sinh vien vao RAM nhung khong the luu vao file JSON.";
            return result;
        }
    }

    result.success = true;
    result.message = "Them sinh vien moi thanh cong.";
    return result;
}

void StudentCRUD::createStudentInteractive()
{
    cout << "====================================================\n";
    cout << "           THEM SINH VIEN MOI (CREATE)              \n";
    cout << "      (Nhan phim ESC bat ky luc nao de huy bo)      \n";
    cout << "====================================================\n";

    Student newStudent;
    newStudent.id = this->generateNextStudentId();

    cout << "MSSV (He thong tu dong tao): " << newStudent.id << "\n";
    cout << "----------------------------------------------------\n";

    // 1. Nhập Họ và tên (Bấm ESC để hủy)
    if (!inputStringWithEsc("Nhap Ho va Ten: ", newStudent.name, false))
    {
        cout << "[Thong bao] Da huy thao tac them sinh vien.\n";
        return;
    }

    // 2. Nhập Mã lớp (Bấm ESC để hủy)
    if (!inputStringWithEsc("Nhap Ma Lop (Vi du: C01, 21DTHD1): ", newStudent.classId, false))
    {
        cout << "[Thong bao] Da huy thao tac them sinh vien.\n";
        return;
    }

    // 3. Nhập Điểm GPA (Bấm ESC để hủy)
    while (true)
    {
        string gpaInput;
        if (!inputStringWithEsc("Nhap Diem GPA (0.0 - 10.0): ", gpaInput, false))
        {
            cout << "[Thong bao] Da huy thao tac them sinh vien.\n";
            return;
        }

        try
        {
            double val = stod(gpaInput);
            if (val >= 0.0 && val <= 10.0)
            {
                newStudent.gpa = val;
                break;
            }
            cout << "[Loi] GPA phai nam trong khoang tu 0.0 den 10.0. Vui long nhap lai!\n";
        }
        catch (...)
        {
            cout << "[Loi] Gia tri GPA phai la so thuc hop le. Vui long nhap lai!\n";
        }
    }

    // Thực hiện thêm vào hệ thống
    CRUDResult result = this->createStudent(newStudent, true);

    cout << "----------------------------------------------------\n";
    if (result.success)
    {
        cout << "[Thanh cong] " << result.message << '\n';
        cout << "  - MSSV   : " << newStudent.id << '\n';
        cout << "  - Ho ten : " << newStudent.name << '\n';
        cout << "  - Lop    : " << newStudent.classId << '\n';
        cout << "  - GPA    : " << fixed << setprecision(2) << newStudent.gpa << '\n';
    }
    else
    {
        cout << "[That bai] " << result.message << '\n';
    }
    cout << "====================================================\n\n";
}

// ============================================================================
// 3. UPDATE (CẬP NHẬT / SỬA THÔNG TIN SINH VIÊN - VÀO THẲNG MŨI TÊN, HỖ TRỢ ESC)
// ============================================================================
CRUDResult StudentCRUD::updateStudent(const string &studentId, const string &newName, const string &newClassId, double newGpa, bool autoSave)
{
    CRUDResult result;

    if (this->studentsPtr == nullptr)
    {
        result.success = false;
        result.message = "Con tro du lieu chua duoc khoi tao.";
        return result;
    }

    int index = this->findIndexById(studentId);
    if (index == -1)
    {
        result.success = false;
        result.message = "Khong tim thay sinh vien voi MSSV: " + studentId;
        return result;
    }

    if (newGpa < 0.0 || newGpa > 10.0)
    {
        result.success = false;
        result.message = "Diem GPA moi khong hop le (0.0 <= GPA <= 10.0).";
        return result;
    }

    // Cập nhật thông tin vào phần tử trong RAM
    Student &target = (*this->studentsPtr)[index];
    if (!newName.empty())
    {
        target.name = newName;
    }
    if (!newClassId.empty())
    {
        target.classId = newClassId;
    }
    target.gpa = newGpa;
    result.student = target;

    // Tự động lưu vào file JSON
    if (autoSave)
    {
        if (!this->saveToFile())
        {
            result.success = true;
            result.message = "Da cap nhat vao RAM nhung khong the luu vao file JSON.";
            return result;
        }
    }

    result.success = true;
    result.message = "Cap nhat thong tin sinh vien thanh cong.";
    return result;
}

void StudentCRUD::updateStudentInteractive()
{
    int selectedIdx = this->selectStudentInteractive("CHON SINH VIEN CAN CHINH SUA");
    if (selectedIdx == -1)
    {
        cout << "[Thong bao] Da huy thao tac sua sinh vien.\n";
        return;
    }

    clearScreen();
    const Student &current = (*this->studentsPtr)[selectedIdx];
    cout << "====================================================\n";
    cout << "          CHINH SUA THONG TIN SINH VIEN             \n";
    cout << "      (Nhan phim ESC bat ky luc nao de huy bo)      \n";
    cout << "====================================================\n";
    cout << "  - MSSV (Co dinh) : " << current.id << '\n';
    cout << "  - Ho ten hien tai: " << current.name << '\n';
    cout << "  - Lop hien tai   : " << current.classId << '\n';
    cout << "  - GPA hien tai   : " << fixed << setprecision(2) << current.gpa << '\n';
    cout << "----------------------------------------------------\n";

    string newName;
    if (!inputStringWithEsc("Nhap Ho ten moi (Enter de giu nguyen, ESC de huy): ", newName, true, current.name))
    {
        cout << "[Thong bao] Da huy thao tac chinh sua.\n";
        return;
    }

    string newClassId;
    if (!inputStringWithEsc("Nhap Ma lop moi (Enter de giu nguyen, ESC de huy): ", newClassId, true, current.classId))
    {
        cout << "[Thong bao] Da huy thao tac chinh sua.\n";
        return;
    }

    double newGpa = current.gpa;
    string gpaInput;
    if (!inputStringWithEsc("Nhap GPA moi (Enter de giu nguyen, ESC de huy): ", gpaInput, true, to_string(current.gpa)))
    {
        cout << "[Thong bao] Da huy thao tac chinh sua.\n";
        return;
    }

    if (!gpaInput.empty())
    {
        try
        {
            double val = stod(gpaInput);
            if (val >= 0.0 && val <= 10.0)
            {
                newGpa = val;
            }
            else
            {
                cout << "[Canh bao] GPA khong hop le. Giu nguyen GPA cu (" << current.gpa << ").\n";
            }
        }
        catch (...)
        {
            cout << "[Canh bao] GPA khong hop le. Giu nguyen GPA cu (" << current.gpa << ").\n";
        }
    }

    CRUDResult updateRes = this->updateStudent(current.id, newName, newClassId, newGpa, true);

    cout << "----------------------------------------------------\n";
    if (updateRes.success)
    {
        cout << "[Thanh cong] " << updateRes.message << '\n';
        cout << "  - MSSV   : " << updateRes.student.id << '\n';
        cout << "  - Ho ten : " << updateRes.student.name << '\n';
        cout << "  - Lop    : " << updateRes.student.classId << '\n';
        cout << "  - GPA    : " << fixed << setprecision(2) << updateRes.student.gpa << '\n';
    }
    else
    {
        cout << "[That bai] " << updateRes.message << '\n';
    }
    cout << "====================================================\n\n";
}

// ============================================================================
// 4. DELETE (XÓA SINH VIÊN - VÀO THẲNG MŨI TÊN, HỖ TRỢ ESC)
// ============================================================================
CRUDResult StudentCRUD::deleteStudentById(const string &studentId, bool autoSave)
{
    CRUDResult result;

    if (this->studentsPtr == nullptr)
    {
        result.success = false;
        result.message = "Con tro du lieu chua duoc khoi tao.";
        return result;
    }

    int index = this->findIndexById(studentId);
    if (index == -1)
    {
        result.success = false;
        result.message = "Khong tim thay sinh vien voi MSSV: " + studentId;
        return result;
    }

    Student deletedStudent = (*this->studentsPtr)[index];
    result.student = deletedStudent;

    // Xóa phần tử tại vị trí index khỏi vector
    this->studentsPtr->erase(this->studentsPtr->begin() + index);

    // Tự động lưu vào file JSON
    if (autoSave)
    {
        if (!this->saveToFile())
        {
            result.success = true;
            result.message = "Da xoa sinh vien khoi RAM nhung khong the luu vao file JSON.";
            return result;
        }
    }

    result.success = true;
    result.message = "Xoa sinh vien thanh cong.";
    return result;
}

void StudentCRUD::deleteStudentInteractive()
{
    int selectedIdx = this->selectStudentInteractive("CHON SINH VIEN CAN XOA");
    if (selectedIdx == -1)
    {
        cout << "[Thong bao] Da huy thao tac xoa sinh vien.\n";
        return;
    }

    clearScreen();
    const Student &st = (*this->studentsPtr)[selectedIdx];
    cout << "====================================================\n";
    cout << "           XAC NHAN XOA SINH VIEN                   \n";
    cout << "====================================================\n";
    cout << "  - MSSV   : " << st.id << '\n';
    cout << "  - Ho ten : " << st.name << '\n';
    cout << "  - Lop    : " << st.classId << '\n';
    cout << "  - GPA    : " << fixed << setprecision(2) << st.gpa << '\n';
    cout << "----------------------------------------------------\n";

    string confirmStr;
    if (!inputStringWithEsc("Ban co chac chan muon xoa sinh vien nay? (y/N - ESC de huy): ", confirmStr, true, "N"))
    {
        cout << "[Huy bo] Da huy thao tac xoa sinh vien.\n";
        return;
    }

    if (confirmStr == "y" || confirmStr == "Y")
    {
        string idToDelete = st.id;
        CRUDResult deleteRes = this->deleteStudentById(idToDelete, true);
        cout << "----------------------------------------------------\n";
        if (deleteRes.success)
        {
            cout << "[Thanh cong] Da xoa sinh vien co MSSV " << idToDelete << " khoi he thong va cap nhat file database.json.\n";
        }
        else
        {
            cout << "[That bai] " << deleteRes.message << '\n';
        }
    }
    else
    {
        cout << "[Huy bo] Da huy thao tac xoa sinh vien.\n";
    }
    cout << "====================================================\n\n";
}

// ============================================================================
// 5. MENU ĐIỀU KHIỂN CRUD TỔNG HỢP
// ============================================================================
void StudentCRUD::runCRUDMenu()
{
    int choice = -1;
    while (choice != 0)
    {
        clearScreen();
        cout << "====================================================\n";
        cout << "     CHUONG TRINH QUAN LY SINH VIEN (PHAT)          \n";
        cout << "====================================================\n";
        cout << "1. Them sinh vien moi (Create - Auto MSSV)\n";
        cout << "2. Cap nhat thong tin sinh vien (Update)\n";
        cout << "3. Xoa sinh vien (Delete)\n";
        cout << "4. Luu du lieu hien tai vao database.json\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "----------------------------------------------------\n";

        string choiceStr;
        if (!inputStringWithEsc("Chon chuc nang [0-4]: ", choiceStr, false))
        {
            break; // Bấm ESC tại menu sẽ quay lại menu chính
        }

        try
        {
            choice = stoi(choiceStr);
        }
        catch (...)
        {
            clearScreen();
            cout << "\n[Loi] Vui long nhap so tu 0 den 4.\n\n";
            pauseScreen();
            continue;
        }

        switch (choice)
        {
        case 1:
            clearScreen();
            this->createStudentInteractive();
            pauseScreen();
            break;
        case 2:
            clearScreen();
            this->updateStudentInteractive();
            pauseScreen();
            break;
        case 3:
            clearScreen();
            this->deleteStudentInteractive();
            pauseScreen();
            break;
        case 4:
            clearScreen();
            if (this->saveToFile())
            {
                cout << "\n[Thanh cong] Da dong bo toan bo " << this->studentsPtr->size() << " sinh vien vao " << this->dbFilePath << "!\n\n";
            }
            pauseScreen();
            break;
        case 0:
            clearScreen();
            cout << "\nQuay lai menu chinh...\n\n";
            break;
        default:
            clearScreen();
            cout << "\n[Loi] Lua chon khong hop le. Vui long chon lai!\n\n";
            pauseScreen();
            break;
        }
    }
}

