#ifndef SORTED_GPA_FILTER_H
#define SORTED_GPA_FILTER_H

#include <vector>
#include "../student.h"
#include "FilterGpaResult.h"

using namespace std;

class SortedGpaFilter
{
private:
    vector<Student> sortedStudents;

public:
    void build(const vector<Student>& students);

    FilterGpaResult filter(double minGpa, double maxGpa);
};

#endif