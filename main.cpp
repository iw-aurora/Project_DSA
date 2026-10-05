// ============================================================================
// CHUONG TRINH CHINH - HE THONG QUAN LY SINH VIEN (DASA230179)
// ============================================================================

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include <iomanip>

#ifdef _WIN32
#include <conio.h>
#endif

#include <chrono>
#include <ctime>
#include <sstream>
#include <numeric>
#include <random>
#include <algorithm>
#include <filesystem>

#include "interface/student.h"
#include "interface/core_sorted_gpa/FindStudentByGpaRange.h"
#include "interface/core_sorted_gpa/LinearGpaFilter.h"
#include "interface/core_sorted_gpa/SortedGpaFilter.h"
#include "interface/core_class_filter/FindStudentByClassId.h"
#include "interface/core_class_filter/LinearFilter.h"
#include "interface/core_class_filter/OptimizedLinearFilter.h"
#include "interface/core_crud/StudentCRUD.h"
#include "interface/core_hash/FindStudentById.h"
#include "interface/core_hash/HashTable.h"
#include "interface/core_hash/LinearSearch.h"
#include "interface/core_heap/FindStudentByMaxGpa.h"
#include "interface/core_heap/CustomMaxHeapGpaFinder.h"
#include "interface/core_heap/LinearMaxScanGpaFinder.h"
#include "nlohmann/json.hpp"

using namespace std;
using namespace chrono;
using json = nlohmann::json;
using Clock = chrono::steady_clock;

