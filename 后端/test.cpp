#define  _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include"user.h"
using namespace std;

bool clean() {
	UserBpt.clear();
	puts("1");
	return 1;
}

bool exit() {
	puts("BYE");
	return 1;
}

const mystring<20> command[] = {
	"register",
	"login",
	"query_profile",
	"modify_profile",
	"modify_privilege",
	/*"query_ticket",
	"query_transfer",
	"buy_ticket",
	"query_order",
	"refund_ticket",*/
	/*"add_train",
	"sale_train",
	"query_train",
	"delete_train",
	"modify_train",*/
	"clean",
	"exit"
};
bool(*Func[])() = {
	Register,
	login,
	query_profile,
	modify_profile,
	modify_privilege,
	/*query_ticket,
	query_transfer,
	buy_ticket,
	query_order,
	refund_ticket,*/
	/*add_train,
	sale_train,
	query_train,
	delete_train,
	modify_train,*/
	clean,
	exit
};
const int command_num = sizeof(Func) / sizeof(Func[0]);

int main()
{
	UserBpt.clear();
	mystring<20> current_command;
	int cnt = 0;
	while (cin >> current_command) {
		//printf("Test%d: ", ++cnt);
		cnt++;
		int i = 0;
		bool f = 0;
		for (; i < command_num; i++)
			if (current_command == command[i]) {
				f = 1;
				Func[i]();
				break;
				/*try {
				printf("Run command %s, result is %s\n", command[i].c_str(), Func[i]() ? "True" : "False");
				}
				catch (...) {
				printf("Throw error when test command %s\n", current_command.c_str());
				}*/
			}

		if (i == command_num && !f)
			printf("Not found command %s\n", current_command.c_str());
	}

	return 0;
}
