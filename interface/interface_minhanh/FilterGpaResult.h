#ifndef FILTER_GPA_RESULT_H
#define FILTER_GPA_RESULT_H

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

struct GpaRangeViewResult
{
    const vector<const Student *> *sortedStudents;
    size_t startIndex;
    size_t endIndex;
    double buildTimeMs;
    double queryTimeMs;
    double totalTimeMs;
    long long comparisons;

    GpaRangeViewResult()
    {
        this->sortedStudents = nullptr;
        this->startIndex = 0;
        this->endIndex = 0;
        this->buildTimeMs = 0.0;
        this->queryTimeMs = 0.0;
        this->totalTimeMs = 0.0;
        this->comparisons = 0;
    }

    size_t size() const
    {
        return endIndex >= startIndex ? endIndex - startIndex : 0;
    }

    bool empty() const
    {
        return size() == 0;
    }

    const Student &at(size_t index) const
    {
        return *(*sortedStudents)[startIndex + index];
    }
};

#endif