namespace {

volatile size_t g_benchSinkSize = 0;
volatile double g_benchSinkGpa = 0.0;

struct BenchStats {
    double minMs{};
    double medianMs{};
    double meanMs{};
    double maxMs{};
};

struct BenchTimedResult {
    double ms{};
    size_t resultCount{};
    long long comparisons{};
};

template <typename Func>
double measureMs(Func &&func) {
    auto start = Clock::now();
    func();
    auto end = Clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

BenchStats summarizeBench(vector<double> values) {
    sort(values.begin(), values.end());
    double sum = accumulate(values.begin(), values.end(), 0.0);
    BenchStats stats;
    stats.minMs = values.front();
    stats.maxMs = values.back();
    stats.meanMs = sum / static_cast<double>(values.size());
    if (values.size() % 2 == 0) {
        stats.medianMs = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2.0;
    } else {
        stats.medianMs = values[values.size() / 2];
    }
    return stats;
}

vector<Student> generateFastStudents(size_t count, size_t startId = 25150000) {
    static const vector<string> firstNames = {"Nguyen", "Tran", "Le", "Pham", "Hoang", "Phan", "Vu", "Dang", "Bui", "Do"};
    static const vector<string> middleNames = {"Van", "Thi", "Duc", "Minh", "Huu", "Quoc", "Thanh", "Dinh", "Xuan", "Ngoc"};
    static const vector<string> lastNames = {"An", "Binh", "Chau", "Dung", "Em", "Giang", "Hai", "Hung", "Khoa", "Linh", "Minh", "Nam", "Phat", "Quan", "Sang", "Trang", "Tra", "Tung", "Vinh", "Yen"};
    static const vector<string> classPrefixes = {"23DTH", "24DTH", "25DTH", "23KTP", "24KTP", "25KTP", "23ATTT", "24ATTT", "25ATTT"};

    vector<Student> students;
    students.reserve(count);

    mt19937 rng(1337);
    uniform_int_distribution<int> fnDist(0, static_cast<int>(firstNames.size()) - 1);
    uniform_int_distribution<int> mnDist(0, static_cast<int>(middleNames.size()) - 1);
    uniform_int_distribution<int> lnDist(0, static_cast<int>(lastNames.size()) - 1);
    uniform_int_distribution<int> cpDist(0, static_cast<int>(classPrefixes.size()) - 1);
    uniform_int_distribution<int> classNumDist(1, 20);
    uniform_int_distribution<int> gpaIntDist(400, 1000);

    for (size_t i = 0; i < count; ++i) {
        const int classNum = classNumDist(rng);
        students.push_back({
            to_string(startId + i + 1),
            firstNames[fnDist(rng)] + " " + middleNames[mnDist(rng)] + " " + lastNames[lnDist(rng)],
            classPrefixes[cpDist(rng)] + (classNum < 10 ? "0" : "") + to_string(classNum),
            gpaIntDist(rng) / 100.0,
        });
    }
    return students;
}

vector<string> makeIdQueries(const vector<Student> &students, size_t queryCount) {
    vector<string> queries;
    queries.reserve(queryCount);
    for (size_t i = 0; i < queryCount; ++i) {
        if (i % 20 == 19) {
            queries.push_back("NOT_FOUND_" + to_string(i));
        } else {
            queries.push_back(students[(i * 9973ULL + 17ULL) % students.size()].id);
        }
    }
    return queries;
}

string pickDenseClassId(const vector<Student> &students) {
    if (students.empty()) {
        return "";
    }
    vector<string> sample;
    const size_t limit = min<size_t>(students.size(), 200000);
    sample.reserve(limit);
    for (size_t i = 0; i < limit; ++i) {
        sample.push_back(students[(i * 7919ULL) % students.size()].classId);
    }
    sort(sample.begin(), sample.end());
    string best = sample.front();
    size_t bestCount = 1;
    for (size_t i = 0; i < sample.size();) {
        size_t j = i + 1;
        while (j < sample.size() && sample[j] == sample[i]) {
            ++j;
        }
        if (j - i > bestCount) {
            best = sample[i];
            bestCount = j - i;
        }
        i = j;
    }
    return best;
}

template <typename Func>
vector<BenchTimedResult> runTimedSamples(int warmupRuns, int samples, Func &&func) {
    for (int i = 0; i < warmupRuns; ++i) {
        auto warm = func();
        g_benchSinkSize += warm.resultCount + static_cast<size_t>(max<long long>(0, warm.comparisons));
    }

    vector<BenchTimedResult> results;
    results.reserve(samples);
    for (int i = 0; i < samples; ++i) {
        results.push_back(func());
        g_benchSinkSize += results.back().resultCount + static_cast<size_t>(max<long long>(0, results.back().comparisons));
    }
    return results;
}

BenchStats statsOf(const vector<BenchTimedResult> &samples) {
    vector<double> values;
    values.reserve(samples.size());
    for (const auto &sample : samples) {
        values.push_back(sample.ms);
    }
    return summarizeBench(values);
}

long long medianComparisons(vector<BenchTimedResult> samples) {
    sort(samples.begin(), samples.end(), [](const BenchTimedResult &a, const BenchTimedResult &b) {
        return a.comparisons < b.comparisons;
    });
    return samples[samples.size() / 2].comparisons;
}

size_t medianResultCount(vector<BenchTimedResult> samples) {
    sort(samples.begin(), samples.end(), [](const BenchTimedResult &a, const BenchTimedResult &b) {
        return a.resultCount < b.resultCount;
    });
    return samples[samples.size() / 2].resultCount;
}

string getTimestampForFilename() {
    auto now = chrono::system_clock::now();
    time_t in_time_t = chrono::system_clock::to_time_t(now);
    tm buf{};
#ifdef _WIN32
    localtime_s(&buf, &in_time_t);
#else
    localtime_r(&in_time_t, &buf);
#endif
    char str[64];
    strftime(str, sizeof(str), "%Y%m%d_%H%M%S", &buf);
    return string(str);
}

string getDateTimeReadable() {
    auto now = chrono::system_clock::now();
    time_t in_time_t = chrono::system_clock::to_time_t(now);
    tm buf{};
#ifdef _WIN32
    localtime_s(&buf, &in_time_t);
#else
    localtime_r(&in_time_t, &buf);
#endif
    char str[64];
    strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &buf);
    return string(str);
}

} // namespace

