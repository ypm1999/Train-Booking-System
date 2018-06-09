#ifndef _EXCEPTIONS
#define _EXCEPTIONS

#include <iostream>
#include <cstring>
//#include "String.h"

class Exception {
protected:
	const std::string variant = "";
	std::string detail = "";
public:
	Exception() {}
	Exception(const Exception &ec) : variant(ec.variant), detail(ec.detail) {}
	Exception(const std::string& variant, const std::string& detail = "") : variant(variant), detail(detail) {}
	virtual std::string what() {
		return variant + " " + detail;
	}
};

class index_out_of_bound : public Exception {
public:
	index_out_of_bound(const std::string& detail = "") :Exception("Index out of bound", detail) {}
};

class runtime_error : public Exception {
public:
	runtime_error(const std::string& detail = "") :Exception("Runtime error", detail) {}
};

class invalid_iterator : public Exception {
public:
	invalid_iterator(const std::string& detail = "") :Exception("Invalid iterator", detail) {}
};

class container_is_empty : public Exception {
public:
	container_is_empty(const std::string& detail = "") :Exception("Container is empty", detail) {}
};

class open_file_error : public Exception {
public:
	open_file_error(const std::string& detail = "") :Exception("open file error", detail) {}
};

class insert_error : public Exception {
public:
	insert_error(const std::string& detail = "") :Exception("insert error", detail) {}
};

#endif
