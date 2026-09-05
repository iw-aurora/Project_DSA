#ifndef RQ1_FILTER_H
#define RQ1_FILTER_H
#include <string>
#include <vector>
#include "../student.h"
using namespace std;
struct FilterResult
{
    vector<Student> students;
    double buildTimeMs;
    double queryTimeMs;
    double totalTimeMs;
    long long comparisons;
    FilterResult()
    {
        this->students = vector<Student>();
        this->buildTimeMs = 0.0;
        this->queryTimeMs = 0.0;
        this->totalTimeMs = 0.0;
        this->comparisons = 0;
    }
};
class RQ1_Filter
{
private:
    const vector<Student> *studentsPtr;

private:
    void appendStudents(vector<Student> &resultStudents, const Student &newStudents) const
    {
        size_t currentSize = resultStudents.size();
        if (currentSize == resultStudents.capacity())
        {
            size_t newCapacity = currentSize == 0 ? 1 : currentSize * 2;
            resultStudents.reserve(newCapacity);
        }
        resultStudents.resize(currentSize + 1);
        resultStudents[currentSize] = newStudents;
    }

public:
    explicit RQ1_Filter(const vector<Student> &students);
    FilterResult filterBaseline(const string &classId) const;
    void runBenchmark(const string &classId, const FilterResult &result);
};

#endif // RQ1_FILTER_H