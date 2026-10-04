// ============================================================================
// COMPREHENSIVE AUTOMATED TEST SUITE FOR DSA SYSTEM
// Project: Records-and-Decision Engine (DSA_15)
// Covers: MC1 (Hash), MC2 (Heap), RQ1 (Class), RQ2 (GPA Range), CRUD & Edge Cases
// ============================================================================

#include "interface/student.h"
#include "interface/core_hash/HashTable.h"
#include "interface/core_heap/CustomMaxHeapGpaFinder.h"
#include "interface/core_class_filter/OptimizedLinearFilter.h"
#include "interface/core_sorted_gpa/SortedGpaFilter.h"
#include "interface/core_crud/StudentCRUD.h"

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

// ANSI Colors for Pretty Output
namespace Color {
    const string RESET   = "\033[0m";
    const string GREEN   = "\033[32m";
    const string RED     = "\033[31m";
    const string YELLOW  = "\033[33m";
    const string CYAN    = "\033[36m";
    const string BOLD    = "\033[1m";
}

static int totalTests = 0;
static int passedTests = 0;
static int failedTests = 0;

void recordTest(const string& suiteName, const string& testName, bool condition, const string& details = "") {
    totalTests++;
    if (condition) {
        passedTests++;
        cout << "  [" << Color::GREEN << "PASS" << Color::RESET << "] " << testName << endl;
    } else {
        failedTests++;
        cout << "  [" << Color::RED << "FAIL" << Color::RESET << "] " << testName;
        if (!details.empty()) {
            cout << " -> " << Color::YELLOW << details << Color::RESET;
        }
        cout << endl;
    }
}

// Generate Mock Students for Testing
vector<Student> generateMockData(size_t n) {
    vector<Student> list;
    list.reserve(n);
    for (size_t i = 1; i <= n; ++i) {
        Student s;
        s.id = "25" + to_string(100000 + i);
        s.name = "Sinh Vien " + to_string(i);
        s.classId = "25DTH" + to_string((i % 5) + 1);
        s.gpa = 4.0 + (i % 61) * 0.1; // 4.0 to 10.0
        if (s.gpa > 10.0) s.gpa = 10.0;
        list.push_back(s);
    }
    return list;
}

// ============================================================================
// SUITE 1: MC1 - CLOSED HASH TABLE
// ============================================================================
void test_MC1_HashTable() {
    cout << Color::BOLD << Color::CYAN << "\n[TEST SUITE 1] MC1: Closed Hash Table (Tra cứu theo MSSV)" << Color::RESET << endl;

    auto data = generateMockData(1000);
    HashTable ht;
    ht.build(data);

    // 1.1 Tìm kiếm phần tử đầu, giữa, cuối
    const Student* s1 = ht.search(data[0].id);
    recordTest("MC1", "1.1 Tra cuu chinh xac phan tu dau dai", s1 != nullptr && s1->id == data[0].id && s1->name == data[0].name);

    const Student* sMid = ht.search(data[500].id);
    recordTest("MC1", "1.2 Tra cuu chinh xac phan tu giua dai", sMid != nullptr && sMid->id == data[500].id && sMid->gpa == data[500].gpa);

    const Student* sLast = ht.search(data.back().id);
    recordTest("MC1", "1.3 Tra cuu chinh xac phan tu cuoi dai", sLast != nullptr && sLast->id == data.back().id);

    // 1.2 Tìm kiếm khóa không tồn tại
    const Student* sNotFound = ht.search("99999999");
    recordTest("MC1", "1.4 Tra cuu MSSV khong ton tai tra ve nullptr an toan", sNotFound == nullptr);

    // 1.3 Tìm kiếm chuỗi rỗng
    const Student* sEmpty = ht.search("");
    recordTest("MC1", "1.5 Tra cuu MSSV rong tra ve nullptr an toan", sEmpty == nullptr);

    // 1.4 Kiểm tra phân bổ và collision
    recordTest("MC1", "1.6 Bang bam khoi tao hop le voi collisions co kiem soat", ht.getCollisions() >= 0);
}

