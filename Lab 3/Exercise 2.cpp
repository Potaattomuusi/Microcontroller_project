#include <iostream>
using namespace std;

class Student {
private:
    string name;
    int age;
    char grade;
    string group = "2025 group";

public:
    // Setters
    void setName(string n) {
        name = n;
    }

    void setAge(int a) {
        age = a;
    }

    void setGrade(char g) {
        grade = g;
    }

    // Getters
    string getName() {
        return name;
    }

    int getAge() {
        return age;
    }

    char getGrade() {
        return grade;
    }

    void displayInfo() {
        cout << "Student Info:" << endl;
        cout << "Name: " << getName() << endl;
        cout << "Age: " << getAge() << endl;
        cout << "Grade: " << getGrade() << endl;
        cout << "Group: " << group << endl;
    }
};

int main() {
    Student s;

    // Setting values using setters
    s.setName("Steve");
    s.setAge(21);
    s.setGrade('A');


    // Display all info
    s.displayInfo();

    return 0;
}
