#include "include/TUMStudentManager.h"
#include "include/FileManager.h"
#include <iostream>

using namespace std;

TUMStudentManager::TUMStudentManager() {
    faculties = FileManager::loadData();
    if (faculties.empty()) {
        initializeSampleData();
    }
}

void TUMStudentManager::initializeSampleData() {
    faculties.push_back(Faculty("Faculty of Computers, Informatics and Microelectronics", 
                               "FCIM", StudyField::SOFTWARE_ENGINEERING));
    faculties.push_back(Faculty("Faculty of Food Technology", 
                               "FTA", StudyField::FOOD_TECHNOLOGY));
    faculties.push_back(Faculty("Faculty of Mechanical Engineering and Transport", 
                               "FIMIT", StudyField::MECHANICAL_ENGINEERING));
}

const vector<Faculty>& TUMStudentManager::getFaculties() const {
    return faculties;
}

bool TUMStudentManager::createFaculty(const string& name, const string& abbreviation, StudyField field) {
    faculties.emplace_back(name, abbreviation, field);
    return true;
}

bool TUMStudentManager::addStudentToFaculty(size_t facultyIndex, const Student& student) {
    if (facultyIndex >= faculties.size()) return false;
    faculties[facultyIndex].addStudent(student);
    return true;
}

bool TUMStudentManager::graduateStudentInFaculty(size_t facultyIndex, const string& email) {
    if (facultyIndex >= faculties.size()) return false;
    return faculties[facultyIndex].graduateStudent(email);
}

int TUMStudentManager::findFacultyByStudentEmail(const string& email) const {
    for (size_t i = 0; i < faculties.size(); ++i) {
        if (faculties[i].hasStudent(email)) return static_cast<int>(i);
    }
    return -1;
}

vector<Student> TUMStudentManager::getEnrolledStudents(size_t facultyIndex) const {
    if (facultyIndex >= faculties.size()) return {};
    return faculties[facultyIndex].getEnrolledStudents();
}

vector<Student> TUMStudentManager::getGraduates(size_t facultyIndex) const {
    if (facultyIndex >= faculties.size()) return {};
    return faculties[facultyIndex].getGraduates();
}

bool TUMStudentManager::hasFaculty(size_t index) const {
    return index < faculties.size();
}

bool TUMStudentManager::saveData() const {
    return FileManager::saveData(faculties);
}