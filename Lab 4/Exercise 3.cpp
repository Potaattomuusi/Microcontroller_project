#include <iostream>
#include <string>
using namespace std;

class Car {
private:
    string model;
    int year;
    double price;

public:
    Car() : model("Unknown"), year(0), price(0.0) {}

    Car(string m, int y, double p) : model(m), year(y), price(p) {
        cout << "Car Constructor: " << model << " (" << year << ") - $" << price << endl;
    }

    ~Car() {
        cout << "Car Destructor: " << model << " destroyed" << endl;
    }

    void setDetails(string m, int y, double p) {
        model = m;
        year = y;
        price = p;
        cout << "Car Constructor: " << model << " (" << year << ") - $" << price << endl;
    }

    void display() const {
        cout << "Model: " << model << ", Year: " << year << ", Price: $" << price << endl;
    }
};

class CarCollection {
private:
    Car* cars;
    int size;

public:
    CarCollection() {
        cout << "How many cars do you want to enter? ";
        cin >> size;
        cin.ignore(); 
        cars = new Car[size];
        for (int i = 0; i < size; ++i) {
            string model;
            int year;
            double price;

            cout << "\nEnter model for car " << i + 1 << ": ";
            getline(cin, model);
            cout << "Enter year: ";
            cin >> year;
            cout << "Enter price: ";
            cin >> price;
            cin.ignore();

            cars[i].setDetails(model, year, price);
        }
        cout << "CarCollection initialized with " << size << " cars" << endl;
    }

    ~CarCollection() {
        delete[] cars;
        cout << "CarCollection destroyed" << endl;
    }

    void displayCars() const {
        cout << "\n--- Car Collection ---\n";
        for (int i = 0; i < size; ++i) {
            cars[i].display();
        }
    }
};

int main() {
    CarCollection* collection = new CarCollection(); 
    collection->displayCars();

    delete collection; 
    return 0;
}
