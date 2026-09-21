#include "../../interface/interface_tra/CustomMaxHeapGpaFinder.h"
#include <chrono>

using namespace std::chrono;

// ============================================================
// CONSTRUCTOR
// ============================================================
CustomMaxHeapGpaFinder::CustomMaxHeapGpaFinder(
    const std::vector<Student> &students) {
  this->studentsPtr = &students;
  this->isInitialized = false;
  // Xây dựng Max Heap ngay khi khởi tạo đối tượng (Độ phức tạp O(n))
  buildStructure();
}

// ============================================================
// HÀM SO SÁNH ĐỘ ƯU TIÊN (Xử lý Tie-break)
// ============================================================
bool CustomMaxHeapGpaFinder::higherPriority(const Student &a,
                                            const Student &b) const {
  // 1. GPA cao hơn được ưu tiên lên trên
  if (a.gpa != b.gpa) {
    return a.gpa > b.gpa;
  }
  // 2. Nếu GPA bằng nhau -> MSSV nhỏ hơn được ưu tiên
  return a.id < b.id;
}

// ============================================================
// HEAPIFY UP (Đưa phần tử lên trên khi cần thiết)
// ============================================================
void CustomMaxHeapGpaFinder::heapifyUp(int index) {
  while (index > 0) {
    int parent = (index - 1) / 2;
    if (!higherPriority(maxHeap[index], maxHeap[parent])) {
      break;
    }
    std::swap(maxHeap[index], maxHeap[parent]);
    index = parent;
  }
}

// ============================================================
// HEAPIFY DOWN (Vun đống xuống dưới)
// ============================================================
void CustomMaxHeapGpaFinder::heapifyDown(int index) {
  int size = static_cast<int>(maxHeap.size());
  while (true) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int highest = index;

    if (left < size && higherPriority(maxHeap[left], maxHeap[highest])) {
      highest = left;
    }
    if (right < size && higherPriority(maxHeap[right], maxHeap[highest])) {
      highest = right;
    }

    if (highest == index) {
      break;
    }

    std::swap(maxHeap[index], maxHeap[highest]);
    index = highest;
  }
}

// ============================================================
// XÂY DỰNG CUSTOM MAX HEAP
// ============================================================
void CustomMaxHeapGpaFinder::buildStructure() {
  maxHeap.clear();
  if (studentsPtr == nullptr || studentsPtr->empty()) {
    isInitialized = true;
    return;
  }

  // Sao chép dữ liệu từ nguồn
  maxHeap = *studentsPtr;

  // Thuật toán Floyd xây dựng heap từ dưới lên: O(n)
  int startIndex = static_cast<int>(maxHeap.size() / 2) - 1;
  for (int i = startIndex; i >= 0; i--) {
    heapifyDown(i);
  }

  isInitialized = true;
}

// ============================================================
// TÌM GPA CAO NHẤT (Truy xuất O(1) từ đỉnh heap)
// ============================================================
MC2Result CustomMaxHeapGpaFinder::findMaxGPA() const {
  MC2Result result;
  if (!isInitialized || maxHeap.empty()) {
    result.found = false;
    result.comparisons = 0;
    result.queryTimeMs = 0;
    return result;
  }

  // Bắt đầu đo thời gian truy vấn
  auto start = high_resolution_clock::now();

  // Lấy trực tiếp phần tử ở đỉnh heap (index 0) - O(1)
  result.found = true;
  result.student = maxHeap[0];
  result.comparisons = 0;

  // Kết thúc đo thời gian
  auto end = high_resolution_clock::now();
  result.queryTimeMs = duration<double, std::milli>(end - start).count();

  return result;
}

// ============================================================
// CẬP NHẬT HOẶC THÊM MỚI SINH VIÊN
// ============================================================
void CustomMaxHeapGpaFinder::updateOrInsert(const Student &student) {
  if (!isInitialized) {
    buildStructure();
  }

  // Kiểm tra xem sinh viên đã tồn tại chưa để cập nhật
  for (int i = 0; i < static_cast<int>(maxHeap.size()); i++) {
    if (maxHeap[i].id == student.id) {
      maxHeap[i] = student;
      if (i > 0 && higherPriority(maxHeap[i], maxHeap[(i - 1) / 2])) {
        // Nếu phần tử mới ưu tiên hơn cha, đẩy lên.
        heapifyUp(i);
      } else {
        // Nếu không cần đẩy lên, kiểm tra và đẩy xuống nếu cần.
        heapifyDown(i);
      }
    return;
    }
  }

  // Nếu chưa tồn tại thì thêm mới vào cuối rồi vun đống lên
  maxHeap.push_back(student);
  heapifyUp(static_cast<int>(maxHeap.size()) - 1);
}