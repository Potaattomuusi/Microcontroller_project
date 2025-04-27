#include <iostream>
#include <string>
using namespace std;

class Car {
protected:
    string model;
    int year;
    float price;

public:
    Car() : model(""), year(0), price(0.0f) {}
    virtual ~Car() {}

    void inputBasic() {
        cout << "Enter model: ";
        cin >> model;
        cout << "Enter year: ";
        cin >> year;
        cout << "Enter price: ";
        cin >> price;
    }

    void displayBasic() const {
        cout << "Model: " << model << ", Year: " << year << ", Price: " << price << endl;
    }

    virtual void input() = 0;
    virtual void display() const = 0;
};

class ElectricCar : public Car {
private:
    float batterySize;

public:
    ElectricCar() : batterySize(0.0f) {}

    void input() override {
        inputBasic();
        cout << "Enter battery size (kWh): ";
        cin >> batterySize;
    }

    void display() const override {
        displayBasic();
        cout << "Battery size: " << batterySize << " kWh" << endl;
    }
};

class GasCar : public Car {
private:
    float fuelCapacity;

public:
    GasCar() : fuelCapacity(0.0f) {}

    void input() override {
        inputBasic();
        cout << "Enter fuel capacity (liters): ";
        cin >> fuelCapacity;
    }

    void display() const override {
        displayBasic();
        cout << "Fuel capacity: " << fuelCapacity << " liters" << endl;
    }
};

class HybridCar : public Car {
private:
    float batterySize;
    float fuelCapacity;

public:
    HybridCar() : batterySize(0.0f), fuelCapacity(0.0f) {}

    void input() override {
        inputBasic();
        cout << "Enter battery size (kWh): ";
        cin >> batterySize;
        cout << "Enter fuel capacity (liters): ";
        cin >> fuelCapacity;
    }

    void display() const override {
        displayBasic();
        cout << "Battery size: " << batterySize << " kWh, Fuel capacity: " << fuelCapacity << " liters" << endl;
    }
};
