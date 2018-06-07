if#ifndef _TRAIN
#define _TRAIN

#include <iostream>
#include "mystring.h"
#include <cmath>
#include <cstdio>
#include "station.h"
#include "Bplustree.h"
using namespace std;

struct TrainKey {
	mystring<20> TrainId;
	int nth;

	TrainKey() {}
	TrainKey(const mystring<20> &Tid, const int &ith) {
		TrainId = Tid;
		nth = ith;
	}

	bool operator<(const TrainKey &other) {
		if (TrainId < other.TrainId) return 1;
		else if (other.TrainId < TrainId) return 0;
		if (nth < other.nth) return 1;
		else return 0;
	}
};

class TrainNode {
private:
	TrainKey trainKey;
	mystring<20> TrainName;
	mystring<20> StationName;
	int StationNum;
	int PriceNum;
	int arr_time;
	int start_time;
	mystring<10> Catalog[5];
	double Price[5];
	int leftTicket[5][30];
	bool pub;

	mystring<5> toString(int x) {
		mystring<5> a;
		
		if (x == -1) {
			a[0] = a[1] = a[3] = a[4] = 'x';
			a[2] = ':';
		}
		else {
			a[0] = x / 600 + '0';
			x -= (x / 600) * 600;
			a[1] = x / 60 + '0';
			x -= (x / 60) * 60;
			a[2] = ':';
			a[3] = x / 10 + '0';
			x -= (x / 10) * 10;
			a[4] = x;
		}
	
		return a;
	}
	

public:
	TrainNode(const mystring<20> &Tid, const mystring<20> &Tn, const mystring<20> &Stn, const int &nst, const int &np, const int &ith, mystring<10> *cata, double *p, mystring<5> &atm, mystring<5> &stm) {
		trainKey = TrainKey(Tid, ith);
		TrainName = Tn;
		StationName = Stn;
		StationNum = nst;
		PriceNum = np;;
		for (int i = 0; i < np; i++) {
			Catalog[i] = cata[i];
			Price[i] = p[i];
			memset(leftTicket[i], 2000, 30);
		}
		if (atm[0] == 'x') arr_time = -1;
		else arr_time = (atm[0] - '0') * 600 + (atm[1] - '0') * 60 + (atm[3] - '0') * 10 + (atm[4] - '0');
		if (stm[0] == 'x') start_time = -1;
		else start_time = (stm[0] - '0') * 600 + (stm[1] - '0') * 60 + (stm[3] - '0') * 10 + (stm[4] - '0');
		pub = 0;
	}

	TrainKey key() {
		return trainKey;
	}

	int stationNum() {
		return StationNum;
	}

	int priceNum() {
		return PriceNum;
	}

	pair<mystring<5>, mystring<5>> time() {
		return pair<mystring<5>, mystring<5>>(toString(arr_time), toString(start_time));
	}

	mystring<5> stop() {
		if (start_time == -1 || arr_time == -1) return toString(0);
		else return toString(start_time - arr_time);
	}

	mystring<20> trainName() {
		return TrainName;
	}

	mystring<20> stationName() {
		return StationName;
	}

	mystring<10> *catalog(){
		return Catalog;
	}

	double *price() {
		return Price;
	}

	bool ispub() {
		return pub;
	}

	void pub_TrainNode() {
		pub = true;
	}
};

void add_train(const mystring<20> &Tid, TrainNode *trNd, const int &nst) {
	sjtu::Bplustree<TrainKey, TrainNode> TrainBpt("TrainBpt");
	if (!TrainBpt.find(TrainKey(Tid, 0))) {
		printf("0\n");
		return;
	}

	for (int i = 0; i < nst; i++)
		TrainBpt.insert(trNd[i].key(), trNd[i]);
	printf("1\n");
	return;
}

void sale_train(const mystring<20> &Tid) {
	sjtu::Bplustree<TrainKey, TrainNode> TrainBpt("TrainBpt");
	auto itTB = TrainBpt.search(TrainKey(Tid, 0));
	if ((*itTB).ispub()) {
		printf("0\n");
		return;
	}
	int nst = (*itTB).stationNum();
	for (int i = 0; i < nst; i++, itTB++) {
		(*itTB).pub_TrainNode();
		itTB.save();
	}
	printf("1\n");
	return;
}

void query_train(const mystring<20> &Tid) {
	sjtu::Bplustree<TrainKey, TrainNode> TrainBpt("TrainBpt");
	auto itTB = TrainBpt.search(TrainKey(Tid, 0));
	int np = (*itTB).priceNum();
	int nst = (*itTB).stationNum();

	puts(Tid.c_str()); puts(" ");
	puts((*itTB).trainName().c_str()); puts(" ");
	printf("%d %d ", nst, np);
	for (int i = 0; i < np; i++) {
		puts((*itTB).catalog()[i].c_str());
		puts(" ");
	}
	printf("\n");
	for (int i = 0; i < nst; i++, itTB++) {
		puts((*itTB).stationName()..c_str()); puts(" ");
		puts((*itTB).time().first.c_str()); puts(" ");
		puts((*itTB).time().second.c_str()); puts(" ");
		puts((*itTB).stop().c_str()); puts(" ");
		for (int i = 0; i < np; i++) {
			printf("￥%f", (*itTB).price()[i]);
			puts(" ");
		}
		printf("\n");
	}
}

void delete_train(const mystring<20> &Tid) {
	sjtu::Bplustree<TrainKey, TrainNode> TrainBpt("TrainBpt");
	auto itTB = TrainBpt.search(TrainKey(Tid, 0));
	if ((*itTB).ispub()) {
		printf("0\n");
		return;
	}

	int nst = (*itTB).stationNum();
	for (int i = 0; i < nst; i++)
		TrainBpt.erase(TrainKey(Tid, i));
	printf("1\n");
}


void modify_train(const mystring<20> &Tid, TrainNode *trNd, const int &nst) {
	sjtu::Bplustree<TrainKey, TrainNode> TrainBpt("TrainBpt");
	if (!TrainBpt.find(TrainKey(Tid, 0))) {
		printf("0\n");
		return;
	}

	for (int i = 0; i < nst; i++)
		TrainBpt.insert(trNd[i].key(), trNd[i]);
	printf("1\n");
	return;
}

#endif
