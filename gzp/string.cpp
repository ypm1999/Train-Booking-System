#include "String.h"

String::String(const char *str) {
	if (!str) {
		_size = 0;
		ch = NULL;
	}
	else {
		_size = strlen(str);
		ch = new char[_size + 1];
		strcpy(ch, str);
		ch[_size] = '\0';
	}
}

String::String(String && str) {
	_size = str._size;
	ch = str.ch;
	str.ch = NULL;
}

String::String(const String &str) {
	if (this == &str) return;
	if (!str.size()) {
		_size = 0;
		ch = NULL;
	}
	else {
		_size = str._size;
		ch = new char[_size + 1];
		strcpy(ch, str.ch);
	}
}

String::~String() {
	if (ch) delete ch;
}

String& String::operator=(const char *str) {
	if (ch == str) return *this;
	if (!str) {
		_size = 0;
		ch = NULL;
	}
	else {
		_size = strlen(str);
		ch = new char[_size + 1];
		strcpy(ch, str);
		ch[_size] = 0;
	}
	return *this;
}

String& String::operator=(const String &str) {
	if (this == &str) return *this;
	if (!str.size()) {
		_size = 0;
		ch = NULL;
	}
	else {
		_size = str._size;
		ch = new char[_size + 1];
		strcpy(ch, str.ch);
	}
	return *this;
}

bool String::operator==(const char *str) {
	if (strcmp(ch, str) == 0) return true;
	else return false;
}

bool String::operator==(const String &str) {
	if (strcmp(ch, str.ch) == 0) return true;
	else return false;
}

char& String::operator[](size_t p){
	return ch[p];
}

String String::operator+(const char *str) {
	String tmp(*this);
	tmp._size += strlen(str);
	strcat(tmp.ch, str);
	tmp.ch[_size] = '\0';
	return std::move(tmp);
}

String String::operator+(const char c) {
	String tmp(*this);
	tmp.ch[_size] = c;
	tmp._size++;
	tmp.ch[_size] = '\0';
	return std::move(tmp);
}

String String::operator+(const String &str) {
	String tmp(*this);
	strcat(tmp.ch, str.ch);
	tmp._size += str._size - 1;	
	tmp.ch[_size] = '\0';
	return std::move(tmp);
}