#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <cstring>
#include <iostream>
#include <stdexcept>

template<int MAXL>
class mystring
{
private:
	char data[MAXL];
	int len;
	
public:
	mystring(): len(0) { data[0] = 0; }
	mystring(const mystring &s): len(0) {
		for (int i = 0; i < s.len; i++) {
			data[len++] = s.data[i];
		}
	}
	mystring(const char *s): len(0) {
		for (int i = 0; s[i]; i++)
			data[len++] = s[i];
	}
	~mystring() {}

	mystring& operator=(const mystring &s) {
		len = 0;
		for (int i = 0; i < s.len; i++) {
			data[len++] = s.data[i];
		}
		return *this;
	}
	mystring& operator=(const char *s) {
		len = 0;
		for (int i = 0; s[i]; i++) {
			data[len++] = s[i];
		}
		return *this;
	}
	mystring& operator+=(const mystring &rhs) {
		if (len + rhs.len > MAXL) {
			throw std::invalid_argument("mystring operator +");
		}
		for (int i = 0; i < rhs.len; i++)
			data[len++] = rhs.data[i];
		return *this;
	}
	mystring& operator+=(const char *rhs) {
		if (len + strlen(rhs) > MAXL) {
			throw std::invalid_argument("mystring operator +");
		}
		for (int i = 0; rhs[i]; i++)
			data[len++] = rhs[i];
		return *this;
	}
	friend mystring operator+(const char *lhs, const mystring &rhs) {
		return mystring(lhs) + rhs;
	}
	mystring operator+(const mystring &rhs) const {
		mystring res = *this;
		return res += rhs;
	}
	mystring operator+(const char *rhs) const {
		mystring res = *this;
		return res += rhs;
	}
	friend bool operator==(const char *lhs, const mystring &rhs) {
		return rhs == lhs;
	}
	bool operator==(const mystring &rhs) const {
		if (len != rhs.len) return false;
		for (int i = 0; i < len; i++)
			if (data[i] != rhs.data[i])
				return false;
		return true;
	}
	bool operator==(const char *rhs) const {
		return *this == mystring(rhs);
	}
	char& operator[](const int &n) {
		return data[n];
	}
	const char& operator[](const int &n) const {
		return data[n];
	}
	friend std::istream& operator>>(std::istream &in, mystring &s) {
		static char c[MAXL + 1];
		in >> c;
		s = mystring(c);
		return in;
	}
	friend std::ostream& operator<<(std::ostream &out, const mystring &s) {
		for (int i = 0; i < s.len; i++)
			out << s.data[i];
		return out;
	}
	int size() const {
		return len;
	}
	int length() const {
		return len;
	}
	mystring substr(int start, int len) {
		mystring res;
		for (int i = 0; i < len; i++) {
			res.data[res.len++] = data[start + i];
		}
		return res;
	}
	char *c_str() {
		return data;
	}
};

#endif // _STRING_HPP_

