#define  _CRT_SECURE_NO_WARNINGS
#define _NO_CRT_STDIO_INLINE
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include"train.h"
#include"station.h"
#include"ticket.h"
using namespace std;


const mystring<20> command[] = {
	/*"register",
	"login",
	"query_profile",
	"modify_profile",
	"modify_privilege",*/
	/*"query_ticket",
	"query_transfer",
	"buy_ticket",
	"query_order",
	"refund_ticket",*/
	"add_train",
	"sale_train",
	"query_train",
	"delete_train",
	"modify_train",
	"clean",
	"exit"
};
bool(*Func[])() = {
	/*Register,
	login,
	query_profile,
	modify_profile,
	modify_privilege,*/
	/*query_ticket,
	query_transfer,
	buy_ticket,
	query_order,
	refund_ticket,*/
	add_train,
	sale_train,
	query_train,
	delete_train,
	modify_train,
	clean,
	exit
};
const int command_num = sizeof(Func) / sizeof(Func[0]);

int main()
{
	freopen("data.in", "r", stdin);
	mystring<20> current_command;
	int cnt = 0;
	while (cin >> current_command) {
		//printf("Test%d: ", ++cnt);
		cnt++;
		int i = 0;
		for (; i < command_num; i++)
			if (current_command == command[i]) {
				try {
					printf("Run command %s, result is %s\n", command[i].c_str(), Func[i]() ? "True" : "False");
				}
				catch (...) {
					printf("Throw error when test command %s\n", current_command.c_str());
				}
				break;
			}

		if (i == command_num)
			printf("Not found command %s\n", current_command.c_str());
	}

	return 0;
}
