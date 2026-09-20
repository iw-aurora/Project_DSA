#ifndef SORTED_GPA_FILTER_H
#define SORTED_GPA_FILTER_H

#include "../student.h"
#include "../../interface/interface_minhanh/FindStudentByGpaRange.h" 

class SortedGpaFilter
{
public:
    static FilterGpaResult filter(const std::vector<Student> &students, double minGpa, double maxGpa);
};

#endif