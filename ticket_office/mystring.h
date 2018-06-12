#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <cstring>
#include <iostream>
#include <stdexcept>

template<int MAXL>
class mystring
{
private:
	char data[MAXL + 1];
	int len;
	
public:
	mystring(): len(0) { memset(data, 0, sizeof(data)); }
	mystring(const mystring &s) {
		strcpy(data, s.data);
		len = s.len;
	}
	mystring(const char *s) {
		strcpy(data, s);
		len = strlen(s);
	}
	template<int L>
	mystring(const mystring<L> &s) {
		if (MAXL < L) {
			throw std::invalid_argument("QAQ");
		}
		strcpy(data, s);
		len = strlen(s);
	}
	~mystring() {}

	mystring& operator=(const mystring &s) {
		len = s.len;
		strcpy(data, s.data);
		return *this;
	}
	mystring& operator=(const char *s) {
		strcpy(data, s);
		len = strlen(s);
		return *this;
	}
	template<int L>
	mystring& operator=(const mystring<L> &s) {
		if (MAXL < L) {
			throw std::invalid_argument("QAQ");
		}
		strcpy(data, s);
		len = strlen(s);
	}
	mystring& operator+=(const mystring &rhs) {
		if (len + rhs.len > MAXL) {
			throw std::invalid_argument("mystring operator +");
		}
		strcpy(data + len, rhs.data);
		len += rhs.len;
		return *this;
	}
	mystring& operator+=(const char *rhs) {
		int l = strlen(rhs);
		if (len + l > MAXL) {
			throw std::invalid_argument("mystring operator +");
		}
		strcpy(data + len, rhs);
		len += l;
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
		return strcmp(data, rhs.data) == 0;
	}
	bool operator==(const char *rhs) const {
		return *this == mystring(rhs);
	}
	bool operator<(const mystring &rhs) const {
		return strcmp(data, rhs.data) < 0;
	}
	bool operator<(const char *rhs) const {
		return strcmp(data, rhs) < 0;
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
	mystring substr(int start, int len) const {
		mystring res;
		res.len = len;
		strncpy(res.data, data + start, len);
		return res;
	}
	const char *c_str() const { return data; }
	bool get() {
		if (scanf("%s", data) != 1)
			return 0;
		len = strlen(data);
		return 1;
	}
};

#endif // _STRING_HPP_