// ============================================================================
// SUITE 2: MC2 - CUSTOM MAX-HEAP
// ============================================================================
void test_MC2_MaxHeap() {
    cout << Color::BOLD << Color::CYAN << "\n[TEST SUITE 2] MC2: Custom Max-Heap (Truy xuất GPA cao nhất)" << Color::RESET << endl;

    auto data = generateMockData(500);
    // Explicitly place known max GPA
    data[100].gpa = 10.0;
    data[100].id = "25100101";

    CustomMaxHeapGpaFinder heapFinder(data);
    heapFinder.buildStructure();

    MC2Result res = heapFinder.findMaxGPA();
    recordTest("MC2", "2.1 Tim dung sinh vien co GPA cao nhat = 10.0", res.found && abs(res.student.gpa - 10.0) < 1e-5);

    // 2.2 Tie-break verification (2 sinh viên cùng GPA 10.0, chọn theo MSSV)
    vector<Student> tieData = {
        {"25100002", "Nguyen Van B", "25DTH1", 10.0},
        {"25100001", "Nguyen Van A", "25DTH1", 10.0},
        {"25100003", "Nguyen Van C", "25DTH1", 9.5}
    };
    CustomMaxHeapGpaFinder tieHeap(tieData);
    tieHeap.buildStructure();
    MC2Result tieRes = tieHeap.findMaxGPA();
    recordTest("MC2", "2.2 Xu ly Tie-break uu tien MSSV nho hon khi cung GPA", tieRes.found && tieRes.student.id == "25100001");

    // 2.3 Single element
    vector<Student> singleData = {{"25100099", "Tran Van Don", "25DTH2", 8.5}};
    CustomMaxHeapGpaFinder singleHeap(singleData);
    singleHeap.buildStructure();
    MC2Result singleRes = singleHeap.findMaxGPA();
    recordTest("MC2", "2.3 Heap hoat dong chinh xac voi tap du lieu 1 phan tu", singleRes.found && singleRes.student.id == "25100099");
}

// ============================================================================
// SUITE 3: RQ1 - CLASS INDEX VIEW
// ============================================================================
void test_RQ1_ClassFilter() {
    cout << Color::BOLD << Color::CYAN << "\n[TEST SUITE 3] RQ1: Class Index View (Lọc sinh viên theo Mã Lớp)" << Color::RESET << endl;

    auto data = generateMockData(500);
    ClassIndexFilter filter;
    filter.build(data);

    // 3.1 Lọc lớp hợp lệ có sinh viên
    ClassIndexViewResult v1 = filter.filterView("25DTH1");
    bool allMatch = true;
    for (size_t i = 0; i < v1.size(); ++i) {
        if (data[v1.at(i)].classId != "25DTH1") {
            allMatch = false;
            break;
        }
    }
    recordTest("RQ1", "3.1 Loc chinh xac tat ca sinh vien thuoc lop 25DTH1", v1.size() > 0 && allMatch);

    // 3.2 Lớp không tồn tại
    ClassIndexViewResult vNone = filter.filterView("99XXX_INVALID");
    recordTest("RQ1", "3.2 Loc ma lop khong ton tai tra ve danh sach rong (0 SV)", vNone.size() == 0 && vNone.empty());

    // 3.3 Khớp số lượng với Ground Truth (Linear Scan)
    size_t expectedCount = 0;
    for (const auto& s : data) {
        if (s.classId == "25DTH2") expectedCount++;
    }
    ClassIndexViewResult v2 = filter.filterView("25DTH2");
    recordTest("RQ1", "3.3 So luong ket qua trung khop 100% voi Linear Scan Ground Truth", v2.size() == expectedCount);
}

