#include <iostream>
using namespace std;

// 1. Swap without pointer or reference
void swap1(float a, float b) {
    float temp = a;
    a = b;
    b = temp;
    cout << "Inside swap1: (only local swap, does not affect original values)" << endl;
    cout << "Now a = " << a << ", b = " << b << endl;
}

// 2. Swap using pointers
void swap2(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

// 3. Swap using references
void swap3(float &a, float &b) {
    float temp = a;
    a = b;
    b = temp;
}

int main() {
    float x = 5.5, y = 10.1;

    cout << "Starting values: x = " << x << ", y = " << y << endl << endl;

    // Try swap without pointer/reference
    cout << "Trying swap1 (without pointers or references)..." << endl;
    swap1(x, y);
    cout << "After swap1, original values are still: x = " << x << ", y = " << y << endl << endl;

    // Try swap using pointers
    cout << "Now trying swap2 (using pointers)..." << endl;
    swap2(&x, &y);
    cout << "After swap2, new values: x = " << x << ", y = " << y << endl << endl;

    // Try swap using references
    cout << "Finally, trying swap3 (using references)..." << endl;
    swap3(x, y);
    cout << "After swap3, final values: x = " << x << ", y = " << y << endl << endl;


    return 0;
}
