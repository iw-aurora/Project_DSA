#ifndef OPTIMIZED_LINEAR_FILTER_H
#define OPTIMIZED_LINEAR_FILTER_H

#include <string>
#include <unordered_map>
#include <vector>
#include "../student.h"

using namespace std;

// ============================================================================
// STRUCT: OptimizedFilterResult
// ============================================================================
// Purpose:
// - Legacy result type that stores matching student indexes instead of
//   copying full Student records.
// - Kept for older call sites that still expect an owned vector<int>.
// ============================================================================

struct OptimizedFilterResult
{
    vector<int> indexes;
    long long comparisons;

    // ------------------------------------------------------------------------
    // Constructor
    // Chức năng:
    // - Khởi tạo số phép so sánh bằng 0.
    // ------------------------------------------------------------------------

    OptimizedFilterResult()
    {
        comparisons = 0;
    }
};

struct ClassIndexViewResult
{
    const vector<int> *indexes;
    long long comparisons;

    ClassIndexViewResult()
    {
        indexes = nullptr;
        comparisons = 0;
    }

    size_t size() const
    {
        return indexes == nullptr ? 0 : indexes->size();
    }

    bool empty() const
    {
        return size() == 0;
    }

    int at(size_t index) const
    {
        return (*indexes)[index];
    }
};

// ============================================================================
// CLASS: OptimizedLinearFilter
// ============================================================================
// Purpose:
// - Final RQ1 data structure.
// - build(): one pass over N students to create classId -> indexes.
// - filterView(): average O(1) lookup that returns a view of the stored
//   index vector, so the query does not copy K Student records or K indexes.
//
// Complexity:
// - Build time : O(N)
// - Build space: O(N)
// - Query time : O(1) average for lookup, O(K) only when the caller iterates
//   the returned indexes to render/list results.
// - Query space: O(1)
// ============================================================================

class OptimizedLinearFilter
{
private:
    unordered_map<string, vector<int>> classIndex;
    bool built;

public:
    OptimizedLinearFilter();

    void build(const vector<Student> &students);
    ClassIndexViewResult filterView(const string &classId) const;
    OptimizedFilterResult filter(const string &classId) const;

    // ------------------------------------------------------------------------
    // Hàm: filter()
    //
    // Legacy one-shot scan. It is intentionally still O(N), but avoids
    // copying Student objects by storing indexes only.
    // ------------------------------------------------------------------------

    static OptimizedFilterResult filter(
        const vector<Student> &students,
        const string &classId);
};

#endif // OPTIMIZED_LINEAR_FILTER_H
