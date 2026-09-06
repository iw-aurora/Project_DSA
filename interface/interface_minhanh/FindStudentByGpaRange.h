#ifndef FIND_STUDENT_BY_GPA_RANGE_H
#define FIND_STUDENT_BY_GPA_RANGE_H

#include <string>
#include <vector>
#include "../student.h"

using namespace std;

struct FilterGpaResult
{
    vector<Student> students;
    double buildTimeMs;
    double queryTimeMs;
    double totalTimeMs;
    long long comparisons;

    FilterGpaResult()
    {
        this->students = vector<Student>();
        this->buildTimeMs = 0.0;
        this->queryTimeMs = 0.0;
        this->totalTimeMs = 0.0;
        this->comparisons = 0;
    }
};

class FindStudentByGpaRange
{
private:
    const vector<Student> *studentsPtr;

public:
    explicit FindStudentByGpaRange(const vector<Student> &students);
    FilterGpaResult filterBaseline(double minGpa, double maxGpa) const;
    void runBenchmark(double minGpa, double maxGpa, const FilterGpaResult &result) const;
};

#endif 