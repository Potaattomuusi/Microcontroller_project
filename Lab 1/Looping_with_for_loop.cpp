#include <iostream>
using namespace std;

#define MAX_LIMIT 50 // Macro for the upper limit

int main() {
    for (unsigned int i = 2; i <= MAX_LIMIT; i += 2) { // Using unsigned int for non-negative values
        cout << i << " ";
    }
    return 0;
}