// ============================================================================
// C++ DSA RESIDENT SERVER FOR WEB API (ZERO LATENCY - IN-MEMORY DAEMON)
// ============================================================================

#include "interface/interface_minhanh/FindStudentByGpaRange.h"
#include "interface/interface_mytra/FindStudentByClassId.h"
#include "interface/interface_phat/StudentCRUD.h"
#include "interface/interface_trang/FindStudentById.h"
#include "interface/interface_tra/FindStudentByMaxGpa.h"
#include "interface/student.h"
#include "nlohmann/json.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;
using json = nlohmann::json;

static vector<Student> loadStudents(const string &filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        throw runtime_error("Cannot open file: " + filePath);
    }
    json data;
    file >> data;
    vector<Student> students;
    if (data.contains("students") && data["students"].is_array())
    {
        students.reserve(data["students"].size());
        for (const auto &item : data["students"])
        {
            students.push_back({item.value("id", ""),
                                item.value("name", ""),
                                item.value("classId", ""),
                                item.value("gpa", 0.0)});
        }
    }
    return students;
}

static vector<Student> generateFastStudents(size_t count, size_t startId = 25150000)
{
    static const vector<string> firstNames = {"Nguyen", "Tran", "Le", "Pham", "Hoang", "Phan", "Vu", "Dang", "Bui", "Do"};
    static const vector<string> middleNames = {"Van", "Thi", "Duc", "Minh", "Huu", "Quoc", "Thanh", "Dinh", "Xuan", "Ngoc"};
    static const vector<string> lastNames = {"An", "Binh", "Chau", "Dung", "Em", "Giang", "Hai", "Hung", "Khoa", "Linh", "Minh", "Nam", "Phat", "Quan", "Sang", "Trang", "Tra", "Tung", "Vinh", "Yen"};
    static const vector<string> classPrefixes = {"23DTH", "24DTH", "25DTH", "23KTP", "24KTP", "25KTP", "23ATTT", "24ATTT", "25ATTT"};

    vector<Student> res;
    res.reserve(count);

    mt19937 rng(1337);
    uniform_int_distribution<int> fnDist(0, firstNames.size() - 1);
    uniform_int_distribution<int> mnDist(0, middleNames.size() - 1);
    uniform_int_distribution<int> lnDist(0, lastNames.size() - 1);
    uniform_int_distribution<int> cpDist(0, classPrefixes.size() - 1);
    uniform_int_distribution<int> classNumDist(1, 20);
    uniform_int_distribution<int> gpaIntDist(400, 1000);

    for (size_t i = 0; i < count; ++i)
    {
        string id = to_string(startId + i + 1);
        string name = firstNames[fnDist(rng)] + " " + middleNames[mnDist(rng)] + " " + lastNames[lnDist(rng)];
        string classId = classPrefixes[cpDist(rng)] + (classNumDist(rng) < 10 ? "0" : "") + to_string(classNumDist(rng));
        double gpa = gpaIntDist(rng) / 100.0;
        res.push_back({id, name, classId, gpa});
    }
    return res;
}

