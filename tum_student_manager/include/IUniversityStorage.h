#ifndef IUNIVERSITYSTORAGE_H
#define IUNIVERSITYSTORAGE_H

#include "Faculty.h"
#include <vector>
#include <string>

using namespace std;

class IUniversityStorage {
public:
    virtual ~IUniversityStorage() = default;

    // Save faculties to storage (dir parameter optional)
    virtual bool save(const vector<Faculty>& faculties, const string& dir = "data") = 0;

    // Load faculties from storage
    virtual vector<Faculty> load(const string& dir = "data") = 0;
};

#endif
