#include <iostream>
using namespace std;

class Car {
private:
    string brand;
    int year;

public:
    // Constructor
    Car(string b, int y) {
        brand = b;
        year = y;
        cout << "Car " << brand << " from " << year << " created." << endl;
    }

    // Destructor
    ~Car() {
        cout << "Car " << brand << " destroyed." << endl;
    }

    void showInfo() {
        cout << "Brand: " << brand << ", Year: " << year << endl;
    }
};

int main() {
    // Creating a Car object on the heap
    Car* myCar = new Car("Lada", 1989);
    
    // Calling showInfo
    myCar->showInfo();

    // Deleting object (calls destructor)
    delete myCar;

    return 0;
}
