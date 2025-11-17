#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "TUMStudentManager.h"
#include "FileManager.h"
#include <string>

class ConsoleUI {
private:
    FileManager storage;
    TUMStudentManager manager;

    void displayMainMenu();
    void facultyOperations();
    void generalOperations();
    int selectFacultyIndex();
    void createAndAssignStudent(size_t facultyIndex);
    void graduateStudent(size_t facultyIndex);
    void displayEnrolledStudents(size_t facultyIndex);
    void displayGraduates(size_t facultyIndex);
    void checkStudentInFaculty(size_t facultyIndex);
    void createNewFaculty();
    void searchStudentFaculty();
    void displayAllFaculties();
    void displayFacultiesByField();

    tm parseDate(const std::string& dateStr);
    std::string dateToString(const tm& date);

public:
    ConsoleUI();
    void run();
};

#endif
