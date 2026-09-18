#include "../../interface/interface_mytra/OptimizedLinearFilter.h"
// Nếu phù hợp, chỉ lưu index thay vì copy toàn bộ Student.
// Điểm cải tiến so với Baseline:
// - Baseline:
//      result.students.push_back(student);
// - Final:
//      result.indexes.push_back(i);
// Nhờ đó Final không cần tạo bản sao Student cho từng kết quả.
OptimizedFilterResult OptimizedLinearFilter::filter(
    const vector<Student>& students,
    const string& classId
)
{
    // Tạo đối tượng lưu kết quả.
    OptimizedFilterResult result;
    // reserve() giúp vector có sẵn vùng nhớ, hạn chế việc cấp phát lại khi thêm nhiều index.
    // Giúp tối ưu về chi phí thực thi,không làm thay đổi độ phức tạp Big-O.
    result.indexes.reserve(students.size());
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