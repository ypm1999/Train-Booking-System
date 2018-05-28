#ifndef PBS_EXCEPTION_HPP
#define PBS_EXCEPTION_HPP

#include "PBSBase.hpp"
#include "PBSDebug.hpp"

struct PBSException{
	char message[MAX_EXCEPTION_MESSAGE_LENGTH];
	PBSException() {}
	PBSException(const std::string &message) {
		int length = message.length();
		memcpy(this -> message, message.c_str(), length);
		this -> message[length] = '\0';
	}
	void print()const {
#ifdef COLOREXCEPTION
		fprintf(stdout, "\033[1;31mException: \033[;37m%s\033[;0m\n", message.c_str());
#else
		fprintf(stdout, "Exception: %s\n", message);
#endif
	}
};

#endif
