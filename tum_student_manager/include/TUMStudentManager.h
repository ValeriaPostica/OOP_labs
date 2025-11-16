#ifndef TUMSTUDENTMANAGER_H
#define TUMSTUDENTMANAGER_H

#include "Faculty.h"
#include <vector>

using namespace std;

class TUMStudentManager {
private:
    vector<Faculty> faculties;
    
    void initializeSampleData();
    void displayMainMenu();
    void facultyOperations();
    void generalOperations();
    void batchOperations();
    
    Faculty* selectFaculty();
    void createAndAssignStudent(Faculty& faculty);
    void graduateStudent(Faculty& faculty);
    void displayEnrolledStudents(const Faculty& faculty);
    void displayGraduates(const Faculty& faculty);
    void checkStudentInFaculty(const Faculty& faculty);
    void createNewFaculty();
    void searchStudentFaculty();
    void displayAllFaculties();
    void displayFacultiesByField();
    void batchEnrollStudents();
    void batchGraduateStudents();
    
    tm parseDate(const string& dateStr);
    string dateToString(const tm& date);

public:
    TUMStudentManager();
    void run();
};

#endif