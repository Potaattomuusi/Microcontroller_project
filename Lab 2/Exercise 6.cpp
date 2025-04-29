#include <iostream>
#include <cstring> 
using namespace std;

// Define the Student struct
struct Student {
    char name[50];
    int id;
    float grade;
};

int main() {
    const int MAX_STUDENTS = 100;
    Student students[MAX_STUDENTS];
    int count = 0; // number of students added
    int choice;

    while (true) {
        // Menu
        cout << "\nStudent Record System\n";
        cout << "1. Add a student\n";
        cout << "2. Display all students\n";
        cout << "3. Search for student by ID\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                if (count < MAX_STUDENTS) {
                    cout << "Enter student's name: ";
                    cin.ignore(); // clear newline from previous input
                    cin.getline(students[count].name, 50);
                    cout << "Enter student ID: ";
                    cin >> students[count].id;
                    cout << "Enter grade: ";
                    cin >> students[count].grade;
                    count++;
                    cout << "Student added successfully.\n";
                } else {
                    cout << "Student list is full.\n";
                }
                break;
            }

            case 2: {
                if (count == 0) {
                    cout << "No students to display.\n";
                } else {
                    cout << "\nAll Students:\n";
                    for (int i = 0; i < count; i++) {
                        cout << "Name: " << students[i].name
                             << ", ID: " << students[i].id
                             << ", Grade: " << students[i].grade << endl;
                    }
                }
                break;
            }

            case 3: {
                int searchId;
                bool found = false;
                cout << "Enter ID to search: ";
                cin >> searchId;
                for (int i = 0; i < count; i++) {
                    if (students[i].id == searchId) {
                        cout << "Student found:\n";
                        cout << "Name: " << students[i].name
                             << ", Grade: " << students[i].grade << endl;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Student with ID " << searchId << " not found.\n";
                }
                break;
            }

                case 4:
                    cout << "Exiting program.\n";
                    return 0;
            } 
        } 
    return 0;
}

