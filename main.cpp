#include "interface/StudentDatabase.h"

using namespace std;

int main() {
    StudentDatabase database;
    database.displayStudentsFromJson("data/database.json");
    return 0;
}