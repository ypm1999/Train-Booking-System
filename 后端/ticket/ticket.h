#ifndef _TICKET
#define _TICKET

#include<iostream>
#include<cstdio>
#include"Bplustree.h"
#include"mystring.h"
#include"user.h"
#include"sort.h"
#include "vector.hpp"
#include"train.h"

class Order {
public:
	int Id, dateNum, Num;
	char Catalog[2], TrainId[21], TicketKind[21], Loc1[41], Loc2[41], Date[11];
	bool operator<(const Order &other)const {
		if (Id > other.Id) return 0;
		if (Id < other.Id) return 1;
		int cmp = strcmp(Catalog, other.Catalog);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(Date, other.Date);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(Loc1, other.Loc1);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(Loc2, other.Loc2);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(TicketKind, other.TicketKind);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(TrainId, other.TrainId);
		if (cmp < 0) return 1;
		return 0;
	}
	bool operator>(const Order &other) const {
		if (Id > other.Id) return 1;
		if (Id < other.Id) return 0;
		int cmp = strcmp(Catalog, other.Catalog);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		if (dateNum > other.dateNum) return 1;
		if (dateNum < other.dateNum) return 0;
		cmp = strcmp(Loc1, other.Loc1);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		cmp = strcmp(Loc2, other.Loc2);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		cmp = strcmp(TicketKind, other.TicketKind);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		cmp = strcmp(TrainId, other.TrainId);
		if (cmp > 0) return 1;
		return 0;
	}
	bool operator==(const Order &other) const {
		return !(*this < other && *this > other);
	}
public:
	Order() {
		dateNum = -1;
		Num = 0;
	}
	Order(const int &i, char *c, char *d, char *l1, char *l2, char *tk, char *Tid) {
		Id = i;
		strcpy(Catalog, c);
		strcpy(Loc1, l1);
		strcpy(Loc2, l2);
		strcpy(Date, d);
		strcpy(TicketKind, tk);
		strcpy(TrainId, Tid);
		dateNum = -1;
		Num = 0;
	}
};


sjtu::Bplustree<Order, Order> OrderBpt("OrderBpt");

struct ticket {
	char trainId[21], num, ticketKind[5][41];
	int leftTicket[5];
	double price[5];
	int date1, date2, time1, time2;
	ticket() {}
	ticket(const char *id, int _num, char *date, int t1, int t2) {
		strcpy(trainId, id);
		num = _num;
		date1 = (date[8] - '0') * 10 + date[9] - '0';
		date2 = date1;
		time1 = t1;
		time2 = t2;
		while (time1 >= 1440) {
			time1 -= 1440;
			time2 -= 1440;
		}
		while (time2 >= 1440) {
			time2 -= 1440;
			date2++;
		}
	}
	friend bool operator <(const ticket &a, const ticket &b) {
		return strcmp(a.trainId, b.trainId) < 0;
	}
	void todate(int data, char a[]) const {
		sprintf(a, "2018-06-%02d", data);
	}
	void totime(int tim, char a[]) const {
		if (tim == -1)
			strcpy(a, "xx:xx");
		else
			sprintf(a, "%02d:%02d", tim / 60, tim % 60);
	}
	void print(char loc1[], char loc2[]) const {
		char date[12], time[6];
		todate(date1, date);
		totime(time1, time);
		printf("%s %s %s %s ", trainId, loc1, date, time);
		todate(date2, date);
		totime(time2, time);
		printf("%s %s %s ", loc2, date, time);
		for (int i = 0; i < num; i++)
			printf("%s %d %lf%c", ticketKind[i], leftTicket[i], price[i], " \n"[i == num - 1]);
	}
	int get_arrive_time() const {
		return date1 * 1440 + time1;
	}
	int get_start_time() const {
		return date2 * 1440 + time2;
	}
};


void sort(int a[], int l, int r);
vector<ticket> tmp;
bool cmp(const int &a, const int &b) {
	return tmp[a] < tmp[b];
}

bool cmp_arrive_time(const ticket &a, const ticket &b) {
	return a.get_arrive_time() < b.get_arrive_time();
}

bool cmp_start_time(const ticket &a, const ticket &b) {
	return a.get_arrive_time() < b.get_arrive_time();
}


void getleftticket(char *id, int l, int r, int n, int date, int left[], double pri[]) {
	auto it = TrainBpt.search(TrainKey(id, l));
	memset(left, 0x3f, sizeof(*left) * 5);
	memset(pri, 0, sizeof(*pri) * 5);
	int now = 0;
	auto tmp = it;
	while (true) {
		tmp = it;
		if(tmp.key().nth > l)
			for (int i = 0; i < n; i++)
				pri[i] += tmp->Price[i];
		it++;
		if (!(strcmp(id, it.key().TrainId) == 0 && it.key().nth <= r))
			break;
		for (int i = 0; i < n; i++) {
			left[i] = min(left[i], tmp->leftTicket[i][date]);
		}
		if(it->start_time - now >= 1440)
			date++, now += 1440;
	}
}