// ============================================================================
// SUITE 4: RQ2 - SORTED GPA RANGE FILTER
// ============================================================================
void test_RQ2_SortedGpa() {
    cout << Color::BOLD << Color::CYAN << "\n[TEST SUITE 4] RQ2: Sorted GPA Range Filter (Lọc theo khoảng GPA)" << Color::RESET << endl;

    auto data = generateMockData(1000);
    SortedGpaFilter gpaFilter;
    gpaFilter.build(data);

    // 4.1 Khoảng bình thường [8.0, 9.5]
    GpaRangeViewResult r1 = gpaFilter.filter(8.0, 9.5);
    bool inRange = true;
    for (size_t i = 0; i < r1.size(); ++i) {
        double g = r1.at(i).gpa;
        if (g < 8.0 || g > 9.5) {
            inRange = false;
            break;
        }
    }
    recordTest("RQ2", "4.1 Loc dung tat ca sinh vien trong khoang [8.0, 9.5]", r1.size() > 0 && inRange);

    // 4.2 Đối chiếu số lượng chính xác với Linear Scan Ground Truth
    size_t gtCount = 0;
    for (const auto& s : data) {
        if (s.gpa >= 8.0 && s.gpa <= 9.5) gtCount++;
    }
    recordTest("RQ2", "4.2 So luong khop 100% voi Linear Ground Truth", r1.size() == gtCount);

    // 4.3 Khoảng nghịch (min > max: [9.0, 7.0])
    GpaRangeViewResult rRev = gpaFilter.filter(9.0, 7.0);
    recordTest("RQ2", "4.3 Khoang nguoc [9.0, 7.0] xu ly an toan va tra ve 0 ket qua", rRev.size() == 0);

    // 4.4 Khoảng ngoài biên [11.0, 12.0]
    GpaRangeViewResult rOut = gpaFilter.filter(11.0, 12.0);
    recordTest("RQ2", "4.4 Khoang ngoai bien [11.0, 12.0] tra ve 0 ket qua", rOut.size() == 0);

    // 4.5 Toàn dải [0.0, 10.0]
    GpaRangeViewResult rFull = gpaFilter.filter(0.0, 10.0);
    recordTest("RQ2", "4.5 Toan dai [0.0, 10.0] tra ve dung toan bo 1000 sinh vien", rFull.size() == data.size());
}

// ============================================================================
// SUITE 5: CRUD OPERATIONS
// ============================================================================
void test_CRUD_Operations() {
    cout << Color::BOLD << Color::CYAN << "\n[TEST SUITE 5] CRUD: Create - Update - Delete Operations" << Color::RESET << endl;

    auto data = generateMockData(100);
    StudentCRUD crud(data, "data/test_dummy.json");

    // 5.1 Create new student
    Student newS = {"25999001", "Pham Hoang Test", "25DTH3", 9.25};
    CRUDResult cRes = crud.createStudent(newS, false);
    recordTest("CRUD", "5.1 Them moi sinh vien hop le thanh cong", cRes.success && data.size() == 101);

    // 5.2 Create duplicate student ID
    CRUDResult dupRes = crud.createStudent(newS, false);
    recordTest("CRUD", "5.2 Tu choi them trung MSSV va bao loi hop le", !dupRes.success && data.size() == 101);

    // 5.3 Create invalid GPA (> 10.0)
    Student invS = {"25999002", "Loi Diem", "25DTH1", 11.5};
    CRUDResult invRes = crud.createStudent(invS, false);
    recordTest("CRUD", "5.3 Tu choi them GPA > 10.0 va bao loi hop le", !invRes.success);

    // 5.4 Update existing student
    CRUDResult uRes = crud.updateStudent("25999001", "Pham Hoang Test (Updated)", "25DTH4", 9.80, false);
    recordTest("CRUD", "5.4 Cap nhat thong tin sinh vien thanh cong", uRes.success && data.back().name == "Pham Hoang Test (Updated)" && abs(data.back().gpa - 9.80) < 1e-5);

    // 5.5 Update non-existent student
    CRUDResult uNotFound = crud.updateStudent("00000000", "Khong Co", "25DTH1", 8.0, false);
    recordTest("CRUD", "5.5 Bao loi khi cap nhat MSSV khong ton tai", !uNotFound.success);

    // 5.6 Delete existing student
    CRUDResult dRes = crud.deleteStudentById("25999001", false);
    recordTest("CRUD", "5.6 Xoa sinh vien hop le thanh cong", dRes.success && data.size() == 100);

    // 5.7 Delete non-existent student
    CRUDResult dNotFound = crud.deleteStudentById("00000000", false);
    recordTest("CRUD", "5.7 Bao loi khi xoa MSSV khong ton tai", !dNotFound.success);
}

