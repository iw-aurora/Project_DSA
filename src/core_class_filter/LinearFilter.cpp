#include "../../interface/core_class_filter/LinearFilter.h"
LinearFilterResult LinearFilter::filter(
    const vector<Student>& students,
    const string& classId
)
{
    LinearFilterResult result;
    for (const Student& student : students)
    {
        result.comparisons++;
        if (student.classId == classId)
        {
            result.students.push_back(student);
        }
    }
    return result;
}