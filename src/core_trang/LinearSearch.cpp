#include "../../interface/interface_trang/LinearSearch.h"
using namespace std;

LinearSearch::LinearSearch() : comparisons(0) {}

const Student* LinearSearch::search(const vector<Student>& students,const string& targetId){
    comparisons = 0;
    for (const Student& student : students){
        ++comparisons;
        if (student.id == targetId)
            return &student;
    }
    return nullptr;
}

// Trả về số lần so sánh của lần tìm kiếm gần nhất
size_t LinearSearch::getComparisons() const{
    return comparisons;
}