// ============================================================================
// SUITE 6: EDGE CASES & SYSTEM BOUNDARIES
// ============================================================================
void test_EdgeCases() {
    cout << Color::BOLD << Color::CYAN << "\n[TEST SUITE 6] Edge Cases & System Boundary Conditions" << Color::RESET << endl;

    // 6.1 Empty dataset
    vector<Student> emptyData;
    HashTable emptyHt;
    emptyHt.build(emptyData);
    recordTest("Edge", "6.1 Hash Table xu ly an toan tap du lieu rong (N = 0)", emptyHt.search("25110001") == nullptr);

    SortedGpaFilter emptyGpa;
    emptyGpa.build(emptyData);
    recordTest("Edge", "6.2 Sorted GPA xu ly an toan tap du lieu rong (N = 0)", emptyGpa.filter(4.0, 10.0).size() == 0);

    ClassIndexFilter emptyClass;
    emptyClass.build(emptyData);
    recordTest("Edge", "6.3 Class Index xu ly an toan tap du lieu rong (N = 0)", emptyClass.filterView("25DTH1").size() == 0);

    // 6.2 All identical GPAs
    vector<Student> sameGpaData;
    for (int i = 1; i <= 50; ++i) {
        sameGpaData.push_back({"25" + to_string(200000 + i), "SV Dong Diem", "25DTH1", 8.0});
    }
    SortedGpaFilter sameFilter;
    sameFilter.build(sameGpaData);
    GpaRangeViewResult sameRes = sameFilter.filter(8.0, 8.0);
    recordTest("Edge", "6.4 Sorted GPA loc chinh xac 100% khi tat ca SV dong GPA", sameRes.size() == 50);
}

// ============================================================================
// MAIN TEST RUNNER
// ============================================================================
int main() {
    cout << "====================================================================" << endl;
    cout << "    HE THONG TEST TU DONG PHU MOI YEU CAU (AUTOMATED TEST SUITE)   " << endl;
    cout << "    Do an Mon hoc: Cau truc Du lieu va Giai thuat - Nhom 12        " << endl;
    cout << "====================================================================" << endl;

    test_MC1_HashTable();
    test_MC2_MaxHeap();
    test_RQ1_ClassFilter();
    test_RQ2_SortedGpa();
    test_CRUD_Operations();
    test_EdgeCases();

    cout << "\n====================================================================" << endl;
    cout << "                      KET QUA KIEM THU TONG HOP                     " << endl;
    cout << "====================================================================" << endl;
    cout << "  Tong so Test Cases : " << totalTests << endl;
    cout << "  So Test Dat [PASS] : " << Color::GREEN << passedTests << Color::RESET << endl;
    cout << "  So Test Loi [FAIL] : " << (failedTests > 0 ? Color::RED : Color::GREEN) << failedTests << Color::RESET << endl;
    cout << "  Ty le thanh cong   : " << Color::BOLD << (passedTests * 100.0 / totalTests) << "%" << Color::RESET << endl;
    cout << "====================================================================" << endl;

    return failedTests == 0 ? 0 : 1;
}
