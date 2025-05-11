#pragma once
#include<string>
#include<iostream>
class Students
{
public:
	Students(std::string name, int age) :name(name), age(age) {}
	std::string name;
	int age;
};

