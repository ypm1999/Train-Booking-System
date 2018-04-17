#ifndef _USER
#define _USER

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
#include "String.h"
#include "station.h"
using namespace std;

//查询车票
String Query_ticket(Station s1, Station s2, String cata) {};
//查询中转车票
String Query_tranfer(Station s1, Station s2, String cata) {};

//订单类
class Order {
private:
	String tr_id;
	Station loc1;
	Station loc2;
	String Catalog;
};

//注册用户
size_t Register(String s1, String s2, String s3, String s4) {};
//登陆
bool Login(String s1, String s2) {};

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
	//查询用户信息
	String Query_profile() {};
	//修改用户信息
	bool Modify_profile(String s1, String s2, String s3, String s4) {};
	//修改用户权限
	bool Modify_privilege(size_t i, size_t p) {};
	//购买车票
	bool Buy_ticket(size_t n, String s1, Station s2, Station s3, String s4) {};
	//查询购票信息
	void Query_order(String cata) {};
	//退订车票
	bool Refund_ticket(size_t n, String s1, Station s2, Station s3, String s4) {};
};

#endif
