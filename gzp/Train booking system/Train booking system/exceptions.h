#ifndef _EXCEPTIONS
#define _EXCEPTIONS

#include <iostream>
#include <cstring>
//#include "String.h"
using namespace std;

class Exception {
protected:
	const string variant = "";
	string detail = "";
public:
	Exception() {}
	Exception(const Exception &ec) : variant(ec.variant), detail(ec.detail){}
	virtual string what() {
		return variant + " " + detail;
	}
};

class index_out_of_bound : public Exception {
	/* __________________________ */
};

class runtime_error : public Exception {
	/* __________________________ */
};

class invalid_iterator : public Exception {
	/* __________________________ */
};

class container_is_empty : public Exception {
	/* __________________________ */
};

class open_file_error : public Exception {
	/* __________________________ */
};

class insert_error : public Exception {
	/* ___________________________*/
};

#endif
