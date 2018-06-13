#ifndef _USER
#define _USER

#include<iostream>
#include<cstdio>
#include"Bplustree.h"
#include"mystring.h"
#include"exceptions.h"

class User {
	friend bool Register();
	friend bool login();
	friend bool modify_profile();
	friend bool query_profile();
	friend bool modify_privilege();

	char Name[41], Password[21], Email[21], Phone[21];
	int Privilege, Id;
	void in(){
		scanf("%s%s%s%s", Name, Password, Email, Phone);
	}
	void out(){
		printf("%s %s %s %d\n", Name, Email, Phone, Privilege);
	}
};

sjtu::Bplustree<int, User> UserBpt("UserBpt");

bool Register() {
	User user;
	user.in();
	user.Id = UserBpt.size() + 2018;
	user.Privilege = (user.Id == 2018) ? 2 : 1;
	UserBpt.insert(user.Id, user);
	printf("%d\n", user.Id);
	return 1;
}

bool login() {
	int i;
	char passwd[21];
	scanf("%d%s", &i, passwd);

	auto itU = UserBpt.search(i);
	if (itU.valid() && strcmp((*itU).Password, passwd) == 0) {
		puts("1");
		return 1;
	}
	puts("0");
	return 0;
}

bool query_profile() {
	int i;
	scanf("%d", &i);

	if (!UserBpt.find(i)) {
		puts("0");
		return 0;
	}
	else {
		auto user = *UserBpt.search(i);
		user.out();
		return 1;
	}
}


bool modify_profile() {
	int id;
	scanf("%d", &id);
	auto tmp = UserBpt.search(id);

	if (!tmp.valid()) {
		char t[41];
		scanf("%s%s%s%s", t, t, t, t);
		puts("0");
		return 0;
	}
	(*tmp).in();
	tmp.save();
	puts("1");
	return 1;
}


bool modify_privilege() {
	int i1, i2, p;
	scanf("%d%d%d", &i1, &i2, &p);
	auto itU1 = UserBpt.search(i1);
	if (itU1.valid() && (*itU1).Privilege == 2) {
		auto itU2 = UserBpt.search(i2);
		if(itU2.valid()){
			User &u2 = (*itU2);
			if (u2.Privilege == 2 && p == 1) {
				puts("0");
				return 0;
			}
			if (u2.Privilege == 1 && p == 2) {
				u2.Privilege = 2;
				itU2.save();
			}
			puts("1");
			return 1;
		}
	}
	puts("0");
	return 0;
}


#endif
