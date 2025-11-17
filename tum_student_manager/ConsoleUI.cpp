#include "include/ConsoleUI.h"
#include "include/TUMStudentManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

ConsoleUI::ConsoleUI() : storage(), manager(storage) {}

void ConsoleUI::run() {
    cout << " Technical University of Moldova - Student Management System " << endl;
    while (true) {
        displayMainMenu();
        string choice;
        getline(cin, choice);
        if (choice == "1") {
            facultyOperations();
        } else if (choice == "2") {
            generalOperations();
        } else if (choice == "0") {
            bool ok = manager.saveData();
            if (ok) cout << "Data saved successfully!" << endl;
            else cout << "Warning: could not save data." << endl;
            cout << "Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid option. Please try again." << endl;
        }
    }
}

void ConsoleUI::displayMainMenu() {
    cout << "\n MAIN MENU " << endl;
    cout << "1. Faculty Operations" << endl;
    cout << "2. General Operations" << endl;
    cout << "0. Exit" << endl;
    cout << "Choose an option: ";
}

int ConsoleUI::selectFacultyIndex() {
    const auto& faculties = manager.getFaculties();
    if (faculties.empty()) {
        cout << "No faculties available. Please create a faculty first." << endl;
        return -1;
    }
    cout << "\n Select Faculty " << endl;
    for (size_t i = 0; i < faculties.size(); ++i) {
        cout << (i + 1) << ". " << faculties[i].toString() << endl;
    }
    cout << "Choose faculty (1-" << faculties.size() << "): ";
    string choice;
    getline(cin, choice);
    try {
        int index = stoi(choice) - 1;
        if (index >= 0 && index < static_cast<int>(faculties.size())) {
            return index;
        } else {
            cout << "Invalid faculty choice." << endl;
        }
    } catch (...) {
        cout << "Please enter a valid number." << endl;
    }
    return -1;
}

void ConsoleUI::facultyOperations() {
    int idx = selectFacultyIndex();
    if (idx < 0) return;
    size_t facultyIndex = static_cast<size_t>(idx);

    while (true) {
        cout << "\n FACULTY OPERATIONS: " << manager.getFaculties()[facultyIndex].getName() << endl;
        cout << "1. Create and assign a student" << endl;
        cout << "2. Graduate a student" << endl;
        cout << "3. Display enrolled students" << endl;
        cout << "4. Display graduates" << endl;
        cout << "5. Check if student belongs to faculty" << endl;
        cout << "0. Back to main menu" << endl;
        cout << "Choose an option: ";

        string choice;
        getline(cin, choice);
        if (choice == "1") {
            createAndAssignStudent(facultyIndex);
        } else if (choice == "2") {
            graduateStudent(facultyIndex);
        } else if (choice == "3") {
            displayEnrolledStudents(facultyIndex);
        } else if (choice == "4") {
            displayGraduates(facultyIndex);
        } else if (choice == "5") {
            checkStudentInFaculty(facultyIndex);
        } else if (choice == "0") {
            break;
        } else {
            cout << "Invalid option." << endl;
        }
    }
}

void ConsoleUI::createAndAssignStudent(size_t facultyIndex) {
    try {
        string firstName, lastName, email, enrollmentDateStr, dobStr;
        cout << "Enter first name: ";
        getline(cin, firstName);
        cout << "Enter last name: ";
        getline(cin, lastName);
        cout << "Enter email: ";
        getline(cin, email);
        cout << "Enter enrollment date (yyyy-mm-dd): ";
        getline(cin, enrollmentDateStr);
        cout << "Enter date of birth (yyyy-mm-dd): ";
        getline(cin, dobStr);

        tm enrollmentDate = parseDate(enrollmentDateStr);
        tm dateOfBirth = parseDate(dobStr);

        Student student(firstName, lastName, email, enrollmentDate, dateOfBirth);
        if (manager.addStudentToFaculty(facultyIndex, student)) {
            cout << "Student added successfully." << endl;
        } else {
            cout << "Failed to add student." << endl;
        }
    } catch (const exception& e) {
        cout << "Error creating student: " << e.what() << endl;
    }
}

void ConsoleUI::graduateStudent(size_t facultyIndex) {
    string email;
    cout << "Enter student email to graduate: ";
    getline(cin, email);
    if (manager.graduateStudentInFaculty(facultyIndex, email)) {
        cout << "Student graduated successfully!" << endl;
    } else {
        cout << "Cannot graduate student: " << email << " (student not found or already graduated)" << endl;
    }
}

void ConsoleUI::displayEnrolledStudents(size_t facultyIndex) {
    auto enrolled = manager.getEnrolledStudents(facultyIndex);
    cout << "\n Enrolled Students in " << manager.getFaculties()[facultyIndex].getName() << endl;
    if (enrolled.empty()) cout << "No enrolled students." << endl;
    else {
        for (const auto& s : enrolled) cout << s.toString() << endl;
    }
}

void ConsoleUI::displayGraduates(size_t facultyIndex) {
    auto grads = manager.getGraduates(facultyIndex);
    cout << "\n Graduates from " << manager.getFaculties()[facultyIndex].getName() << endl;
    if (grads.empty()) cout << "No graduates." << endl;
    else {
        for (const auto& s : grads) cout << s.toString() << endl;
    }
}

