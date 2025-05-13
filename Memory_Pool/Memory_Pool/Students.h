#pragma once
#include<string>
#include<iostream>
class Students
{
public:
	Students(std::string name, int age) :name(name), age(age) {}
	~Students()
	{
		std::cout << "Destructor called for " << name << std::endl;
	}
	std::string name;
	int age;
};

