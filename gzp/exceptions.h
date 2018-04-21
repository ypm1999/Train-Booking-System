#ifndef _EXCEPTIONS
#define _EXCEPTIONS

#include <iostream>
#include "String.h"
using namespace std;

class Exception {
protected:
	const String variant;
	String detail;
public:
	Exception() {}
	Exception(const Exception &ec) : variant(ec.variant), detail(ec.detail){}
	virtual String what() {}
};

class index_out_of_bound : public exception {
	/* __________________________ */
};

class runtime_error : public exception {
	/* __________________________ */
};

class invalid_iterator : public exception {
	/* __________________________ */
};

class container_is_empty : public exception {
	/* __________________________ */
};

#endif
