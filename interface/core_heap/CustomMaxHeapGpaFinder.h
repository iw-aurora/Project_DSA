#ifndef CUSTOM_MAX_HEAP_GPA_FINDER_H
#define CUSTOM_MAX_HEAP_GPA_FINDER_H

#include "../student.h"
#include "LinearMaxScanGpaFinder.h"
#include <vector>

using namespace std;

class CustomMaxHeapGpaFinder {
private:
  const vector<Student> *studentsPtr;
  vector<Student> maxHeap;
  bool isInitialized;

  bool higherPriority(const Student &a, const Student &b) const;
  void heapifyUp(int index);
  void heapifyDown(int index);

public:
  explicit CustomMaxHeapGpaFinder(const vector<Student> &students);
  void buildStructure();
  MC2Result findMaxGPA() const;
  void updateOrInsert(const Student &student);
};

#endif