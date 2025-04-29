#include <iostream>
using namespace std;

int main() {
    const int size = 5;
    int array1[size] = {1, 4, 7, 10, 15};
    int array2[size]; //other array to store reversed valuesm
    int* ptr = array1; // pointer to the start of array1

    // Traverse array1 using the pointer and store values in reverse order in array2
    for (int i = 0; i < size; i++) {
        array2[size - 1 - i] = *(ptr + i); // dereference the pointer to get value
    }

    // Print the new array
    cout << "Reversed array: ";
    for (int i = 0; i < size; i++) {
        cout << array2[i] << " ";
    }
    cout << endl;

    return 0;
}
