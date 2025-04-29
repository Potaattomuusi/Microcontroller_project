#include <iostream>
using namespace std;

int main() {
    int number;

    while (true) {
        
        cout << "Give a number: ";
        cin >> number;

        // If number is negative, stop the loop
        if (number < 0) {
            break;
        }

        // If number is zero, skip processing
        if (number == 0) {
            continue;
        }

        // Otherwise, print the square of the number
        cout << "Square of " << number << " is " << number * number << endl;
    }

    return 0;
}
