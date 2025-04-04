#include <iostream>
using namespace std;

int main() {
    int num, reversedNum = 0;

    // Take an integer input from the user
    cout << "Enter an integer: ";
    cin >> num;

    // Reverse the digits using a for loop
    for (; num != 0; num /= 10) {
        reversedNum = reversedNum * 10 + (num % 10); // Extract the last digit and add it to reversedNum
    }

    // Print the reversed number
    cout << "The reversed number is: " << reversedNum << endl;

    return 0;
}