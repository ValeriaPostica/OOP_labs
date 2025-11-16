#ifndef FACULTY_H
#define FACULTY_H

#include "Student.h"
#include "StudyField.h"
#include <vector>
#include <string>

using namespace std;

class Faculty {
private:
    string name;
    string abbreviation;
    vector<Student> students;
    StudyField studyField;

public:
    Faculty(const string& name, const string& abbreviation, StudyField studyField);
    
    void addStudent(const Student& student);
    bool graduateStudent(const string& email);
    vector<Student> getEnrolledStudents() const;
    vector<Student> getGraduates() const;
    bool hasStudent(const string& email) const;
    
    // Getters
    string getName() const;
    string getAbbreviation() const;
    StudyField getStudyField() const;
    vector<Student> getStudents() const;
    
    string toString() const;
};

#endif