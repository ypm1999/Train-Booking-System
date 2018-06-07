#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <cstring>
#include <iostream>
#include <stdexcept>

template<int MAXL>
class string
{
private:
	char data[MAXL];
	int len;
	
public:
	string(): len(0) { memset(data, 0, MAXL); }
	string(const string &s): len(0) {
		for (int i = 0; i < s.len; i++) {
			data[len++] = s.data[i];
		}
	}
	string(const char *s): len(0) {
		for (int i = 0; s[i]; i++)
			data[len++] = s[i];
	}
	~string() {}

	string& operator=(const string &s) {
		len = 0;
		for (int i = 0; i < s.len; i++) {
			data[len++] = s.data[i];
		}
		return *this;
	}
	string& operator=(const char *s) {
		len = 0;
		for (int i = 0; s[i]; i++) {
			data[len++] = s[i];
		}
		return *this;
	}
	string& operator+=(const string &rhs) {
		if (len + rhs.len > MAXL) {
			throw std::invalid_argument("string operator +");
		}
		for (int i = 0; i < rhs.len; i++)
			data[len++] = rhs.data[i];
		return *this;
	}
	string& operator+=(const char *rhs) {
		if (len + strlen(rhs) > MAXL) {
			throw std::invalid_argument("string operator +");
		}
		for (int i = 0; rhs[i]; i++)
			data[len++] = rhs[i];
		return *this;
	}
	friend string operator+(const char *lhs, const string &rhs) {
		return string(lhs) + rhs;
	}
	string operator+(const string &rhs) const {
		string res = *this;
		return res += rhs;
	}
	string operator+(const char *rhs) const {
		string res = *this;
		return res += rhs;
	}
	friend bool operator==(const char *lhs, const string &rhs) {
		return rhs == lhs;
	}
	bool operator==(const string &rhs) const {
		if (len != rhs.len) return false;
		for (int i = 0; i < len; i++)
			if (data[i] != rhs.data[i])
				return false;
		return true;
	}
	bool operator==(const char *rhs) const {
		return *this == string(rhs);
	}
	char& operator[](const int &n) {
		return data[n];
	}
	const char& operator[](const int &n) const {
		return data[n];
	}
	friend std::istream& operator>>(std::istream &in, string &s) {
		static char c[MAXL + 1];
		in >> c;
		s = string(c);
		return in;
	}
	friend std::ostream& operator<<(std::ostream &out, const string &s) {
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
	string substr(int start, int len) {
		string res;
		for (int i = 0; i < len; i++) {
			res.data[res.len++] = data[start + i];
		}
		return res;
	}
	
	char *c_str() const { return data; }
	
	void print() const {
		for (int i = 0; data[i] && i < MAXL; i++)
			printf("%c", data[i]);
	}
};

#endif // _STRING_HPP_
