#include <iostream> 
#include <cmath>    

using namespace std;

// Multiply two integers
int multiply(int a, int b) {
    return a * b;
}

// Multiply two doubles
double multiply(double a, double b) {
    return a * b;
}

// Multiply an integer and a double, with a flag
int multiply(int a, double b, bool flag) {
    double result = a * b;

    if (flag == false) {
        // If flag is false, just return integer part
        return (int)result;
    } else {
        // If flag is true, ceil or floor depending on decimal
        double decimalPart = result - (int)result;

        if (decimalPart >= 0.5) {
            return (int)ceil(result); // round up
        } else {
            return (int)floor(result); // round down
        }
    }
}

int main() {
    // Call the functions with different types of values

    int intResult = multiply(3, 4); 
    double doubleResult = multiply(2.5, 4.2); 
    int mixedResultFalse = multiply(5, 3.7, false); 
    int mixedResultTrue = multiply(5, 3.7, true);  

    cout << "3 * 4 = " << intResult << endl;
    cout << "2.5 * 4.2 = " << doubleResult << endl;
    cout << "5 * 3.7 with flag false = " << mixedResultFalse << endl;
    cout << "5 * 3.7 with flag true = " << mixedResultTrue << endl;

    return 0;
}
