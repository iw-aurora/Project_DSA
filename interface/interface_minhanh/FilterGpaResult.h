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

#endif