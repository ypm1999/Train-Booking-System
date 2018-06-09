#ifndef _TICKET
#define _TICKET

#include<iostream>
#include<cstdio>
#include"Bplustree.h"
#include"mystring.h"
#include"user.h"
#include"sort.h"
#include <vector>
#include"train.h"

using namespace std;


class Order {
	friend bool buy_ticket();
private:
	int Id, dateNum;
	char Catalog[2], TrainId[21], TicketKind[21], Loc1[41], Loc2[41], Date[9];
	int Num;
	bool operator<(const Order &other) {
		if (Id > other.Id) return 0;
		if (Id < other.Id) return 1;
		int cmp = strcmp(Catalog, other.Catalog);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(Loc1, other.Loc1);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(Loc2, other.Loc2);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(Date, other.Date);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(TicketKind, other.TicketKind);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		cmp = strcmp(TrainId, other.TrainId);
		if (cmp > 0) return 0;
		if (cmp < 0) return 1;
		if (Num < other.Num) return 1;
		return 0;
	}
	bool operator>(const Order &other) {
		if (Id > other.Id) return 1;
		if (Id < other.Id) return 0;
		int cmp = strcmp(Catalog, other.Catalog);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		cmp = strcmp(Loc1, other.Loc1);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		cmp = strcmp(Loc2, other.Loc2);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		cmp = strcmp(Date, other.Date);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		cmp = strcmp(TicketKind, other.TicketKind);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		cmp = strcmp(TrainId, other.TrainId);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		if (Num > other.Num) return 1;
		return 0;
	}
	bool operator==(const Order &other) {
		return !(*this < other && *this > other);
	}
public:
	Order() {}
};

sjtu::Bplustree<Order, Order> OrderBpt("OrderBpt");

struct ticket {
	char trainId[21], num, ticketKind[5][41];
	int leftTicket[5], price[5];
	int date1, date2, time1, time2;
	ticket() {}
	ticket(const char *id, int _num, char *date, int t1, int t2) {
		strcpy(trainId, id);
		num = _num;
		date1 = (date[8] - 0) * 10 + date[9] - '0';
		date2 = date1;
		time1 = t1;
		time2 = t2;
		while (time2 >= 1440) {
			time2 -= 1440;
			date2++;
		}
	}
	friend bool operator <(const ticket &a, const ticket &b) {
		return strcmp(a.trainId, b.trainId) < 0;
	}
	void todate(int data, char a[]) const {
		sprintf(a, "2018-6-%02d", data);
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
		for (int i = 1; i <= num; i++)
			printf("%s %d %lf%c", ticketKind[i], leftTicket[i], price[i], " \n"[i == num]);
	}
	int get_arrive_time() const {
		return date1 * 1440 + time1;
	}
	int get_start_time() const {
		return date2 * 1440 + time2;
	}
};


void getleftticket(const char id[], int l, int r, int left[]) {

}

void sort(int a[], int l, int r);
vector<ticket> tmp;
bool cmp(int a, int b) {
	return tmp[a] < tmp[b];
}

bool cmp_arrive_time(const ticket &a, const ticket &b) {
	return a.get_arrive_time() < b.get_arrive_time();
}

bool cmp_start_time(const ticket &a, const ticket &b) {
	return a.get_arrive_time() < b.get_arrive_time();
}

void getticket(char loc1[], char loc2[], char date[], vector<ticket> tmp, 
	sjtu::Bplustree<StationKey, StationNode>::Iterator it1, 
	sjtu::Bplustree<StationKey, StationNode>::Iterator it2){
	while (true) {
		mystring<20> id1(it1.key().TrainId), id2(it2.key().TrainId);
		if (id1 == id2 && it1->nth < it2.data().nth) {
			int n = it1->PriceNum;
			ticket t(id1.c_str(), n, date, it1->start_time, it2->arr_time);
			getleftticket(id1.c_str(), it1->nth, it2->nth, t.leftTicket);
			for (int i = 0; i < n; i++) {
				strcpy(t.ticketKind[i], it1->Catalog[i]);
				t.price[i] = it2->Price[i] - it1->Price[i];
			}
			tmp.push_back(t);
		}
		else {
			if (id1 < id2) {
				it1++;
				if (!it1.valid() || strcmp(it1.key().StationName, loc1) != 0)
					break;
			}
			else {
				it2++;
				if (!it2.valid() || strcmp(it2.key().StationName, loc2) != 0)
					break;
			}

		}
	}
}
bool query_ticket() {
	char loc1[41], loc2[41], date[12], catelog[11];
	scanf("%s%s%s%s", loc1, loc2, date, catelog);
	auto it1 = StationBpt.lower_bound(StationKey(loc1));
	auto it2 = StationBpt.lower_bound(StationKey(loc2));
	if (!(it1.valid() && it2.valid())) {
		puts("-1");
		return 0;
	}
	getticket(loc1, loc2, date, tmp, it1, it2);
	int cnt = tmp.size();
	int *a = new int[cnt + 2];
	for (int i = 1; i <= cnt; i++)
		a[i] = i;
	sort(a, 1, cnt);
	printf("%d\n", tmp.size());
	for (int i = 0; i < tmp.size(); i++)
		tmp[a[i]].print(loc1, loc2);
	delete[] a;
	tmp.clear();
}

void try_transfer(char loc1[], char loc2[], char loc[], 
char ans[], char date[], char catelog[], ticket now[], int &res) {
	auto it1 = StationBpt.lower_bound(StationKey(loc1));
	auto it2 = StationBpt.lower_bound(StationKey(loc2));
	auto it = StationBpt.lower_bound(StationKey(loc));
	if (!(it1.valid() && it2.valid() && it.valid()))
		return;
	vector<ticket> t1, t2;
	getticket(loc1, loc, date, t1, it1, it);
	getticket(loc, loc2, date, t2, it, it2);
	int n1 = t1.size(), n2 = t2.size();
	//sort(t1, 1, n1, cmp_arrive_time);
	//sort(t2, 1, n2, cmp_start_time);
	for (int i = 0; i < n1; i++) {
		int tim1 = t1[i].get_arrive_time();
		int st = t1[i].get_start_time();
		for (int j = 0; j < n2; j++) {
			int tim2 = t2[j].get_arrive_time();
			int ed = t2[j].get_arrive_time();
			if (tim1 <= tim2 && ed - st < res) {
				now[0] = t1[i];
				now[1] = t2[j];
				strcpy(ans, loc);
				res = ed - st;
			}
		}
	}
}


bool query_transfer() {
	char loc1[41], loc2[41], loc[41], date[12], catelog[11];
	scanf("%s%s%s%s", loc1, loc2, date, catelog);
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
		tmp[0].print(loc1, loc);
		tmp[1].print(loc, loc2);
		delete[] now;
		return 1;
	}
}

#endif