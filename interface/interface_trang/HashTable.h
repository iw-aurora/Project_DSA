#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "../student.h"
#include <cstddef>
#include <string>
#include <vector>

using namespace std;

class HashTable
{
private:
    // Một ô trong bảng hash, lưu con trỏ đến sinh viên và trạng thái đã sử dụng
    struct Entry {
        const Student* student; // Con trỏ đến sinh viên được lưu trữ
        bool occupied; // Đánh dấu ô đã được sử dụng hay chưa
        Entry();
    };
    vector<Entry> table;
    size_t collisions;
    size_t probes;
    // Tính vị trí ban đầu của key trong bảng hash
    size_t hash(const string& key) const;
public:
    HashTable();
    // Xây dựng bảng hash từ danh sách sinh viên
    void build(const vector<Student>& students);
     // Tìm sinh viên theo MSSV
    const Student* search(const string& targetId);
    size_t getCollisions() const;
    size_t getProbes() const;
};

#endif
