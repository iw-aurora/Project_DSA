#ifndef LINEAR_GPA_FILTER_H
#define LINEAR_GPA_FILTER_H

#include "../student.h"
#include "FindStudentByGpaRange.h"
#include <vector>

using namespace std;

class LinearGpaFilter
{
public:
    static FilterGpaResult filter(const vector<Student> &students, double minGpa, double maxGpa);
};

#endif