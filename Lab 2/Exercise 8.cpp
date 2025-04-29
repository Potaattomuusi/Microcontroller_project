#include <iostream>

int main(void){
	int x = 100;
	int& refx = x;
	std::cout << x << std::endl;
	refx = 101;
	std::cout << x << std::endl;
	return 0;
	//changing the reference changes the actual variable
}