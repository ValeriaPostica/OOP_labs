#ifndef TUMSTUDENTMANAGER_H
#define TUMSTUDENTMANAGER_H

#include "Faculty.h"
#include <vector>

using namespace std;

class TUMStudentManager {
private:
    vector<Faculty> faculties;
    void initializeSampleData();

public:
    TUMStudentManager();

    // Read-only access to faculties
    const vector<Faculty>& getFaculties() const;

    // Business operations (no UI)
    bool createFaculty(const string& name, const string& abbreviation, StudyField field);
    bool addStudentToFaculty(size_t facultyIndex, const Student& student);
    bool graduateStudentInFaculty(size_t facultyIndex, const string& email);
    int findFacultyByStudentEmail(const string& email) const; // returns index or -1
    vector<Student> getEnrolledStudents(size_t facultyIndex) const;
    vector<Student> getGraduates(size_t facultyIndex) const;
    bool hasFaculty(size_t index) const;

    // Persistence
    bool saveData() const;
};

#endif