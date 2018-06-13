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

struct ticket {
	char trainId[21], num, ticketKind[5][21];
	short leftTicket[5];
	double price[5];
	short date1, date2, time1, time2;
	ticket() {}
	ticket(const char *id, int _num, int date0, int t1, int t2) {
		strcpy(trainId, id);
		num = _num;
		date1 = date0;
		time1 = t1;
		time2 = t2;
		while (time1 >= 1440) {
			time1 -= 1440;
			time2 -= 1440;
			date1++;
		}
		date2 = date1;
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


void getleftticket(char *id, int l, int r, int n, int date, short left[], double pri[]) {
	auto it = TrainBpt.search(TrainKey(id, l));
	memset(left, 0x3f, sizeof(*left) * 5);
	memset(pri, 0, sizeof(*pri) * 5);
	auto tmp = it;
	while (true) {
		tmp = it;
		if(tmp.key().nth > l)
			for (int i = 0; i < n; i++)
				pri[i] += tmp->Price[i];
		it++;
		if (!(strcmp(id, it.key().TrainId) == 0 && it.key().nth <= r))
			break;
		int t = date + tmp->start_time / 1440;
		for (int i = 0; i < n; i++) {
			if (left[i] > tmp->leftTicket[i][t])
				left[i] = tmp->leftTicket[i][t];
		}
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
			int date0 = (date[8] - '0') * 10 + date[9] - '0';
			ticket t(id1.c_str(), n, date0, it1->start_time, it2->arr_time);
			getleftticket(const_cast<char*>(id1.c_str()), it1->nth, it2->nth, n, date0, t.leftTicket, t.price);
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
	for (int now = 0; now < (int)strlen(catelog); now++) {
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
	for (int i = 0; i < (int)tmp.size(); i++)
		tmp[a[i]].print(loc1, loc2);
	delete[] a;
	return 1;
}

void try_transfer(char loc1[], char loc2[], char loc[],
char ans[], char date[], char catelog[], ticket res[], int &mn) {
	for(int now = 0; now < (int)strlen(catelog); now++) {
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

class OrderKey {
public:
	int Id;
	short nth1, nth2, dateNum;
	char Catalog, TrainId[21], TicketKind[21];
	bool operator<(const OrderKey &other)const {
		if (Id > other.Id) return 0;
		if (Id < other.Id) return 1;
		if (Catalog > other.Catalog) return 0;
		if (Catalog < other.Catalog) return 1;
		if (dateNum > other.dateNum) return 0;
		if (dateNum < other.dateNum) return 1;
		if (nth1 > other.nth1) return 0;
		if (nth1 < other.nth1) return 1;
		if (nth2 > other.nth2) return 0;
		if (nth2 < other.nth2) return 1;
		int cmp = strcmp(TrainId, other.TrainId);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(TicketKind, other.TicketKind);
		if (cmp < 0) return 1;
		return 0;
	}
	bool operator>(const OrderKey &other) const{
		if (Id > other.Id) return 1;
		if (Id < other.Id) return 0;
		if (Catalog > other.Catalog) return 1;
		if (Catalog < other.Catalog) return 0;
		if (dateNum > other.dateNum) return 1;
		if (dateNum < other.dateNum) return 0;
		if (nth1 > other.nth1) return 1;
		if (nth1 < other.nth1) return 0;
		if (nth2 > other.nth2) return 1;
		if (nth2 < other.nth2) return 0;
		int cmp = strcmp(TrainId, other.TrainId);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		cmp = strcmp(TicketKind, other.TicketKind);
		if (cmp > 0) return 1;
		return 0;
	}
	bool operator==(const OrderKey &other) const{
		return !(*this < other || *this > other);
	}
public:
	OrderKey(){}
	OrderKey(const int &i, const char &c, const int &d, const int &l1, const int &l2, char *tk, char *Tid) {
		Id = i;
		Catalog = c;
		nth1 = l1;
		nth2 = l2;
		dateNum = d;
		strcpy(TicketKind, tk);
		strcpy(TrainId, Tid);
		dateNum = -1;
	}
	OrderKey(const int &i, const char &c, const int &d) {
		Id = i;
		Catalog = c;
		dateNum = d;
		nth1 = -1;
		nth2 = -1;
		strcpy(TicketKind, "\0");
		strcpy(TrainId, "\0");
	}
};

class Order {
public:
	int Num;
	double Price;
	Order() {
		Num = 0;
		Price = 0;
	}
};

sjtu::Bplustree<OrderKey, Order> OrderBpt("OrderBpt");

void printDate(int x) {
	printf("2018-06-%c%c ", x / 10 + '0', x % 10 + '0');
}

bool buy_ticket() {
	OrderKey orderKey;
	Order order;
	char l1[41], l2[41], d[11];
	scanf("%d%d%s%s%s%s%s", &orderKey.Id, &order.Num, orderKey.TrainId, l1, l2, d, orderKey.TicketKind);
	orderKey.dateNum = (d[8] - '0') * 10 + d[9] - '0';

	auto itU = UserBpt.search(orderKey.Id);
	auto itT0 = TrainBpt.search(TrainKey(orderKey.TrainId, 0));
	if (!itT0.valid() || !itT0->pub) {
		puts("0");
		return 0;
	}
	auto itS1 = StationBpt.search(StationKey(l1, itT0->TrainCata, orderKey.TrainId));
	auto itS2 = StationBpt.search(StationKey(l2, itT0->TrainCata, orderKey.TrainId));
	if (!itU.valid() || !itS1.valid() || !itS2.valid()) {
		puts("0");
		return 0;
	}

	orderKey.nth1 = itS1->nth;
	orderKey.nth2 = itS2->nth;
	auto itT = TrainBpt.search(TrainKey(orderKey.TrainId, itS1->nth));
//	if (itT->start_time >= 1440) {
//		orderKey.dateNum--;
//	}
	orderKey.Catalog = itS1.key().catalog;
	auto itO = OrderBpt.search(orderKey);

	int i;
	for (i = 0; i < itT->PriceNum; i++)
		if (strcmp(orderKey.TicketKind, itT->Catalog[i]) == 0)
			break;
	if (i == itT->PriceNum) {
		puts("0");
		return 0;
	}
	auto tmp = itT;
	for (; tmp.key().nth != itS2->nth; tmp++){
		int date = orderKey.dateNum + tmp->start_time / 1440;
		if (tmp->leftTicket[i][date] < order.Num) {
			puts("0");
			return 0;
		}
	}
	int lr = itT.key().nth;
	for (; itT.key().nth != itS2->nth; itT++) {
		int date = orderKey.dateNum + itT->start_time / 1440;
		itT->leftTicket[i][date] -= order.Num;
		if(itT.key().nth != lr) order.Price += itT->Price[i];
		itT.save();
	}
	order.Price += itT->Price[i];
	if (itO.valid()) {
		itO->Num += order.Num;
		itO.save();
	}
	else OrderBpt.insert(orderKey, order);

//	if (OrderBpt.find(orderKey)) cout << "insert succeed";
	puts("1");
	return 1;
}

bool refund_ticket() {
	OrderKey orderKey;
	int num;
	char l1[41], l2[41], d[11];
	scanf("%d%d%s%s%s%s%s", &orderKey.Id, &num, orderKey.TrainId, l1, l2, d, orderKey.TicketKind);
	orderKey.dateNum = (d[8] - '0') * 10 + d[9] - '0';

	auto itU = UserBpt.search(orderKey.Id);
	auto itT0 = TrainBpt.search(TrainKey(orderKey.TrainId, 0));
	if (!itT0.valid()) {
		puts("0");
		return 0;
	}
	auto itS1 = StationBpt.search(StationKey(l1, itT0->TrainCata, orderKey.TrainId));
	auto itS2 = StationBpt.search(StationKey(l2, itT0->TrainCata, orderKey.TrainId));
	if (!itU.valid() || !itS1.valid() || !itS2.valid()) {
		puts("0");
		return 0;
	}
	orderKey.nth1 = itS1->nth;
	orderKey.nth2 = itS2->nth;
	auto itT = TrainBpt.search(TrainKey(orderKey.TrainId, itS1->nth));
	orderKey.Catalog = itS1.key().catalog;
	auto itO = OrderBpt.search(orderKey);
	if (!itO.valid()) {
		puts("0");
		return 0;
	}

	int i;
	for (i = 0; i < itT->PriceNum; i++)
		if (strcmp(orderKey.TicketKind, itT->Catalog[i]) == 0)
			break;


	for (; itT.key().nth != itS2->nth; itT++) {
		itT->leftTicket[i][orderKey.dateNum] += num;
		itT.save();
	}
	if (itO->Num < num) {
		puts("0");
		return 0;
	}
	if (itO->Num == num)
		OrderBpt.erase(orderKey);
	else {
		itO->Num -= num;
		itO.save();
	}

	puts("1");
	return 1;
}

bool query_order() {
	int id, d;
	char date[10], c[5];
	scanf("%d%s%s", &id, date, c);
	char cata = c[0];

	d = (date[8] - '0') * 10 + date[9] - '0';
	auto itO = OrderBpt.lower_bound(OrderKey(id, cata, d));
	if (!itO.valid()) {
		puts("-1");
		return 0;
	}

	vector<OrderKey> vec[5];
	vector<Order> vecO[5];
	vector<TrainNode> vecT1;
	vector<TrainNode> vecT2;
	int i = -1;
	int cnt = 0;
	if (itO.key().Id != id || cata != itO.key().Catalog || itO.key().dateNum != d) {
		puts("-1");
		return 0;
	}
	int n = -1;
	while (itO.key().Id == id && cata == itO.key().Catalog && itO.key().dateNum == d) {
		cnt++;
		if (n != itO.key().nth1) {
			vecT1.push_back(*(TrainBpt.search(TrainKey(itO.key().TrainId, itO.key().nth1))));
			vecT2.push_back(*(TrainBpt.search(TrainKey(itO.key().TrainId, itO.key().nth2))));
			i++;
		}
		n = itO.key().nth1;
		vec[i].push_back(itO.key());
		vecO[i].push_back(itO.data());
		itO++;
	}

	printf("%d\n", cnt);
	for (int j = 0; j <= i; j++) {
		printf("%s %s ", vec[j][0].TrainId, vecT1[j].StationName);
		printDate(vec[j][0].dateNum);
		printTime(vecT1[j].start_time);
		printf("%s ", vecT2[j].StationName);
		if (vecT2[j].arr_time >= 1440)
			printDate(vec[j][0].dateNum + 1);
		else
			printDate(vec[j][0].dateNum);
		printTime(vecT2[j].arr_time);
		for (int k = 0; k < (int)vec[j].size(); k++) {
			printf("%s %d %f%c", vec[j][k].TicketKind, vecO[j][k].Num, vecO[j][k].Price, " \n"[k == (int)vec[j].size() - 1]);
		}
	}
	return 1;
}

#endif
