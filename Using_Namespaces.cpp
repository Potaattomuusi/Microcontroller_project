#include <iostream>
#include <string>
using namespace std;


//Custom namespace
    namespace MathOperations {
        int add(int a, int b) {
        
            return a + b;
        }
    }
    
    namespace TextOperations {
        string concat(string a, string b) {
            return a + b;
        }
    }
    
    int main() {
        cout << "Enter two numbers: \n";
        int a, b;
        cin >> a >> b;
        cout << "Sum: " << MathOperations::add(a, b) << endl;

        cout << "Enter two strings: \n";
        string x, y;
        cin >> x >> y;
        cout << "Concatenated string: " << TextOperations::concat(x, y) << endl;

        return 0;
    }