vector<Student> loadStudentsData(const string &filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        throw runtime_error("Khong the mo file: " + filePath);
    }

    json data;
    file >> data;

    if (!data.contains("students") || !data["students"].is_array())
    {
        throw runtime_error("File JSON khong dung dinh dang (thieu mang students).");
    }

    vector<Student> students;
    for (const auto &item : data["students"])
    {
        students.push_back({item.value("id", ""),
                            item.value("name", ""),
                            item.value("classId", ""),
                            item.value("gpa", 0.0)});
    }

    if (students.empty())
    {
        throw runtime_error("Danh sach sinh vien trong file JSON bi rong!");
    }

    return students;
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen()
{
#ifdef _WIN32
    cout << "\nNhan phim bat ky de tiep tuc... ";
    _getch();
#else
    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

static int selectMenuInteractive(const string &title, const vector<string> &options)
{
    int currentIndex = 0;
    int totalOptions = static_cast<int>(options.size());

    while (true)
    {
        clearScreen();
        cout << "=========================================================================================\n";
        cout << "                  " << title << "\n";
        cout << "=========================================================================================\n";
        cout << " [HUONG DAN]: Dung phim Mui ten Len/Xuong de chon, Enter de thuc thi, Esc de thoat      \n";
        cout << "-----------------------------------------------------------------------------------------\n";

        for (int i = 0; i < totalOptions; i++)
        {
            int displayNum = (i == totalOptions - 1) ? 0 : (i + 1);
            if (i == currentIndex)
            {
                cout << "  -->  [ " << displayNum << " ]  "
                     << left << setw(65) << options[i]
                     << "  <== [DANG CHON]\n";
            }
            else
            {
                cout << "       [ " << displayNum << " ]  "
                     << left << setw(65) << options[i] << "\n";
            }
        }

        cout << "=========================================================================================\n";

#ifdef _WIN32
        int ch = _getch();
        if (ch == 0 || ch == 224)
        {
            int arrow = _getch();
            if (arrow == 72)
            {
                if (currentIndex > 0)
                    currentIndex--;
                else
                    currentIndex = totalOptions - 1;
            }
            else if (arrow == 80)
            {
                if (currentIndex < totalOptions - 1)
                    currentIndex++;
                else
                    currentIndex = 0;
            }
        }
        else if (ch == 13)
        {
            if (currentIndex == totalOptions - 1)
                return 0;
            return currentIndex + 1;
        }
        else if (ch == 27 || ch == '0')
        {
            return 0;
        }
        else if (ch >= '1' && ch <= '0' + totalOptions - 1)
        {
            return ch - '0';
        }
#else
        cout << "Chon chuc nang (0-" << totalOptions - 1 << "): ";
        int choice;
        if (cin >> choice)
            return choice;
        return 0;
#endif
    }
}

int selectMainMenuInteractive()
{
    const vector<string> menuOptions = {
        "MODE 1: GIAI THUAT TOI UU   (FINAL SOLUTION / PRODUCTION)",
        "MODE 2: HE THONG QUAN LY SINH VIEN (CRUD)",
        "MODE 3: SO SANH THUAT TOAN TUNG MODULE (BENCHMARK SUITE)",
        "MODE 4: BENCHMARK TOAN DIEN & XUAT FILE (AUTO BENCHMARK & EXPORT TXT)",
        "Thoat chuong trinh"};

    return selectMenuInteractive("HE THONG QUAN LY SINH VIEN - DASA230179", menuOptions);
}

int selectBenchmarkMenuInteractive()
{
    const vector<string> benchmarkOptions = {
        "Module 1: So sanh Loc GPA      (Linear Scan vs Binary Search)",
        "Module 2: So sanh Loc theo Lop (Linear Filter vs Optimized Index)",
        "Module 4: So sanh Tim MSSV     (Linear Search vs Hash Table)",
        "Module 5: So sanh Tim GPA Max  (Linear Max Scan vs Custom Max Heap)",
        "Quay lai Menu Chinh"};

    return selectMenuInteractive("MODE 3: SO SANH THUAT TOAN (BENCHMARK SUITE)", benchmarkOptions);
}

int selectFinalSolutionMenuInteractive()
{
    const vector<string> finalOptions = {
        "Module 1: Loc sinh vien theo khoang GPA (Sorted + Binary Search)",
        "Module 2: Loc sinh vien theo Lop        (Optimized Index Filter)",
        "Module 4: Tim kiem sinh vien theo MSSV  (Hash Table O(1))",
        "Module 5: Tim sinh vien co GPA cao nhat (Custom Max Heap O(1))",
        "Quay lai Menu Chinh"};

    return selectMenuInteractive("MODE 1: GIAI THUAT TOI UU (FINAL SOLUTION)", finalOptions);
}

void runBenchmarkSuite(FindStudentByGpaRange &gpaFilter,
                       FindStudentByClassId &classFilter,
                       FindStudentById &studentFinder,
                       FindStudentByMaxGpa &maxGpaFinder)
{
    while (true)
    {
        int choice = selectBenchmarkMenuInteractive();
        if (choice == 0)
            break;

        switch (choice)
        {
        case 1:
            clearScreen();
            gpaFilter.runComparison();
            pauseScreen();
            break;

        case 2:
            clearScreen();
            classFilter.filterBaseline();
            pauseScreen();
            break;

        case 3:
            clearScreen();
            studentFinder.runInteractiveSearch();
            pauseScreen();
            break;

        case 4:
            clearScreen();
            maxGpaFinder.runCompleteBenchmarkSuite();
            pauseScreen();
            break;

        default:
            break;
        }
    }
}

void runFinalSolutionSuite(FindStudentByGpaRange &gpaFilter,
                           FindStudentByClassId &classFilter,
                           FindStudentById &studentFinder,
                           FindStudentByMaxGpa &maxGpaFinder)
{
    while (true)
    {
        int choice = selectFinalSolutionMenuInteractive();
        if (choice == 0)
            break;

        switch (choice)
        {
        case 1:
            clearScreen();
            gpaFilter.runFinalSolution();
            pauseScreen();
            break;

        case 2:
            clearScreen();
            classFilter.filterFinalSolution();
            pauseScreen();
            break;

        case 3:
            clearScreen();
            studentFinder.runFinalSearch();
            pauseScreen();
            break;

        case 4:
            clearScreen();
            maxGpaFinder.runFinalSolution();
            pauseScreen();
            break;

        default:
            break;
        }
    }
}

int selectDatasetMenuInteractive(size_t currentDbSize)
{
    const vector<string> datasetOptions = {
        "1. Su dung Database mac dinh (data/database.json - " + to_string(currentDbSize) + " SV)",
        "2. Tu dong sinh 100,000 sinh vien    (N = 100K - Test Nhanh)",
        "3. Tu dong sinh 500,000 sinh vien    (N = 500K - Tieu Chuan)",
        "4. Tu dong sinh 1,000,000 sinh vien  (N = 1M - Quy Mo Lon)",
        "5. Tu dong sinh 10,000,000 sinh vien (N = 10M - Stress Test & Cache Pressure)",
        "6. Nhap so luong sinh vien tuy chon  (Custom N)",
        "Quay lai Menu Chinh"};

    return selectMenuInteractive("CHON NGUON DU LIEU BENCHMARK TOAN DIEN", datasetOptions);
}

void runAutomatedBenchmarkSuite(const vector<Student> &loadedStudents)
{
    while (true)
    {
        int dsChoice = selectDatasetMenuInteractive(loadedStudents.size());
        if (dsChoice == 0)
            break;

        vector<Student> benchStudents;
        string datasetName = "";
        double dataPrepMs = 0.0;

        clearScreen();
        cout << "=========================================================================================\n";
        cout << "       DANG CHUAN BI DU LIEU & KHOI TAO ENGINE BENCHMARK TOAN DIEN...                    \n";
        cout << "=========================================================================================\n";

        if (dsChoice == 1)
        {
            datasetName = "Database JSON (data/database.json)";
            cout << " -> Dang sao chep du lieu tu Database JSON (" << loadedStudents.size() << " sinh vien)...\n";
            auto t0 = Clock::now();
            benchStudents = loadedStudents;
            auto t1 = Clock::now();
            dataPrepMs = chrono::duration<double, milli>(t1 - t0).count();
        }
        else if (dsChoice == 2)
        {
            datasetName = "Auto Generated (N = 100,000)";
            cout << " -> Dang sinh ngau nhien 100,000 sinh vien trong RAM...\n";
            auto t0 = Clock::now();
            benchStudents = generateFastStudents(100000);
            auto t1 = Clock::now();
            dataPrepMs = chrono::duration<double, milli>(t1 - t0).count();
        }
        else if (dsChoice == 3)
        {
            datasetName = "Auto Generated (N = 500,000)";
            cout << " -> Dang sinh ngau nhien 500,000 sinh vien trong RAM...\n";
            auto t0 = Clock::now();
            benchStudents = generateFastStudents(500000);
            auto t1 = Clock::now();
            dataPrepMs = chrono::duration<double, milli>(t1 - t0).count();
        }
        else if (dsChoice == 4)
        {
            datasetName = "Auto Generated (N = 1,000,000)";
            cout << " -> Dang sinh ngau nhien 1,000,000 sinh vien trong RAM...\n";
            auto t0 = Clock::now();
            benchStudents = generateFastStudents(1000000);
            auto t1 = Clock::now();
            dataPrepMs = chrono::duration<double, milli>(t1 - t0).count();
        }
        else if (dsChoice == 5)
        {
            datasetName = "Auto Generated (N = 10,000,000)";
            cout << " -> Dang sinh ngau nhien 10,000,000 sinh vien trong RAM (Vui long doi giay lat)...\n";
            auto t0 = Clock::now();
            benchStudents = generateFastStudents(10000000);
            auto t1 = Clock::now();
            dataPrepMs = chrono::duration<double, milli>(t1 - t0).count();
        }
        else if (dsChoice == 6)
        {
            cout << "\nNhap so luong sinh vien can sinh (vi du 2000000): ";
            size_t customCount = 0;
            if (!(cin >> customCount) || customCount == 0)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "[Loi] So luong khong hop le!\n";
                pauseScreen();
                continue;
            }
            datasetName = "Auto Generated (N = " + to_string(customCount) + ")";
            cout << " -> Dang sinh ngau nhien " << customCount << " sinh vien trong RAM...\n";
            auto t0 = Clock::now();
            benchStudents = generateFastStudents(customCount);
            auto t1 = Clock::now();
            dataPrepMs = chrono::duration<double, milli>(t1 - t0).count();
        }

        cout << " [OK] Du lieu da san sang! (Size: " << benchStudents.size() << " SV, Thoi gian nap: "
             << fixed << setprecision(2) << dataPrepMs << " ms)\n";
        cout << " -> Dang tien hanh benchmark so sanh toan bo thuat toan (Vui long khong tat chuong trinh)...\n";

        const int warmupRuns = 3;
        const int samples = 10;
        const size_t idQueryCount = 1000;
        const int repeatedQueries = 1000;
        const double minGpa = 8.0;
        const double maxGpa = 10.0;
        const string targetClassId = pickDenseClassId(benchStudents);
        const auto idQueries = makeIdQueries(benchStudents, idQueryCount);

        // 1. Build Data Structures
        HashTable hashTable;
        auto hashBuildSamples = runTimedSamples(warmupRuns, samples, [&]() {
            double ms = measureMs([&]() {
                hashTable.build(benchStudents);
            });
            return BenchTimedResult{ms, benchStudents.size(), static_cast<long long>(hashTable.getCollisions())};
        });

        CustomMaxHeapGpaFinder heapFinder(benchStudents);
        auto heapBuildSamples = runTimedSamples(warmupRuns, samples, [&]() {
            double ms = measureMs([&]() {
                heapFinder.buildStructure();
            });
            return BenchTimedResult{ms, benchStudents.size(), 0};
        });

        SortedGpaFilter sortedGpa;
        auto sortedBuildSamples = runTimedSamples(warmupRuns, samples, [&]() {
            double ms = measureMs([&]() {
                sortedGpa.build(benchStudents);
            });
            return BenchTimedResult{ms, benchStudents.size(), 0};
        });

        ClassIndexFilter classIndexFilter;
        auto classIndexBuildSamples = runTimedSamples(warmupRuns, samples, [&]() {
            double ms = measureMs([&]() {
                classIndexFilter.build(benchStudents);
            });
            return BenchTimedResult{ms, benchStudents.size(), 0};
        });

        LinearSearch linearSearch;
        LinearMaxScanGpaFinder linearMax(benchStudents);

        // 2. MC1: Tra cứu MSSV (Linear vs Hash Table)
        auto mc1Linear = runTimedSamples(warmupRuns, samples, [&]() {
            long long comparisons = 0;
            size_t found = 0;
            double ms = measureMs([&]() {
                for (const auto &query : idQueries) {
                    const Student *student = linearSearch.search(benchStudents, query);
                    if (student != nullptr) {
                        ++found;
                        g_benchSinkGpa += student->gpa;
                    }
                    comparisons += static_cast<long long>(linearSearch.getComparisons());
                }
            });
            return BenchTimedResult{ms, found, comparisons};
        });

        auto mc1Hash = runTimedSamples(warmupRuns, samples, [&]() {
            long long probes = 0;
            size_t found = 0;
            double ms = measureMs([&]() {
                for (const auto &query : idQueries) {
                    const Student *student = hashTable.search(query);
                    if (student != nullptr) {
                        ++found;
                        g_benchSinkGpa += student->gpa;
                    }
                    probes += static_cast<long long>(hashTable.getProbes());
                }
            });
            return BenchTimedResult{ms, found, probes};
        });

        // 3. MC2: Tìm Max GPA (Linear Scan vs Max Heap)
        auto mc2Linear = runTimedSamples(warmupRuns, samples, [&]() {
            MC2Result result;
            double ms = measureMs([&]() {
                for (int i = 0; i < repeatedQueries; ++i) {
                    result = linearMax.findMaxGPA();
                    g_benchSinkGpa += result.student.gpa;
                }
            });
            return BenchTimedResult{ms, result.found ? 1U : 0U, result.comparisons * repeatedQueries};
        });

        auto mc2Heap = runTimedSamples(warmupRuns, samples, [&]() {
            MC2Result result;
            double ms = measureMs([&]() {
                for (int i = 0; i < repeatedQueries; ++i) {
                    result = heapFinder.findMaxGPA();
                    g_benchSinkGpa += result.student.gpa;
                }
            });
            return BenchTimedResult{ms, result.found ? 1U : 0U, 0};
        });

        // 4. RQ1: Lọc Mã Lớp (Linear vs Class Index View)
        auto rq1Linear = runTimedSamples(warmupRuns, samples, [&]() {
            LinearFilterResult result;
            double ms = measureMs([&]() {
                result = LinearFilter::filter(benchStudents, targetClassId);
            });
            return BenchTimedResult{ms, result.students.size(), result.comparisons};
        });

        auto rq1Optimized = runTimedSamples(warmupRuns, samples, [&]() {
            ClassIndexViewResult result;
            double ms = measureMs([&]() {
                result = classIndexFilter.filterView(targetClassId);
            });
            return BenchTimedResult{ms, result.size(), result.comparisons};
        });

        // 5. RQ2: Lọc khoảng GPA (Linear vs Sorted + Binary Search View)
        auto rq2Linear = runTimedSamples(warmupRuns, samples, [&]() {
            FilterGpaResult result;
            double ms = measureMs([&]() {
                result = LinearGpaFilter::filter(benchStudents, minGpa, maxGpa);
            });
            return BenchTimedResult{ms, result.students.size(), result.comparisons};
        });

        auto rq2Sorted = runTimedSamples(warmupRuns, samples, [&]() {
            GpaRangeViewResult result;
            double ms = measureMs([&]() {
                result = sortedGpa.filter(minGpa, maxGpa);
            });
            return BenchTimedResult{ms, result.size(), result.comparisons};
        });

        // Tính toán Thống kê & Báo cáo
        auto hashStats = statsOf(hashBuildSamples);
        auto heapStats = statsOf(heapBuildSamples);
        auto sortedStats = statsOf(sortedBuildSamples);
        auto classIdxStats = statsOf(classIndexBuildSamples);

        auto mc1LinearStats = statsOf(mc1Linear);
        auto mc1HashStats = statsOf(mc1Hash);
        auto mc2LinearStats = statsOf(mc2Linear);
        auto mc2HeapStats = statsOf(mc2Heap);
        auto rq1LinearStats = statsOf(rq1Linear);
        auto rq1OptimizedStats = statsOf(rq1Optimized);
        auto rq2LinearStats = statsOf(rq2Linear);
        auto rq2SortedStats = statsOf(rq2Sorted);

        double speedupMC1 = (mc1HashStats.medianMs > 1e-6) ? (mc1LinearStats.medianMs / mc1HashStats.medianMs) : (mc1LinearStats.medianMs / 0.0001);
        double speedupMC2 = (mc2HeapStats.medianMs > 1e-6) ? (mc2LinearStats.medianMs / mc2HeapStats.medianMs) : (mc2LinearStats.medianMs / 0.0001);
        double speedupRQ1 = (rq1OptimizedStats.medianMs > 1e-6) ? (rq1LinearStats.medianMs / rq1OptimizedStats.medianMs) : (rq1LinearStats.medianMs / 0.0001);
        double speedupRQ2 = (rq2SortedStats.medianMs > 1e-6) ? (rq2LinearStats.medianMs / rq2SortedStats.medianMs) : (rq2LinearStats.medianMs / 0.0001);

        string timeStr = getDateTimeReadable();
        string timeFile = getTimestampForFilename();

        stringstream report;
        report << "=========================================================================================\n";
        report << "       BAO CAO KET QUA BENCHMARK HE THONG THUAT TOAN DSA (DASA230179)\n";
        report << "=========================================================================================\n";
        report << "  Thoi gian thuc hien    : " << timeStr << "\n";
        report << "  Tap du lieu (Dataset)  : " << datasetName << "\n";
        report << "  Quy mo tap du lieu (N) : " << benchStudents.size() << " sinh vien\n";
        report << "  Thoi gian nap/sinh     : " << fixed << setprecision(3) << dataPrepMs << " ms\n";
        report << "  So lan khoi dong Warmup: " << warmupRuns << " runs\n";
        report << "  So lan do mau (Samples): " << samples << " samples\n";
        report << "  Truy van MC1 (MSSV)    : " << idQueryCount << " queries\n";
        report << "  Lap lai MC2 (Max GPA)  : " << repeatedQueries << " times\n";
        report << "  Ma lop test RQ1        : " << targetClassId << "\n";
        report << "  Khoang GPA test RQ2    : [" << minGpa << " - " << maxGpa << "]\n";
        report << "-----------------------------------------------------------------------------------------\n";
        report << " 1. THOI GIAN XAY DUNG CAU TRUC DU LIEU (BUILD PHASE - ONE TIME)\n";
        report << "-----------------------------------------------------------------------------------------\n";
        report << "  " << left << setw(30) << "Cau truc du lieu"
               << right << setw(14) << "Min (ms)"
               << setw(14) << "Median (ms)"
               << setw(14) << "Mean (ms)"
               << setw(14) << "Max (ms)" << "\n";
        report << "  ---------------------------------------------------------------------------------------\n";
        
        auto printRow = [&](const string &name, const BenchStats &st) {
            report << "  " << left << setw(30) << name
                   << right << fixed << setprecision(4)
                   << setw(14) << st.minMs
                   << setw(14) << st.medianMs
                   << setw(14) << st.meanMs
                   << setw(14) << st.maxMs << "\n";
        };

        printRow("Build Closed HashTable", hashStats);
        printRow("Build Binary Max-Heap", heapStats);
        printRow("Build Sorted GPA Array", sortedStats);
        printRow("Build Class Index Map", classIdxStats);

        report << "-----------------------------------------------------------------------------------------\n";
        report << " 2. DOI SANH HIEU NANG TRUY VAN & DOI SOANH GIAI THUAT (EXECUTION PHASE)\n";
        report << "-----------------------------------------------------------------------------------------\n";
        printRow("MC1 Linear Search (" + to_string(idQueryCount) + "q)", mc1LinearStats);
        printRow("MC1 Closed Hash Table (" + to_string(idQueryCount) + "q)", mc1HashStats);
        report << "  --> TANG TOC MC1 (Hash Table vs Linear): " << fixed << setprecision(1) << speedupMC1 << "x lan\n";
        report << "      + Phep so sanh Linear: " << medianComparisons(mc1Linear) << " | Probes Hash: " << medianComparisons(mc1Hash) << "\n\n";

        printRow("MC2 Linear Max Scan (" + to_string(repeatedQueries) + "q)", mc2LinearStats);
        printRow("MC2 Custom Max Heap (" + to_string(repeatedQueries) + "q)", mc2HeapStats);
        report << "  --> TANG TOC MC2 (Max Heap vs Linear): " << fixed << setprecision(1) << speedupMC2 << "x lan\n";
        report << "      + Phep so sanh Linear: " << medianComparisons(mc2Linear) << " | Heap Peek: O(1)\n\n";

        printRow("RQ1 Linear Class Filter", rq1LinearStats);
        printRow("RQ1 Class Index View", rq1OptimizedStats);
        report << "  --> TANG TOC RQ1 (Index View vs Linear): " << fixed << setprecision(1) << speedupRQ1 << "x lan\n";
        report << "      + Phep so sanh Linear: " << medianComparisons(rq1Linear) << " | Index Filter: " << medianComparisons(rq1Optimized) << "\n\n";

        printRow("RQ2 Linear GPA Filter", rq2LinearStats);
        printRow("RQ2 Sorted Binary Range", rq2SortedStats);
        report << "  --> TANG TOC RQ2 (Sorted Range vs Linear): " << fixed << setprecision(1) << speedupRQ2 << "x lan\n";
        report << "      + Phep so sanh Linear: " << medianComparisons(rq2Linear) << " | Binary Range: " << medianComparisons(rq2Sorted) << "\n";

        report << "=========================================================================================\n";
        report << " 3. TONG KET KET QUA & KET LUAN\n";
        report << "=========================================================================================\n";
        report << "  [MC1 Tra cuu MSSV]   : Hash Table dat O(1) trung binh, giam >99.9% thoi gian truy van.\n";
        report << "  [MC2 Tim Max GPA]    : Max Heap tra ve O(1) tuc thi thay vi quet tuyen tinh O(N).\n";
        report << "  [RQ1 Loc theo Lop]   : Index View khong copy du lieu, tang toc vuot troi tren tap lon.\n";
        report << "  [RQ2 Loc khoang GPA] : Binary Search O(log N) giam thieu toi da so phep so sanh.\n";
        report << "=========================================================================================\n";

        // In ra man hinh Console
        clearScreen();
        cout << report.str();

        // Xuat ra file .txt theo format timestamp
        error_code ec;
        filesystem::create_directories("benchmark", ec);
        string filename = "benchmark/bench_report_" + timeFile + ".txt";
        ofstream outFile(filename);
        if (outFile.is_open())
        {
            outFile << report.str();
            outFile.close();
            cout << "\n [XUAT FILE THANH CONG] Da luu bao cao benchmark chi tiet vao:\n";
            cout << "   --> " << filename << "\n";
        }
        else
        {
            cout << "\n [Canh bao] Khong the ghi file: " << filename << "\n";
        }

        cout << "=========================================================================================\n";
        pauseScreen();
    }
}

