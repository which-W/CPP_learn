#include<iostream>
#include"myVector.hpp"



int main() {
		myVector<int> a(4, 0);
		int first[4] = { 1,2,3,4 };
		a= myVector<int>(first, first + 5);
		myVector b = a;
		for (int i = 0; i < a.size(); i++) {
			a[i] = i;
			std::cout << a[i] << std::endl;
		}
		std::cout << "........" << std::endl;
		for (int i = 0; i < b.size(); i++) {
			b[i] = i;
			std::cout << b[i] << std::endl;
		}
		a.erase(a.begin() + 2);
		std::cout << "........" << std::endl;
		
}
