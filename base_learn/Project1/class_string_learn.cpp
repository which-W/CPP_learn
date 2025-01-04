#include<iostream>
#include<cstring>

using namespace std;

class String
{
public:
	String();
	String(const char * s);
	~String();
	size_t getlength() const;
	char operator[](size_t index) const;
	String &operator=(const String& s);
	bool operator==(const String& s) const;
	bool operator!=(const String& s) const;
	String copy() const;
	String operator+(const String& s)const;
	friend ostream&operator<<(ostream& os, const String& s);
private:
	char* str;
	size_t length;
};

String::String()
{
	length = 0;
	str = new char[1];
	str[0] = '\0';
}


String::String(const char* s) {
	length = strlen(s);
	str = new char[length + 1];
	strcpy(str , s);
};


char String::operator[](size_t index) const {
	return str[index];
};

String& String::operator=(const String& s) {
	if (this != &s) {
		delete[] str;
		length = s.length;
		str = new char[length + 1];
		str = s.str;
	}
	return *this;
};
size_t String::getlength() const {
	return length;
};

bool String::operator==(const String& s) const{
	return strcmp(str, s.str) == 0;
};


bool String::operator!=(const String& s) const{
	return strcmp(str, s.str) != 0;
}

String String::copy() const {
	String s = *this;
	return s;
};

String String::operator+(const String& s) const{
	String result;
	result.length = length + s.length;
	result.str = new char[result.length + 1];
	strcpy(result.str, str);
	strcat(result.str, s.str);
	return result;
};

ostream&operator<<(ostream& os, const String& s) {
	os << s;
	return os;
};
String::~String()
{
	delete[] str;
}