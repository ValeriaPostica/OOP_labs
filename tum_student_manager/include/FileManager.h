#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Faculty.h"
#include <vector>
#include <string>

using namespace std;

class FileManager {
private:
    static const string DATA_FILE;

public:
    static bool saveData(const vector<Faculty>& faculties);
    static vector<Faculty> loadData();
};

#endif