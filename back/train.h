#ifndef _TRAIN
#define _TRAIN

#include <iostream>
#include "mystring.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include "user.h"
#include "Bplustree.h"

struct TrainKey {
	char TrainId[21];
	short nth;

	TrainKey() {
		nth = 0;
	}
	TrainKey(char *Tid, const int &ith) {
		strcpy(TrainId, Tid);
		nth = ith;
	}

	bool operator<(const TrainKey &other) const {
		int cmp = strcmp(TrainId, other.TrainId);
		if (cmp < 0) return 1;
		if (cmp > 0) return 0;
		if (nth < other.nth) return 1;
		return 0;
	}
	bool operator==(const TrainKey &other) const {
		if (strcmp(TrainId, other.TrainId) != 0) return 0;
		if (nth != other.nth) return 0;
		return 1;
	}
	bool operator>(const TrainKey &other) const {
		return !(*this < other || *this > other);
	}
};

class TrainNode {
	friend void getleftticket(char *id, int l, int r, int n, int date, int left[], double pri[]);
	friend bool add_train();
	friend bool sale_train();
	friend bool query_train();
	friend bool delete_train();
	friend bool modify_train();
	friend bool query_order();
	//friend bool query_ticket();
	friend bool buy_ticket();
	friend bool refund_ticket();
	//friend bool query_transfer();
	friend class StationNode;
public:
	char TrainName[41];
	char StationName[31];
	char TrainCata;
	int StationNum;
	int PriceNum;
	short arr_time;
	short start_time;
	char Catalog[5][21];
	double Price[5];
	bool pub;
	short leftTicket[5][32];

public:
	TrainNode() {
		pub = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 32; j++)
				leftTicket[i][j] = 2000;
		}
	}
};


struct StationKey {
	char StationName[41];
	char catalog;
	char TrainId[21];

	StationKey() {
		StationName[0] = 0;
		catalog = 0;
		TrainId[0] = 0;
	}
	StationKey(char *Stn, const char &Tc) {
		strcpy(StationName, Stn);
		catalog = Tc;
		TrainId[0] = 0;
	}
	StationKey(char *Stn, const char &Tc, char *Tid) {
		strcpy(StationName, Stn);
		catalog = Tc;
		strcpy(TrainId, Tid);
	}

	bool operator<(const StationKey &other)const {
		int cmp = strcmp(StationName, other.StationName);
		if (cmp < 0) return 1;
		if (cmp > 0) return 0;
		if (catalog < other.catalog) return 1;
		if (catalog > other.catalog) return 0;
		if (strcmp(TrainId, other.TrainId) < 0) return 1;
		return 0;
	}

	bool operator>(const StationKey &other) const {
		int cmp = strcmp(StationName, other.StationName);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		if (catalog > other.catalog) return 1;
		if (catalog < other.catalog) return 0;
		if (strcmp(TrainId, other.TrainId) > 0) return 1;
		return 0;
	}
	bool operator==(const StationKey &other) const {
		return !(*this < other || *this > other);
	}
};


//Station
class StationNode {
	friend bool add_train();
	friend bool sale_train();
	friend bool query_train();
	friend bool delete_train();
	friend bool modify_train();
	friend bool query_order();
	//friend bool query_ticket();
	friend bool buy_ticket();
	friend bool refund_ticket();
	//friend bool query_transfer();
public:
	char TrainName[41];
	int StationNum;
	int PriceNum;
	short arr_time;
	short start_time;
	short nth;
	char Catalog[5][21];
	double Price[5];

public:
	StationNode() {}
	StationNode(const TrainNode &trNd, const int &ith) {
		strcpy(TrainName, trNd.TrainName);
		StationNum = trNd.StationNum;
		PriceNum = trNd.PriceNum;
		nth = ith;
		for (int i = 0; i < PriceNum; i++) {
			strcpy(Catalog[i], trNd.Catalog[i]);
			Price[i] = trNd.Price[i];
		}
		arr_time = trNd.arr_time;
		start_time = trNd.start_time;
	}

};

sjtu::Bplustree<TrainKey, TrainNode> TrainBpt("TrainBpt");
sjtu::Bplustree<StationKey, StationNode> StationBpt("StationBpt");

StationNode search_station(const StationKey &k) {
	return (*StationBpt.search(k));
}

int scanTime() {
	char ch[6];
	scanf("%s", ch);
	if (ch[0] == 'x') return -1;
	else return (ch[0] - '0') * 600 + (ch[1] - '0') * 60 + (ch[3] - '0') * 10 + (ch[4] - '0');
}

void printTime(int x) {
	x %= 1440;
	if (x == -1)
		printf("xx:xx ");
	else
		printf("%02d:%02d ", x / 60, x % 60);
}

double getdouble() {
	char ch = getchar();
	double res = 0;
	while (ch > '9' || ch < '0') ch = getchar();
	while (ch <= '9' && ch >= '0') res = res * 10 + ch - '0', ch = getchar();
	if (ch == '.') {
		double e = 0.1;
		ch = getchar();
		while (ch <= '9' && ch >= '0') res += e * (ch - '0'), e *= 0.1, ch = getchar();
	}
	return res;
}


