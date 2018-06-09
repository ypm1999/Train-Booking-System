#ifndef _STATION
#define _STATION

#include <iostream>
#include "mystring.h"
#include <cmath>
#include <cstdio>
#include "Bplustree.h"
//#include "set.h"
using namespace std;

struct StationKey {
	mystring<20> StationName;
	mystring<20> TrainId;

	StationKey() {}
	StationKey(const mystring<20> &Stn, const mystring<20> &Tid) {
		StationName = Stn;
		TrainId = Tid;
	}

	bool operator<(const StationKey &other) {
		if (StationName < other.StationName) return 1;
		else if (other.StationName < StationName) return 0;
		if (TrainId < other.TrainId) return 1;
		else return 0;
	}
};

//Station
class StationNode {
private:
	StationKey stationKey;
	mystring<20> TrainName;
	int StationNum;
	int PriceNum;
	int nth;
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
	StationNode(const mystring<20> &Tid, const mystring<20> &Tn, const mystring<20> &Stn, const int &nst, const int &np, const int &ith, mystring<10> *cata, double *p, mystring<5> &atm, mystring<5> &stm) {
		stationKey = StationKey(Stn, Tid);
		TrainName = Tn;
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

	StationKey key() const{
		return stationKey;
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

	mystring<10> *catalog() {
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

sjtu::Bplustree<StationKey, StationNode> StationBpt("StationBpt");

void add_station(const StationNode &stNd) {
	StationBpt.insert(stNd.key(), stNd);
}

StationNode query_station(const StationKey &k) {
	return (*StationBpt.search(k));
}

void sale_station(const StationKey &k) {
	auto itSB = StationBpt.search(k);
	(*itSB).pub_TrainNode();
	itSB.save();
}

void del_station(const StationKey &k) {
	StationBpt.erase(k);
}

void modify_station(const StationKey &k, const StationNode &Stn) {
	StationBpt.insert(k, Stn);
}


#endif