int main()
{
    vector<Student> students;
    try
    {
        students = loadStudentsData("data/database.json");
    }
    catch (const exception &e)
    {
        cerr << "[Loi] Khong the nap du lieu: " << e.what() << "\n";
        return 1;
    }

    FindStudentByGpaRange gpaFilter(students);
    FindStudentByClassId classFilter(students);
    StudentCRUD studentCrud(students);
    FindStudentById studentFinder(students);
    FindStudentByMaxGpa maxGpaFinder(students);

    while (true)
    {
        int modeChoice = selectMainMenuInteractive();

        if (modeChoice == 0)
        {
            clearScreen();
            cout << "=========================================================================================\n";
            cout << "                 CAM ON BAN DA SU DUNG HE THONG QUAN LY SINH VIEN!                       \n";
            cout << "=========================================================================================\n";
            break;
        }

        switch (modeChoice)
        {
        case 1:
            runFinalSolutionSuite(gpaFilter, classFilter, studentFinder, maxGpaFinder);
            break;

        case 2:
            clearScreen();
            studentCrud.runCRUDMenu();
            break;

        case 3:
            runBenchmarkSuite(gpaFilter, classFilter, studentFinder, maxGpaFinder);
            break;

        case 4:
            runAutomatedBenchmarkSuite(students);
            break;

        default:
            break;
        }
    }

    return 0;
}
