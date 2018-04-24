#ifndef _USER
#define _USER

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
#include "String.h"
#include "station.h"
using namespace std;



//������
class Order {
private:
	String tr_id;
	Station loc1;
	Station loc2;
	String Catalog;
};

//�û���
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
