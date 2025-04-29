#include <iostream>
using namespace std;

int main() {
    float value1, value2, result;
    char op;  

    // Prompt user for input
    cout << "Enter first number: ";
    cin >> value1;

    cout << "Enter second number: ";
    cin >> value2;

    cout << "Enter operator (+, -, *, /): ";
    cin >> op;

    // Perform calculation based on operator
    switch (op) {
        case '+':
            result = value1 + value2;
            cout << "Result: " << result << endl;
            break;

        case '-':
            result = value1 - value2;
            cout << "Result: " << result << endl;
            break;

        case '*':
            result = value1 * value2;
            cout << "Result: " << result << endl;
            break;

        case '/':
            if (value2 != 0) {
                result = value1 / value2;
                cout << "Result: " << result << endl;
            } else {
                cout << "Division by zero is not allowed." << endl;
            }
            break;

        default:
            cout << "Invalid operator." << endl;
            break;
    }

    return 0;
}
