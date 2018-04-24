#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "statement.h"
#include "exceptions.h"
using namespace std;

//注册用户
size_t Register(String _name, String _password, String _email, String _phone) {};

//登陆
bool Login(size_t _id, String password) {};

//查询用户信息
String Query_profile(size_t _id) {};

//修改用户信息
String Modify_profile(size_t _id, String _name, String _password, String _email, String _phone) {};

//修改用户权限
bool Modify_privilege(size_t _id1, size_t _id2, size_t _privilege) {};

//查询车票
String Query_ticket(size_t loc1, size_t loc2, size_t cata) {};

//查询中转车票
String Query_tranfer(size_t loc1, size_t loc2, size_t cata) {};

//购买车票
bool Buy_ticket(size_t _id, String _num, size_t loc1, size_t loc2, size_t _t_k) {};

//查询购票信息
void Query_order(size_t _id, size_t cata) {};

//退订车票
bool Refund_ticket(size_t _id, size_t _num, size_t _loc1, size_t _loc2, String s4) {};

//新建车次
bool Add_train(String s1, String s2, size_t n1, size_t n2, String s3) {};

//公开车次
bool Sale_train() {};

//删除车次
bool Delete_train(String s) {};

//查询车次信息
String Query_train(String s) {}

//修改车次
bool Modefy_train(String s1, size_t n1, size_t n2, String s2) {};

void clean() {};

void exit() {};

int main()
{
	char ch;
	String s = "";
	ifstream in;
	in.open("file");
	if (!in) throw open_file_error();
	while (!in.eof) {
		in >> ch;
		if (ch == ' ') break;
		s = s + ch;
	}
	if (s == "register") {
		String s1 = "", s2 = "", s3 = "", s4 = "";
		while (!in.eof) {
			in >> ch;
			if (ch == ' ') break;
			s1 = s1 + ch;
		}
		while (!in.eof) {
			in >> ch;
			if (ch == ' ') break;
			s2 = s2 + ch;
		}
		while (!in.eof) {
			in >> ch;
			if (ch == ' ') break;
			s3 = s3 + ch;
		}
		while (!in.eof) {
			in >> ch;
			if (ch == ' ') break;
			s4 = s4 + ch;
		}
		Register(s1, s2, s3, s4);
	}
	else if (s == "login") {

	}
	else if (s == "query_profile") {

	}
	else if (s == "modify_profile") {

	}
	else if (s == "modify_privilege") {

	}
	else if (s == "query_ticket") {

	}
	else if (s == "query_tranfer") {

	}
	else if (s == "buy_ticket") {

	}
	else if (s == "query_order") {

	}
	else if (s == "refund_ticket") {

	}
	else if (s == "add_train") {

	}
	else if (s == "sale_train") {

	}
	else if (s == "query_train") {

	}
	else if (s == "delete_train") {

	}
	else if (s == "modify_train") {

	}
	else if (s == "clean") {

	}
	else if (s == "exit") {

	}
}