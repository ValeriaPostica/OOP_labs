#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Faculty.h"
#include "IUniversityStorage.h"
#include <vector>
#include <string>

using namespace std;

class FileManager : public IUniversityStorage {
public:
    // Implement IUniversityStorage
    bool save(const vector<Faculty>& faculties, const string& dir = "data") override;
    vector<Faculty> load(const string& dir = "data") override;
};

#endif