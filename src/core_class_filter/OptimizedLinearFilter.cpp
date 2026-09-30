#include "../../interface/core_class_filter/OptimizedLinearFilter.h"

ClassIndexFilter::ClassIndexFilter()
    : built(false)
{
}

void ClassIndexFilter::build(const vector<Student> &students)
{
    classIndex.clear();
    classIndex.reserve(64);

    for (int i = 0; i < static_cast<int>(students.size()); ++i)
    {
        classIndex[students[i].classId].push_back(i);
    }

    built = true;
}

ClassIndexViewResult ClassIndexFilter::filterView(const string &classId) const
{
    ClassIndexViewResult result;
    if (!built)
    {
        return result;
    }

    result.comparisons = 1;
    auto it = classIndex.find(classId);
    if (it != classIndex.end())
    {
        result.indexes = &it->second;
    }

    return result;
}

OptimizedFilterResult ClassIndexFilter::filter(const string &classId) const
{
    OptimizedFilterResult result;
    ClassIndexViewResult view = filterView(classId);
    result.comparisons = view.comparisons;
    if (view.indexes != nullptr)
    {
        result.indexes = *view.indexes;
    }
    return result;
}

// Legacy one-shot path: still scans linearly and stores indexes instead of
// copying Student objects. Kept for older console flows and direct tests.
OptimizedFilterResult ClassIndexFilter::filter(
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