void ConsoleUI::checkStudentInFaculty(size_t facultyIndex) {
    string email;
    cout << "Enter student email: ";
    getline(cin, email);
    bool has = manager.getFaculties()[facultyIndex].hasStudent(email);
    cout << "Student " << email << " belongs to " << manager.getFaculties()[facultyIndex].getName() << ": "
         << (has ? "Yes" : "No") << endl;
}

void ConsoleUI::generalOperations() {
    while (true) {
        cout << "\n GENERAL OPERATIONS " << endl;
        cout << "1. Create new faculty" << endl;
        cout << "2. Search student faculty" << endl;
        cout << "3. Display all faculties" << endl;
        cout << "4. Display faculties by field" << endl;
        cout << "0. Back to main menu" << endl;
        cout << "Choose an option: ";

        string choice;
        getline(cin, choice);
        if (choice == "1") createNewFaculty();
        else if (choice == "2") searchStudentFaculty();
        else if (choice == "3") displayAllFaculties();
        else if (choice == "4") displayFacultiesByField();
        else if (choice == "0") break;
        else cout << "Invalid option." << endl;
    }
}

void ConsoleUI::createNewFaculty() {
    string name, abbreviation;
    cout << "Enter faculty name: ";
    getline(cin, name);
    cout << "Enter faculty abbreviation: ";
    getline(cin, abbreviation);
    cout << "Available study fields:" << endl;
    cout << "1. Mechanical Engineering" << endl;
    cout << "2. Software Engineering" << endl;
    cout << "3. Food Technology" << endl;
    cout << "4. Urbanism Architecture" << endl;
    cout << "5. Veterinary Medicine" << endl;
    cout << "Choose study field (1-5): ";
    string fieldChoice;
    getline(cin, fieldChoice);
    StudyField studyField;
    if (fieldChoice == "1") studyField = StudyField::MECHANICAL_ENGINEERING;
    else if (fieldChoice == "2") studyField = StudyField::SOFTWARE_ENGINEERING;
    else if (fieldChoice == "3") studyField = StudyField::FOOD_TECHNOLOGY;
    else if (fieldChoice == "4") studyField = StudyField::URBANISM_ARCHITECTURE;
    else if (fieldChoice == "5") studyField = StudyField::VETERINARY_MEDICINE;
    else { cout << "Invalid field choice." << endl; return; }
    manager.createFaculty(name, abbreviation, studyField);
    cout << "Faculty created successfully!" << endl;
}

void ConsoleUI::searchStudentFaculty() {
    string email;
    cout << "Enter student email: ";
    getline(cin, email);
    int idx = manager.findFacultyByStudentEmail(email);
    if (idx >= 0) cout << "Student " << email << " belongs to: " << manager.getFaculties()[idx].getName() << endl;
    else cout << "Student " << email << " not found in any faculty." << endl;
}

void ConsoleUI::displayAllFaculties() {
    const auto& faculties = manager.getFaculties();
    cout << "\n All Faculties " << endl;
    if (faculties.empty()) cout << "No faculties available." << endl;
    else for (const auto& f : faculties) cout << f.toString() << endl;
}

void ConsoleUI::displayFacultiesByField() {
    cout << "Available study fields:" << endl;
    cout << "1. Mechanical Engineering" << endl;
    cout << "2. Software Engineering" << endl;
    cout << "3. Food Technology" << endl;
    cout << "4. Urbanism Architecture" << endl;
    cout << "5. Veterinary Medicine" << endl;
    cout << "Choose study field (1-5): ";
    string fieldChoice;
    getline(cin, fieldChoice);
    StudyField selectedField;
    if (fieldChoice == "1") selectedField = StudyField::MECHANICAL_ENGINEERING;
    else if (fieldChoice == "2") selectedField = StudyField::SOFTWARE_ENGINEERING;
    else if (fieldChoice == "3") selectedField = StudyField::FOOD_TECHNOLOGY;
    else if (fieldChoice == "4") selectedField = StudyField::URBANISM_ARCHITECTURE;
    else if (fieldChoice == "5") selectedField = StudyField::VETERINARY_MEDICINE;
    else { cout << "Invalid field choice." << endl; return; }

    cout << "\n Faculties in ";
    switch (selectedField) {
        case StudyField::MECHANICAL_ENGINEERING: cout << "Mechanical Engineering"; break;
        case StudyField::SOFTWARE_ENGINEERING: cout << "Software Engineering"; break;
        case StudyField::FOOD_TECHNOLOGY: cout << "Food Technology"; break;
        case StudyField::URBANISM_ARCHITECTURE: cout << "Urbanism Architecture"; break;
        case StudyField::VETERINARY_MEDICINE: cout << "Veterinary Medicine"; break;
    }
    cout << endl;

    bool found = false;
    const auto& faculties = manager.getFaculties();
    for (const auto& faculty : faculties) {
        if (faculty.getStudyField() == selectedField) {
            cout << faculty.toString() << endl;
            found = true;
        }
    }
    if (!found) cout << "No faculties found for this field." << endl;
}


tm ConsoleUI::parseDate(const string& dateStr) {
    tm date = {};
    istringstream ss(dateStr);
    ss >> get_time(&date, "%Y-%m-%d");
    if (ss.fail()) {
        throw runtime_error("Invalid date format. Please use yyyy-mm-dd.");
    }
    return date;
}

string ConsoleUI::dateToString(const tm& date) {
    stringstream ss;
    ss << put_time(&date, "%Y-%m-%d");
    return ss.str();
}
