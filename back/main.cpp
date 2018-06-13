#include<iostream>
#include<cstdio>
#include<cstdlib>
#include"train.h"
#include"ticket.h"
#include"user.h"

bool clean() {
	TrainBpt.clear();
	StationBpt.clear();
	UserBpt.clear();
	OrderBpt.clear();
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
	"query_ticket",
	"query_transfer",
	"buy_ticket",
	"query_order",
	"refund_ticket",
	"add_train",
	"sale_train",
	"query_train",
	"delete_train",
	"modify_train",
	"clean",
	"exit"
};
bool(*Func[])() = {
	Register,
	login,
	query_profile,
	modify_profile,
	modify_privilege,
	query_ticket,
	query_transfer,
	buy_ticket,
	query_order,
	refund_ticket,
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
	mystring<20> current_command;
	while (current_command.get()) {
	//	std::cerr << current_command << std::endl;
		int i = 0;
		for (; i < command_num; i++) {
			if (current_command == command[i]) {
				Func[i]();
				break;
			}
		}
		if (i == command_num)
			fprintf(stderr, "Not found command %s\n", current_command.c_str());
		fflush(stdout);
	}

	return 0;
}
