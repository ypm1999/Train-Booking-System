#ifndef _TRAIN
#define _TRAIN

#include <iostream>
#include "mystring.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include "Bplustree.h"
using namespace std;


struct TrainKey {
	friend bool buy_ticket();
	char TrainId[21];
	int nth;
public:
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
		return !(*this < other && *this > other);
	}
};

class TrainNode {
	friend bool buy_ticket();
	friend bool add_train();
	friend bool sale_train();
	friend bool query_train();
	friend bool delete_train();
	friend bool modify_train();
	friend class StationNode;
private:
	char TrainName[41];
	char StationName[41];
	char TrainCata[11];
	int StationNum;
	int PriceNum;
	int arr_time;
	int start_time;
	char Catalog[5][41];
	double Price[5];
	bool pub;

public:
	TrainNode() {
		pub = 0;
	}
};

//Station
class StationNode {
	friend bool query_ticket();
	friend bool query_transfer();
	friend bool buy_ticket();
	friend bool add_train();
	friend bool sale_train();
	friend bool query_train();
	friend bool delete_train();
	friend bool modify_train();
public:
	char TrainName[41];
	char TrainCata[11];
	int StationNum;
	int PriceNum;
	int arr_time;
	int start_time;
	int nth;
	char Catalog[5][41];
	double Price[5];
	int leftTicket[5][30];


	StationNode() {}
	StationNode(const TrainNode &trNd, const int &ith) {
		strcpy(TrainName, trNd.TrainName);
		StationNum = trNd.StationNum;
		PriceNum = trNd.PriceNum;
		nth = ith;
		strcpy(TrainCata, trNd.TrainCata);
		for (int i = 0; i < PriceNum; i++) {
			strcpy(Catalog[i], trNd.Catalog[i]);
			Price[i] = trNd.Price[i];
			for (int j = 0; j < 30; j++)
				leftTicket[i][j] = 2000;
		}
		arr_time = trNd.arr_time;
		start_time = trNd.start_time;
	}

};


struct StationKey {
	friend bool query_ticket();
	friend bool query_transfer();
	friend bool buy_ticket();
	char StationName[41];
	char TrainId[21];
public:
	StationKey() {
		StationName[0] = 0;
		TrainId[0] = 0;
	}
	StationKey(char *Stn) {
		strcpy(StationName, Stn);
		TrainId[0] = 0;
	}
	StationKey(char *Stn, char *Tid) {
		strcpy(StationName, Stn);
		strcpy(TrainId, Tid);
	}

	bool operator<(const StationKey &other)const {
		int cmp = strcmp(StationName, other.StationName);
		if (cmp < 0) return 1;
		if (cmp > 0) return 0;
		if (strcmp(TrainId, other.TrainId) < 0) return 1;
		return 0;
	}

	bool operator>(const StationKey &other) const {
		int cmp = strcmp(StationName, other.StationName);
		if (cmp > 0) return 1;
		if (cmp < 0) return 0;
		if (strcmp(TrainId, other.TrainId) > 0) return 1;
		return 0;
	}
	bool operator==(const StationKey &other) const {
		return !(*this < other && *this > other);
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
	scanf("%s%s%s%d%d", trainKey.TrainId, trNd.TrainName, trNd.TrainCata, &trNd.StationNum, &trNd.PriceNum);
	for (int i = 0; i < trNd.PriceNum; i++)
		scanf("%s", trNd.Catalog[i]);
	if (!TrainBpt.empty()) {
		if (TrainBpt.find(TrainKey(trainKey))) {
			double d;
			scanf("%s%s%s%s", ch, ch, ch, ch);
			for (int i = 0; i < trNd.PriceNum; i++)
				d = getdouble();
			puts("0");
			return 0;
		}
	}
	for (int i = 0; i < trNd.StationNum; i++) {
		scanf("%s", trNd.StationName);
		trNd.arr_time = scanTime();
		trNd.start_time = scanTime();
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
		StationKey stK(itTB->StationName, Tid);
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
	if (!itTB.valid()) {
		puts("0");
		return 0;
	}

	printf("%s %s %s %d %d ", Tid, itTB->TrainName, itTB->TrainCata, itTB->StationNum, itTB->PriceNum);
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
			printf("§Ô§Õ%f%c", itTB->Price[j], " \n"[j == itTB->PriceNum - 1]);
	}
	puts("1");
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
		StationBpt.erase(StationKey(itTB->StationName, Tid));
	}

	puts("1");
	return 1;
}


bool modify_train() {
	char ch[41];
	TrainNode trNd;
	TrainKey trK;
	scanf("%s%s%s%d%d", trK.TrainId, trNd.TrainName, trNd.TrainCata, &trNd.StationNum, &trNd.PriceNum);
	auto itTB = TrainBpt.search(trK);
	//if (itTB->pub) cout << "pub" << trK.TrainId;
	if (!itTB.valid() || itTB->pub) {
		for (int i = 0; i < trNd.PriceNum; i++)
			scanf("%s", ch);
		for (int i = 0; i < trNd.StationNum; i++) {
			double d;
			scanf("%s%s%s%s", ch, ch, ch, ch);
			for (int j = 0; j < trNd.PriceNum; j++)
				d = getdouble();
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
	for (int i = 0; i < trNd.StationNum; i++) {
		scanf("%s", trNd.StationName);
		trNd.arr_time = scanTime();
		trNd.start_time = scanTime();
		scanf("%s", ch);
		trK.nth = i;
		for (int j = 0; j < trNd.PriceNum; j++)
			trNd.Price[j] = getdouble();
		TrainBpt.insert(trK, trNd);
		if (f) {
			StationKey stK(itTB->StationName, trK.TrainId);
			StationNode stNd((*itTB), i);
			StationBpt.insert(stK, stNd);
		}
	}
	puts("1");
	return 1;
}

bool clean() {
	TrainBpt.clear();
	StationBpt.clear();
	puts("1");
	return 1;
}

bool exit() {
	puts("BYE");
	return 1;
}




#endif
