#include <iostream>


int main(void){
	int x = 100;
	int* ptr1 = &x;
	int* ptr2 = ptr1;

	std::cout << "ptr1 = " << *ptr1 << " ptr2 = " << *ptr2 << std::endl;

	*ptr2 = 101;

	std::cout << "ptr1 = " << *ptr1 << " ptr2 = " << *ptr2 << std::endl;
	//the same variable can be changed from both ptr1 and ptr2
}