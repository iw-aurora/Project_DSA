#ifndef LINEAR_GPA_FILTER_H
#define LINEAR_GPA_FILTER_H

#include "../student.h"
#include "../../interface/interface_minhanh/FindStudentByGpaRange.h"

class LinearGpaFilter
{
public:
    static FilterGpaResult filter(const std::vector<Student> &students,  double minGpa, double maxGpa);
};

#endif