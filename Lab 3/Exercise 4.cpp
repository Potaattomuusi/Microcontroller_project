#include <iostream>
using namespace std;

// 4.1 Base class
class Vehicle {
protected:
    string brand;
    int year;

public:
    Vehicle(string b, int y) {
        brand = b;
        year = y;
    }

    virtual void showInfo() {
        cout << "Brand: " << brand << endl;
        cout << "Year: " << year << endl;
    }

    virtual void startEngine() {
        cout << "Starting engine of Vehicle" << endl;
    }

    virtual ~Vehicle() {}
};

// 4.2 Derived class
class Car : public Vehicle {
private:
    int numDoors;

public:
    Car(string b, int y, int nDoors) : Vehicle(b, y) {
        numDoors = nDoors;
    }

    void showInfo() override {
        Vehicle::showInfo();
        cout << "Number of doors: " << numDoors << endl;
    }

    void startEngine() override {
        cout << "Car engine is starting!" << endl;
    }

    virtual ~Car() {}
};

// 4.3 Further derived class
class ElectricCar : public Car {
private:
    int batteryCapacity;

public:
    // Default constructor
    ElectricCar() : Car("Hyundai", 2023, 4) {
        batteryCapacity = 0;
    }

    // Parameterized constructor
    ElectricCar(string b, int y, int nDoors, int batteryCap) : Car(b, y, nDoors) {
        batteryCapacity = batteryCap;
    }

    void showInfo() override {
        Car::showInfo();
        cout << "Battery Capacity: " << batteryCapacity << " kWh" << endl;
    }

    void startEngine() override {
        cout << "Electric engine is starting... Silent but powerful!" << endl;
    }

    ~ElectricCar() {}
};

int main() {
    // Vehicle object
    Vehicle* v = new Vehicle("Generic", 2015);
    v->showInfo();
    v->startEngine();
    delete v;

    cout << endl;

    // Car object
    Car* c = new Car("Toyota", 2021, 4);
    c->showInfo();
    c->startEngine();
    delete c;

    cout << endl;

    // ElectricCar using default constructor
    ElectricCar* ec1 = new ElectricCar();
    ec1->showInfo();
    ec1->startEngine();
    delete ec1;

    cout << endl;

    // ElectricCar using parameterized constructor
    ElectricCar* ec2 = new ElectricCar("Nissan", 2022, 4, 40);
    ec2->showInfo();
    ec2->startEngine();
    delete ec2;

    return 0;
}
