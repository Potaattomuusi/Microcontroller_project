#include <iostream>
using namespace std;

int main() {
    int num, sum = 0;

    // Take an integer input from the user
    cout << "Enter an integer: ";
    cin >> num;

    // Calculate the sum of digits using a for loop
    for (; num != 0; num /= 10) {
        sum += num % 10; // Add the last digit to the sum
    }

    // Print the result
    cout << "The sum of the digits is: " << sum << endl;

    return 0;
}