#include <iostream>
using namespace std;

#define SQUARE(x) ((x) * (x)) // Macro to calculate the square of x

int main() {
    int num;

    // Take an integer input from the user
    cout << "Enter an integer: ";
    cin >> num;

    // Use the SQUARE macro to calculate and print the square
    cout << "The square of " << num << " is " << SQUARE(num) << endl;

    return 0;
}