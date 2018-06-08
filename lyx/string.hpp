#ifndef _STRING_HPP_
#define _STRING_HPP_

#include <cstring>
#include <iostream>
#include <stdexcept>

template<int MAXL>
class string
{
private:
	char data[MAXL + 1];
	int len;
	
public:
	string(): len(0) { memset(data, 0, sizeof(data)); }
	string(const string &s) {
		strcpy(data, s.data);
		len = s.len;
	}
	string(const char *s) {
		strcpy(data, s);
		len = strlen(s);
	}
	template<int L>
	string(const string<L> &s) {
		if (MAXL < L) {
			throw std::invalid_argument("QAQ");
		}
		strcpy(data, s);
		len = strlen(s);
	}
	~string() {}

	string& operator=(const string &s) {
		len = s.len;
		strcpy(data, s.data);
		return *this;
	}
	string& operator=(const char *s) {
		strcpy(data, s);
		len = strlen(s);
		return *this;
	}
	template<int L>
	string& operator=(const string<L> &s) {
		if (MAXL < L) {
			throw std::invalid_argument("QAQ");
		}
		strcpy(data, s);
		len = strlen(s);
	}
	string& operator+=(const string &rhs) {
		if (len + rhs.len > MAXL) {
			throw std::invalid_argument("string operator +");
		}
		strcpy(data + len, rhs.data);
		len += rhs.len;
		return *this;
	}
	string& operator+=(const char *rhs) {
		int l = strlen(rhs);
		if (len + l > MAXL) {
			throw std::invalid_argument("string operator +");
		}
		strcpy(data + len, rhs);
		len += l;
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
		return strcmp(data, rhs.data) == 0;
	}
	bool operator==(const char *rhs) const {
		return *this == string(rhs);
	}
	bool operator<(const string &rhs) const {
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
	string substr(int start, int len) const {
		string res;
		res.len = len;
		strncpy(res.data, data + start, len);
		return res;
	}
	const char *c_str() const { return data; }
	void get() {
		scanf("%s", data);
		len = strlen(data);
	}
};

#endif // _STRING_HPP_