void getticket(char loc1[], char loc2[], char date[], char cat, vector<ticket> &tmp,
	sjtu::Bplustree<StationKey, StationNode>::Iterator it1,
	sjtu::Bplustree<StationKey, StationNode>::Iterator it2){
	if (!it1.valid() || it1.key().catalog != cat || strcmp(it1.key().StationName, loc1) != 0)
		return;
	if (!it2.valid() || it2.key().catalog != cat || strcmp(it2.key().StationName, loc2) != 0)
		return;
	while (true) {
		mystring<20> id1(it1.key().TrainId), id2(it2.key().TrainId);
		if (id1 == id2 && it1->nth < it2.data().nth) {
			int n = it1->PriceNum;
			ticket t(id1.c_str(), n, date, it1->start_time, it2->arr_time);
			getleftticket(const_cast<char*>(id1.c_str()), it1->nth, it2->nth, n, t.date1 - 1, t.leftTicket, t.price);
			for (int i = 0; i < n; i++)
				strcpy(t.ticketKind[i], it1->Catalog[i]);
			tmp.push_back(t);
			it1++;
			if (!it1.valid() || it1.key().catalog != cat || strcmp(it1.key().StationName, loc1) != 0)
				break;
			it2++;
			if (!it2.valid() || it2.key().catalog != cat || strcmp(it2.key().StationName, loc2) != 0)
				break;
		}
		else {
			if (id1 < id2) {
				it1++;
				if (!it1.valid() || it1.key().catalog != cat || strcmp(it1.key().StationName, loc1) != 0)
					break;
			}
			else {
				it2++;
				if (!it2.valid() || it2.key().catalog != cat || strcmp(it2.key().StationName, loc2) != 0)
					break;
			}

		}
	}
}
bool check(char loc1[], char loc2[]) {
	auto it1 = StationBpt.lower_bound(StationKey(loc1, 0));
	auto it2 = StationBpt.lower_bound(StationKey(loc2, 0));
	if (it1.valid() && it2.valid() && strcmp(it1.key().StationName, loc1) == 0 && strcmp(it2.key().StationName, loc2) == 0)
		return 1;
	return 0;
}

bool query_ticket() {
	char loc1[41], loc2[41], date[12], catelog[11];
	scanf("%s%s%s%s", loc1, loc2, date, catelog);
	if (!(StationBpt.lower_bound(StationKey(loc1, 0)).valid() &&
		StationBpt.lower_bound(StationKey(loc2, 0)).valid())) {
		puts("-1");
		return 0;
	}
	tmp.clear();
	for (int now = 0; now < strlen(catelog); now++) {
		auto it1 = StationBpt.lower_bound(StationKey(loc1, catelog[now]));
		auto it2 = StationBpt.lower_bound(StationKey(loc2, catelog[now]));
		getticket(loc1, loc2, date, catelog[now], tmp, it1, it2);
	}
	int cnt = tmp.size();
	int *a = new int[cnt + 2];
	for (int i = 0; i < cnt; i++)
		a[i] = i;
	sort(a, 0, cnt - 1, cmp);
	printf("%d\n", (int)tmp.size());
	for (int i = 0; i < tmp.size(); i++)
		tmp[a[i]].print(loc1, loc2);
	delete[] a;
	return 1;
}

void try_transfer(char loc1[], char loc2[], char loc[],
char ans[], char date[], char catelog[], ticket res[], int &mn) {
	for(int now = 0; now < strlen(catelog); now++) {
		auto it1 = StationBpt.lower_bound(StationKey(loc1, catelog[now]));
		auto it2 = StationBpt.lower_bound(StationKey(loc2, catelog[now]));
		auto it = StationBpt.lower_bound(StationKey(loc, catelog[now]));
		if (!(it1.valid() && it2.valid() && it.valid()))
			return;
		vector<ticket> t1, t2;
		getticket(loc1, loc, date, catelog[now], t1, it1, it);
		getticket(loc, loc2, date, catelog[now], t2, it, it2);
		int n1 = t1.size(), n2 = t2.size();
		t1.sort(cmp_arrive_time);
		t2.sort(cmp_start_time);
		for (int i = 0; i < n1; i++) {
			int tim1 = t1[i].get_arrive_time();
			int st = t1[i].get_start_time();
			for (int j = 0; j < n2; j++) {
				int tim2 = t2[j].get_arrive_time();
				int ed = t2[j].get_arrive_time();
				if (tim1 <= tim2 && ed - st < mn) {
					res[0] = t1[i];
					res[1] = t2[j];
					strcpy(ans, loc);
					mn = ed - st;
				}
			}
		}
	}

}


bool query_transfer() {
	char loc1[41], loc2[41], loc[41], date[12], catelog[11];
	scanf("%s%s%s%s", loc1, loc2, date, catelog);
	if (!check(loc1, loc2)) {
		puts("-1");
		return 0;
	}
	auto it = StationBpt.lower_bound(StationKey());
	int res = 0x3f3f3f3f;
	ticket *now = new ticket[2];
	while (it.valid()) {
		try_transfer(loc1, loc2, it.key().StationName, loc, date, catelog, now, res);
		it++;
	}
	if (res == 0x3f3f3f3f) {
		puts("-1");
		delete[] now;
		return 0;
	}
	else {
		now[0].print(loc1, loc);
		now[1].print(loc, loc2);
		delete[] now;
		return 1;
	}
}
bool query_order() { return false; }


	bool refund_ticket() {

		return 1;
	}

	bool buy_ticket() {

		return 1;
	}

#endif
