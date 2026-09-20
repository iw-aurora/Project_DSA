#include "../../interface/interface_mytra/OptimizedLinearFilter.h"
// Final Solution lưu index của Student thay vì sao chép
// toàn bộ đối tượng Student vào kết quả.
// Điều này giảm chi phí sao chép dữ liệu nhưng không
// làm thay đổi độ phức tạp của quá trình tìm kiếm.
// Điểm cải tiến so với Baseline:
// - Baseline:
//      result.students.push_back(student);
// - Final:
//      result.indexes.push_back(i);
// Nhờ đó Final không cần tạo bản sao Student cho từng kết quả.
OptimizedFilterResult OptimizedLinearFilter::filter(
    const vector<Student> &students,
    const string &classId)
{
    // Tạo đối tượng lưu kết quả.
    OptimizedFilterResult result;
    for (int i = 0; i < (int)students.size(); i++)
    {
        result.comparisons++;
        if (students[i].classId == classId)
        {
            result.indexes.push_back(i);
        }
    }
    return result;
}