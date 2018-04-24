#ifndef _USER
#define _USER

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
#include "String.h"
#include "station.h"
using namespace std;



//订单类
class Order {
private:
	String tr_id;
	Station loc1;
	Station loc2;
	String Catalog;
};

//用户类
class User {
private:
	String name;
	String password;
	String email;
	String phone;
	size_t id;
	size_t privilege;
	Order order;
public:
	User() {};
	~User() {};
};

#endif