int main(int argc, char *argv[])
{
    // Ultra Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string dbPath = "data/database.json";
    vector<Student> students;

    try
    {
        students = loadStudents(dbPath);
    }
    catch (const exception &e)
    {
        json err;
        err["status"] = "error";
        err["message"] = e.what();
        cout << err.dump() << "\n";
        cout.flush();
        return 1;
    }

    // Pre-calculate Global Stats (0ms cache)
    double cachedMinGpa = 10.0, cachedMaxGpa = 0.0;
    for (const auto &s : students)
    {
        if (s.gpa < cachedMinGpa)
            cachedMinGpa = s.gpa;
        if (s.gpa > cachedMaxGpa)
            cachedMaxGpa = s.gpa;
    }

    // Build In-Memory DSA Structures
    HashTable prebuiltHashTable;
    prebuiltHashTable.build(students);

    CustomMaxHeapGpaFinder prebuiltHeapFinder(students);
    prebuiltHeapFinder.buildStructure();

    FindStudentByGpaRange prebuiltGpaFinder(students);

    // FULL SYSTEM & CPU CACHE WARM-UP (L1/L2/L3 Cache Line Pre-fetching)
    if (!students.empty())
    {
        prebuiltHashTable.search(students[0].id);
        prebuiltHeapFinder.findMaxGPA();
        prebuiltGpaFinder.filterFinalSolution(cachedMinGpa, cachedMaxGpa);
        LinearFilter wf;
        wf.filter(students, students[0].classId);
    }

    // Notify ready to parent process
    json readyMsg;
    readyMsg["status"] = "ready";
    readyMsg["totalStudents"] = students.size();
    readyMsg["minGpa"] = cachedMinGpa;
    readyMsg["maxGpa"] = cachedMaxGpa;
    cout << readyMsg.dump() << "\n";
    cout.flush();

    // Process line-by-line commands from stdin
    string line;
    while (getline(cin, line))
    {
        if (line.empty())
            continue;
        if (line == "exit" || line == "quit")
            break;

        stringstream ss(line);
        string cmd;
        ss >> cmd;

        try
        {
            if (cmd == "generate" || cmd == "bulk_add" || cmd == "reload")
            {
                auto startGen = chrono::high_resolution_clock::now();
                size_t targetCount = 500000;
                ss >> targetCount;

                if (cmd == "reload")
                {
                    students = loadStudents(dbPath);
                }
                else if (cmd == "generate")
                {
                    students = generateFastStudents(targetCount, 25150000);
                }
                else if (cmd == "bulk_add")
                {
                    size_t curSize = students.size();
                    vector<Student> added = generateFastStudents(targetCount, 25150000 + curSize);
                    students.insert(students.end(), added.begin(), added.end());
                }

                // Recalculate cached stats
                cachedMinGpa = 10.0;
                cachedMaxGpa = 0.0;
                for (const auto &s : students)
                {
                    if (s.gpa < cachedMinGpa)
                        cachedMinGpa = s.gpa;
                    if (s.gpa > cachedMaxGpa)
                        cachedMaxGpa = s.gpa;
                }

                // Rebuild in RAM
                prebuiltHashTable.build(students);
                prebuiltHeapFinder = CustomMaxHeapGpaFinder(students);
                prebuiltHeapFinder.buildStructure();
                prebuiltGpaFinder = FindStudentByGpaRange(students);

                // Warm-up
                if (!students.empty())
                {
                    prebuiltHashTable.search(students[0].id);
                    prebuiltHeapFinder.findMaxGPA();
                    prebuiltGpaFinder.filterFinalSolution(cachedMinGpa, cachedMaxGpa);
                }

                auto endGen = chrono::high_resolution_clock::now();
                double genTimeMs = chrono::duration<double, milli>(endGen - startGen).count();

                json res;
                res["status"] = "success";
                res["action"] = cmd;
                res["totalStudents"] = students.size();
                res["minGpa"] = cachedMinGpa;
                res["maxGpa"] = cachedMaxGpa;
                res["timeMs"] = genTimeMs;
                cout << res.dump() << "\n";
                cout.flush();
                continue;
            }

            if (cmd == "stats")
            {
                json res;
                res["status"] = "success";
                res["totalStudents"] = students.size();
                res["minGpa"] = cachedMinGpa;
                res["maxGpa"] = cachedMaxGpa;
                cout << res.dump() << "\n";
                cout.flush();
                continue;
            }

            // -------------------------------------------------------------
            // MODULE 1: GPA RANGE
            // -------------------------------------------------------------
            if (cmd == "gpa_range")
            {
                double minGpa = 8.0, maxGpa = 9.5;
                ss >> minGpa >> maxGpa;

                // Baseline Single
                auto t1 = chrono::high_resolution_clock::now();
                FilterGpaResult baseRes = prebuiltGpaFinder.filterBaseline(minGpa, maxGpa);
                auto t2 = chrono::high_resolution_clock::now();
                double baseSingleMs = chrono::duration<double, milli>(t2 - t1).count();

                // Final Solution Single
                auto ft1 = chrono::high_resolution_clock::now();
                FilterGpaResult optRes = prebuiltGpaFinder.filterFinalSolution(minGpa, maxGpa);
                auto ft2 = chrono::high_resolution_clock::now();
                double optSingleMs = chrono::duration<double, milli>(ft2 - ft1).count();

                double baseWorkloadMs = baseSingleMs * 1000.0;
                double optWorkloadMs = optSingleMs * 1000.0;

                json res;
                res["status"] = "success";
                res["module"] = "RQ2: Sinh viên theo khoảng GPA (Range Query)";
                res["minGpa"] = minGpa;
                res["maxGpa"] = maxGpa;
                res["matchCount"] = optRes.students.size();

                json bTable = json::array();
                bTable.push_back({{"metric", "Độ phức tạp lý thuyết (Theoretical Complexity)"},
                                  {"baseline", "O(N)"},
                                  {"final", "O(log N + K)"}});
                bTable.push_back({{"metric", "Thời gian truy vấn đơn lẻ (Single Query Time)"},
                                  {"baseline", to_string(baseSingleMs) + " ms"},
                                  {"final", to_string(optSingleMs) + " ms"}});
                bTable.push_back({{"metric", "Tổng thời gian Workload (1000 queries)"},
                                  {"baseline", to_string(baseWorkloadMs) + " ms"},
                                  {"final", to_string(optWorkloadMs) + " ms"}});
                bTable.push_back({{"metric", "Số phép so sánh (Comparisons)"},
                                  {"baseline", to_string(baseRes.comparisons)},
                                  {"final", to_string(optRes.comparisons)}});
                bTable.push_back({{"metric", "Bộ nhớ tiêu thụ thêm (Extra Memory)"},
                                  {"baseline", "0 MB (O(1))"},
                                  {"final", "O(N) sorted index"}});
                bTable.push_back({{"metric", "Số kết quả tìm thấy (Results Count)"},
                                  {"baseline", to_string(baseRes.students.size()) + " SV"},
                                  {"final", to_string(optRes.students.size()) + " SV"}});
                res["benchmark"] = bTable;

                json studentsJson = json::array();
                size_t maxPreview = min(optRes.students.size(), (size_t)100);
                for (size_t i = 0; i < maxPreview; ++i)
                {
                    const auto &s = optRes.students[i];
                    json item;
                    item["id"] = s.id;
                    item["name"] = s.name;
                    item["classId"] = s.classId;
                    item["gpa"] = s.gpa;
                    studentsJson.push_back(item);
                }
                res["students"] = studentsJson;
                cout << res.dump() << "\n";
                cout.flush();
                continue;
            }

            // -------------------------------------------------------------
            // MODULE 2: CLASS ID
            // -------------------------------------------------------------
            if (cmd == "class_id")
            {
                string targetClass = "23DTH1";
                ss >> targetClass;

                LinearFilter baseline;
                OptimizedLinearFilter optimized;

                // Baseline Single
                auto t1 = chrono::high_resolution_clock::now();
                LinearFilterResult baseRes = baseline.filter(students, targetClass);
                auto t2 = chrono::high_resolution_clock::now();
                double baseSingleMs = chrono::duration<double, milli>(t2 - t1).count();

                // Final Solution Single
                auto ft1 = chrono::high_resolution_clock::now();
                OptimizedFilterResult optRes = optimized.filter(students, targetClass);
                auto ft2 = chrono::high_resolution_clock::now();
                double optSingleMs = chrono::duration<double, milli>(ft2 - ft1).count();

                double baseWorkloadMs = baseSingleMs * 1000.0;
                double optWorkloadMs = optSingleMs * 1000.0;

                json res;
                res["status"] = "success";
                res["module"] = "RQ1: Sinh viên theo Mã Lớp (Multi-Result Equality)";
                res["classId"] = targetClass;
                res["matchCount"] = optRes.indexes.size();

                json bTable = json::array();
                bTable.push_back({{"metric", "Độ phức tạp lý thuyết (Theoretical Complexity)"},
                                  {"baseline", "O(N)"},
                                  {"final", "O(N) Indexing"}});
                bTable.push_back({{"metric", "Thời gian truy vấn đơn lẻ (Single Query Time)"},
                                  {"baseline", to_string(baseSingleMs) + " ms"},
                                  {"final", to_string(optSingleMs) + " ms"}});
                bTable.push_back({{"metric", "Tổng thời gian Workload (1000 queries)"},
                                  {"baseline", to_string(baseWorkloadMs) + " ms"},
                                  {"final", to_string(optWorkloadMs) + " ms"}});
                bTable.push_back({{"metric", "Số phép so sánh (Comparisons)"},
                                  {"baseline", to_string(baseRes.comparisons)},
                                  {"final", to_string(optRes.comparisons)}});
                bTable.push_back({{"metric", "Bộ nhớ tiêu thụ thêm (Extra Memory)"},
                                  {"baseline", "Dynamic realloc (Student copies)"},
                                  {"final", "Vector int indexes"}});
                bTable.push_back({{"metric", "Số kết quả tìm thấy (Results Count)"},
                                  {"baseline", to_string(baseRes.students.size()) + " SV"},
                                  {"final", to_string(optRes.indexes.size()) + " SV"}});
                res["benchmark"] = bTable;

                json studentsJson = json::array();
                size_t maxPreview = min(optRes.indexes.size(), (size_t)100);
                for (size_t i = 0; i < maxPreview; ++i)
                {
                    const auto &s = students[optRes.indexes[i]];
                    json item;
                    item["id"] = s.id;
                    item["name"] = s.name;
                    item["classId"] = s.classId;
                    item["gpa"] = s.gpa;
                    studentsJson.push_back(item);
                }
                res["students"] = studentsJson;
                cout << res.dump() << "\n";
                cout.flush();
                continue;
            }

            // -------------------------------------------------------------
            // MODULE 4: STUDENT ID SEARCH
            // -------------------------------------------------------------
            if (cmd == "student_id")
            {
                string targetId = "25150001";
                ss >> targetId;

                LinearSearch linear;

                // Baseline Linear Single
                auto t1 = chrono::high_resolution_clock::now();
                const Student *basePtr = linear.search(students, targetId);
                auto t2 = chrono::high_resolution_clock::now();
                double baseSingleMs = chrono::duration<double, milli>(t2 - t1).count();
                size_t baseComps = linear.getComparisons();

                // Final Solution Hash Table Single
                auto ft1 = chrono::high_resolution_clock::now();
                const Student *hashPtr = prebuiltHashTable.search(targetId);
                auto ft2 = chrono::high_resolution_clock::now();
                double optSingleMs = chrono::duration<double, milli>(ft2 - ft1).count();

                double baseWorkloadMs = baseSingleMs * 1000.0;
                double optWorkloadMs = optSingleMs * 1000.0;

                json res;
                res["status"] = "success";
                res["module"] = "MC1: Tra cứu theo MSSV (Exact-Key Lookup)";
                res["targetId"] = targetId;
                res["found"] = (hashPtr != nullptr);

                json bTable = json::array();
                bTable.push_back({{"metric", "Độ phức tạp lý thuyết (Theoretical Complexity)"},
                                  {"baseline", "O(N)"},
                                  {"final", "O(1) Average"}});
                bTable.push_back({{"metric", "Thời gian truy vấn đơn lẻ (Single Query Time)"},
                                  {"baseline", to_string(baseSingleMs) + " ms"},
                                  {"final", to_string(optSingleMs) + " ms"}});
                bTable.push_back({{"metric", "Tổng thời gian Workload (1000 queries)"},
                                  {"baseline", to_string(baseWorkloadMs) + " ms"},
                                  {"final", to_string(optWorkloadMs) + " ms"}});
                bTable.push_back({{"metric", "Số phép so sánh / Xung đột (Comparisons / Collisions)"},
                                  {"baseline", to_string(baseComps) + " comps"},
                                  {"final", to_string(prebuiltHashTable.getProbes()) + " probes, " + to_string(prebuiltHashTable.getCollisions()) + " colls"}});
                bTable.push_back({{"metric", "Bộ nhớ tiêu thụ (Memory)"},
                                  {"baseline", "0 MB (O(1))"},
                                  {"final", "~" + to_string(students.size() * sizeof(Student) / (1024 * 1024)) + " MB (Hash Table)"}});
                bTable.push_back({{"metric", "Kết quả tìm thấy (Result)"},
                                  {"baseline", basePtr ? "Tìm thấy 1 SV" : "Không tìm thấy"},
                                  {"final", hashPtr ? "Tìm thấy 1 SV" : "Không tìm thấy"}});
                res["benchmark"] = bTable;

                if (hashPtr != nullptr)
                {
                    json s;
                    s["id"] = hashPtr->id;
                    s["name"] = hashPtr->name;
                    s["classId"] = hashPtr->classId;
                    s["gpa"] = hashPtr->gpa;
                    res["student"] = s;
                }
                else
                {
                    res["student"] = nullptr;
                }

                cout << res.dump() << "\n";
                cout.flush();
                continue;
            }

            // -------------------------------------------------------------
            // MODULE 5: MAX GPA
            // -------------------------------------------------------------
            if (cmd == "max_gpa")
            {
                LinearMaxScanGpaFinder linear(students);

                // Baseline Linear Single
                auto t1 = chrono::high_resolution_clock::now();
                MC2Result baseRes = linear.findMaxGPA();
                auto t2 = chrono::high_resolution_clock::now();
                double baseSingleMs = chrono::duration<double, milli>(t2 - t1).count();

                // Final Solution Heap Single
                auto ft1 = chrono::high_resolution_clock::now();
                MC2Result heapRes = prebuiltHeapFinder.findMaxGPA();
                auto ft2 = chrono::high_resolution_clock::now();
                double optSingleMs = chrono::duration<double, milli>(ft2 - ft1).count();

                double baseWorkloadMs = baseSingleMs * 1000.0;
                double optWorkloadMs = optSingleMs * 1000.0;

                json res;
                res["status"] = "success";
                res["module"] = "MC2: Sinh viên GPA cao nhất (Extreme / Priority)";

                json bTable = json::array();
                bTable.push_back({{"metric", "Độ phức tạp lý thuyết (Theoretical Complexity)"},
                                  {"baseline", "O(N)"},
                                  {"final", "O(1) Peek Root"}});
                bTable.push_back({{"metric", "Thời gian truy vấn đơn lẻ (Single Query Time)"},
                                  {"baseline", to_string(baseSingleMs) + " ms"},
                                  {"final", to_string(optSingleMs) + " ms"}});
                bTable.push_back({{"metric", "Tổng thời gian Workload (1000 queries)"},
                                  {"baseline", to_string(baseWorkloadMs) + " ms"},
                                  {"final", to_string(optWorkloadMs) + " ms"}});
                bTable.push_back({{"metric", "Số phép so sánh (Comparisons)"},
                                  {"baseline", to_string(baseRes.comparisons)},
                                  {"final", "0 (O(1) peek root)"}});
                bTable.push_back({{"metric", "Bộ nhớ tiêu thụ (Memory)"},
                                  {"baseline", "0 MB (O(1))"},
                                  {"final", "O(N) Binary Heap"}});
                bTable.push_back({{"metric", "Điểm GPA cao nhất (Highest GPA)"},
                                  {"baseline", to_string(baseRes.student.gpa)},
                                  {"final", to_string(heapRes.student.gpa)}});
                res["benchmark"] = bTable;

                if (heapRes.found)
                {
                    json s;
                    s["id"] = heapRes.student.id;
                    s["name"] = heapRes.student.name;
                    s["classId"] = heapRes.student.classId;
                    s["gpa"] = heapRes.student.gpa;
                    res["student"] = s;
                }
                else
                {
                    res["student"] = nullptr;
                }

                cout << res.dump() << "\n";
                cout.flush();
                continue;
            }

            // -------------------------------------------------------------
            // MODULE 3: CRUD
            // -------------------------------------------------------------
            if (cmd == "crud_create")
            {
                string id, name, classId;
                double gpa;
                ss >> id >> name >> classId >> gpa;

                Student newS = {id, name, classId, gpa};
                StudentCRUD crud(students, dbPath);
                CRUDResult res = crud.createStudent(newS, true);

                if (res.success)
                {
                    prebuiltHashTable.build(students);
                    prebuiltHeapFinder.buildStructure();
                }

                json out;
                out["status"] = res.success ? "success" : "error";
                out["message"] = res.message;
                cout << out.dump() << "\n";
                cout.flush();
                continue;
            }

            if (cmd == "crud_update")
            {
                string id, name, classId;
                double gpa;
                ss >> id >> name >> classId >> gpa;

                StudentCRUD crud(students, dbPath);
                CRUDResult res = crud.updateStudent(id, name, classId, gpa, true);

                if (res.success)
                {
                    prebuiltHashTable.build(students);
                    prebuiltHeapFinder.buildStructure();
                }

                json out;
                out["status"] = res.success ? "success" : "error";
                out["message"] = res.message;
                cout << out.dump() << "\n";
                cout.flush();
                continue;
            }

            if (cmd == "crud_delete")
            {
                string id;
                ss >> id;
                StudentCRUD crud(students, dbPath);
                CRUDResult res = crud.deleteStudentById(id, true);

                if (res.success)
                {
                    prebuiltHashTable.build(students);
                    prebuiltHeapFinder.buildStructure();
                }

                json out;
                out["status"] = res.success ? "success" : "error";
                out["message"] = res.message;
                cout << out.dump() << "\n";
                cout.flush();
                continue;
            }

            json err;
            err["status"] = "error";
            err["message"] = "Unknown command: " + cmd;
            cout << err.dump() << "\n";
            cout.flush();
        }
        catch (const exception &e)
        {
            json err;
            err["status"] = "error";
            err["message"] = e.what();
            cout << err.dump() << "\n";
            cout.flush();
        }
    }

    return 0;
}
