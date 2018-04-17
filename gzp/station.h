#ifndef _STATION
#define _STATION

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
#include "String.h"
#include "set.h"
using namespace std;
//≥µ’æ¿‡
class Station {
private:
	size_t index;
	String name;
	Set *in[20], *out[20];
};

#endif