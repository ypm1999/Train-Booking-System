#ifndef _TRAIN
#define _TRAIN

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
#include "String.h"
#include "station.h"
using namespace std;


//≥µ∆±¿‡
class Train {
private:
	String trainid;
	size_t num;
	String name;
	Station th[10];
	size_t catalog;
	size_t left_ticket[5];
	size_t price[5];
	bool pub;
public:
	Train() {};
	~Train() {};
};

#endif
