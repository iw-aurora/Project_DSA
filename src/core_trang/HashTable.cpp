#include "../../interface/interface_trang/HashTable.h"
using namespace std;

// Khởi tạo một ô trong Hash Table
// student = nullptr: chưa có dữ liệu
// occupied = false: ô trống
HashTable::Entry::Entry()
    : student(nullptr), occupied(false) {}

// Khởi tạo thống kê
HashTable::HashTable()
    : collisions(0), probes(0) {}

// FNV-1a: chuyển MSSV -> giá trị hash -> index trong bảng
size_t HashTable::hash(const std::string& key) const{
    size_t value = 1469598103934665603ULL;
    for (unsigned char c : key){
        value ^= c;
        value *= 1099511628211ULL;
    }
    return value % table.size();
}

// Xây dựng Hash Table
// Dùng Open Addressing + Linear Probing
void HashTable::build(const std::vector<Student>& students){
    // Capacity ≈ 2n + 1 để giảm collision
    const size_t capacity =
        students.empty() ? 1 : students.size() * 2 + 1;
    table.assign(capacity, Entry());
    collisions = 0;
    probes = 0;
    for (const Student& student : students){
        size_t index = hash(student.id);
        while (table[index].occupied){
            ++collisions;
            index = (index + 1) % table.size();
        }
        table[index].student = &student;
        table[index].occupied = true;
    }
}

const Student* HashTable::search(const std::string& targetId){
    probes = 0;
    if (table.empty())
        return nullptr;
    size_t index = hash(targetId);
    for (size_t step = 0; step < table.size(); ++step){
        ++probes;
        if (!table[index].occupied)
            return nullptr;
        if (table[index].student->id == targetId)
            return table[index].student;
        index = (index + 1) % table.size();
    }
    return nullptr;
}

size_t HashTable::getCollisions() const{
    return collisions;
}

size_t HashTable::getProbes() const{
    return probes;
}
