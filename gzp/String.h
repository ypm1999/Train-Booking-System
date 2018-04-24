#ifndef _STRING
#define _STRING

#include <iostream>;
#include <cstring>
using namespace std;
//¼òµ¥string
class String{
private:
	char *ch;
	size_t _size;
public:
	String() {
		_size = 0;
		ch[0] = '\0';
	};
	~String() {};

	String(const char *);
	String(const String &);
	String(String &&);

	String& operator=(const char *);
	String& operator=(const String &);

	bool operator==(const char *);
	bool operator==(const String &);

	char &operator[](size_t);
	
	String operator+(const char *);
	String operator+(const char c);
	String operator+(const String &);

	size_t size() const { return _size; }
	char *c_str() const { return ch; }
};

#endif
