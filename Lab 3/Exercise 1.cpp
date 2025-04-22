#include <iostream>
using namespace std;

class Book {
private:
    string title;
    int pages;
    string author = "Steven";

protected:
    float price;

public:
    string publisher;

    void displayAuthorInfo() {
        cout << "Author: " << author << endl;
    }
};

int main() {
    Book b;

    // A. Trying to access private member will cause compile error
    // b.title = "Redstone Guide"; // Uncomment to trigger error
    // cout << "Title: " << b.title << endl;

    // B. Publisher is a public member, so it can be accessed directly
    b.publisher = "Mojang";
    cout << "Publisher: " << b.publisher << endl;

    // C. dispalyAuthorInfo() is a public member function, so it can be accessed directly
    // changing to private will result in compile error
    b.displayAuthorInfo();

    return 0;
}

// D.
// Private: accessible only within the class
//         -Keeps data safe from outside
// Protected: accessible within the class and derived classes
//         -Keeps data safe from outside but allows derived classes to access it
// Public: accessible from anywhere in the program
//         -Allows data to be accessed from outside the class

