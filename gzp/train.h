#ifndef _TRAIN
#define _TRAIN

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
#include "String.h"
#include "station.h"
using namespace std;

bool Add_train(String s1, String s2, size_t n1, size_t n2, String s3) {};
//车票类
class Train {
private:
	String trainid;
	size_t num;
	String name;
	Station th[10];
	size_t ticket_type;
	size_t left_ticket[3];
	size_t price;
	bool pub;
public:
	Train() {};
	~Train() {};
	//公开车次
	bool Sale_train() {};
	//删除车次
	bool Delete_train(String s) {};
	//查询车次信息
	String Query_train(String s) {};
	//修改车次
	bool Modefy_train(String s1, size_t n1, size_t n2, String s2) {};
};

#endif