bool add_train() {
	char ch[41];
	TrainNode trNd;
	TrainKey trainKey;
	char tmp[2];
	scanf("%s%s%s%d%d", trainKey.TrainId, trNd.TrainName, tmp, &trNd.StationNum, &trNd.PriceNum);
	trNd.TrainCata = tmp[0];
	for (int i = 0; i < trNd.PriceNum; i++)
		scanf("%s", trNd.Catalog[i]);
	if (!TrainBpt.empty()) {
		if (TrainBpt.find(TrainKey(trainKey))) {
			for (int i = 0; i < trNd.StationNum; i++) {
				scanf("%s%s%s%s", ch, ch, ch, ch);
				for (int i = 0; i < trNd.PriceNum; i++)
					getdouble();
			}
			puts("0");
			return 0;
		}
	}
	int last_start_time = -1;
	int day_time = 0;
	for (int i = 0; i < trNd.StationNum; i++) {
		scanf("%s", trNd.StationName);
		trNd.arr_time = scanTime() + day_time;
		if (trNd.arr_time < last_start_time) {
			trNd.arr_time += 1440;
			day_time = 1440;
		}
		trNd.start_time = scanTime() + day_time;
		if (trNd.start_time < trNd.arr_time) {
			trNd.start_time += 1440;
			day_time = 1440;
		}
		last_start_time = trNd.start_time;
		scanf("%s", ch);
		trainKey.nth = i;
		for (int j = 0; j < trNd.PriceNum; j++)
			trNd.Price[j] = getdouble();
		TrainBpt.insert(trainKey, trNd);
	}
	puts("1");
	return 1;
}

bool sale_train() {
	char Tid[21];
	scanf("%s", Tid);
	auto itTB = TrainBpt.search(TrainKey(Tid, 0));
	//if (itTB.valid()) cout << "exist";
	if (itTB->pub == 1) {
		puts("0");
		return 0;
	}
	itTB->pub = 1;
	itTB.save();
	for (int i = 0; i < itTB->StationNum; i++) {
		if (i) itTB++;
		StationKey stK(itTB->StationName, itTB->TrainCata, Tid);
		StationNode stNd((*itTB), i);
		StationBpt.insert(stK, stNd);
	}
	puts("1");
	return 1;
}

bool query_train() {
	char Tid[21];
	scanf("%s", Tid);
	auto itTB = TrainBpt.search(TrainKey(Tid, 0));
	//cout << "pub" << itTB->pub << endl;
	if (!itTB.valid() || !itTB->pub) {
		puts("0");
		return 0;
	}

	printf("%s %s %c %d %d ", Tid, itTB->TrainName, itTB->TrainCata, itTB->StationNum, itTB->PriceNum);
	for (int i = 0; i < itTB->PriceNum; i++)
		printf("%s%c", itTB->Catalog[i], " \n"[i == itTB->PriceNum - 1]);
	for (int i = 0; i < itTB->StationNum; i++) {
		if (i) itTB++;
		printf("%s ", itTB->StationName);
		printTime(itTB->arr_time);
		printTime(itTB->start_time);
		if (itTB->arr_time == -1 || itTB->start_time == -1) printf("xx:xx ");
		else printTime(itTB->start_time - itTB->arr_time);
		for (int j = 0; j < itTB->PriceNum; j++)
			printf("￥%f%c", itTB->Price[j], " \n"[j == itTB->PriceNum - 1]);
	}
	return 1;
}

bool delete_train() {
	char Tid[21];
	scanf("%s", Tid);
	auto itTB = TrainBpt.search(TrainKey(Tid, 0));
	if (!itTB.valid()) {
		puts("0");
		return 0;
	}
	if (itTB->pub == 1) {
		puts("0");
		return 0;
	}

	int nst = itTB->StationNum;
	for (int i = 0; i < nst; i++) {
		TrainBpt.erase(TrainKey(Tid, i));
		StationBpt.erase(StationKey(itTB->StationName, itTB->TrainCata, Tid));
	}

	puts("1");
	return 1;
}


bool modify_train() {
	char ch[41];
	TrainNode trNd;
	TrainKey trK;
	char tmp[2];
	scanf("%s%s%s%d%d", trK.TrainId, trNd.TrainName, tmp, &trNd.StationNum, &trNd.PriceNum);
	trNd.TrainCata = tmp[0];
	auto itTB = TrainBpt.search(trK);
	//if (itTB->pub) cout << "pub" << trK.TrainId;
	if (!itTB.valid() || itTB->pub) {
		for (int i = 0; i < trNd.PriceNum; i++)
			scanf("%s", ch);
		for (int i = 0; i < trNd.StationNum; i++) {
			scanf("%s%s%s%s", ch, ch, ch, ch);
			for (int j = 0; j < trNd.PriceNum; j++)
				getdouble();
		}
		puts("0");
		return 0;
	}
	bool f = 0;
	if (itTB->pub == 1) f = 1;
	for (int i = 0; i < trNd.PriceNum; i++)
		scanf("%s", trNd.Catalog[i]);
	for (int i = 0; i < trNd.StationNum; i++)
		TrainBpt.erase(TrainKey(trK.TrainId, i));
	int last_start_time = -1;
	int day_time = 0;
	for (int i = 0; i < trNd.StationNum; i++) {
		scanf("%s", trNd.StationName);
		trNd.arr_time = scanTime() + day_time;
		if (trNd.arr_time < last_start_time) {
			trNd.arr_time += 1440;
			day_time = 1440;
		}
		trNd.start_time = scanTime() + day_time;
		if (trNd.start_time < trNd.arr_time) {
			trNd.start_time += 1440;
			day_time = 1440;
		}
		scanf("%s", ch);
		trK.nth = i;
		for (int j = 0; j < trNd.PriceNum; j++)
			trNd.Price[j] = getdouble();
		TrainBpt.insert(trK, trNd);
		if (f) {
			StationKey stK(itTB->StationName, itTB->TrainCata, trK.TrainId);
			StationNode stNd((*itTB), i);
			StationBpt.insert(stK, stNd);
		}
	}
	puts("1");
	return 1;
}

#endif
