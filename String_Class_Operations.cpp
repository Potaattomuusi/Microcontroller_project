#include <iostream>
#include <string>
#include <algorithm> // For transform and reverse
using namespace std;

// Function to count vowels in a string
int countVowels(const string& str) {
    int count = 0;
    for (char ch : str) {
        char lowerCh = tolower(ch);
        if (lowerCh == 'a' || lowerCh == 'e' || lowerCh == 'i' || lowerCh == 'o' || lowerCh == 'u') {
            count++;
        }
    }
    return count;
}

int main() {
    string input;

    // Take input from the user
    cout << "Enter a string: ";
    getline(cin, input);

    // Convert the string to uppercase
    string upperCaseStr = input;
    transform(upperCaseStr.begin(), upperCaseStr.end(), upperCaseStr.begin(), ::toupper);
    cout << "Uppercase: " << upperCaseStr << endl;

    // Count the number of vowels
    int vowelCount = countVowels(input);
    cout << "Number of vowels: " << vowelCount << endl;

    // Reverse the string
    string reversedStr = input;
    reverse(reversedStr.begin(), reversedStr.end());
    cout << "Reversed string: " << reversedStr << endl;

    return 0;
}
