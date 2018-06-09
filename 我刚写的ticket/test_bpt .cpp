#define  _CRT_SECURE_NO_WARNINGS   
#include "Bplustree.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
using namespace sjtu;

long long aa = 13131, bb = 5353, MOD = (long long)(1e9 + 7), now = 1;
int rand()
{
	for (int i = 1; i<3; i++)
		now = (now * aa + bb) % MOD;
	return now;
}
class Key {
public:
	int x;

	Key(const Key &other) :x(other.x) {}
	Key() : x(0) {}
	Key(int x) :x(x) {}
	int num()const {
		return x;
	}
	bool operator<(const Key _k) const {
		return x < _k.num();
	}
	bool operator>(const Key _k) const {
		return x > _k.num();
	}
	bool operator==(const Key _k) const {
		return x == _k.num();
	}
};

class Data {
public:
	long long s;

	Data() :s(0) {}
	Data(int p) :s(p) {}
	Data(const Data &other) :s(other.s) {}
	Data & operator = (const Data &other) {
		if (this == &other) return *this;
		s = other.s;
		return *this;
	}
	long long val()const {
		return s;
	}
};

Bplustree<Key, Data> Bpt("Bpt");
const int num = 10000;
int a[num + 10];
map<Key, Data> mp;

void geta() {
	for (int i = 1; i <= num; i++)
		swap(a[i], a[rand() % num + 1]);
}
void test_insert() {
	puts("Test: insert");
	geta();
	for (int i = 1; i <= num; i++) {
		Key key(a[i]);
		Data data(rand());
		//cout << i << " "<< a[i] << " " << data.val() << endl;
		mp[key] = data;
		Bpt.insert(key, data);
	}

	puts("");

	for (int i = 1; i <= num; i++) {
		Key key(i);
		try {
			if (mp[key].val() != (Bpt.search(key)).data().val())
				cerr << "error on " << i << " " << mp[key].val() << " " << (Bpt.search(key)).data().val() << endl;
		}
		catch (...) { cout << "index" << endl; }
	}
	puts("No difference with map insert");

}

void test_erase() {
	for (int i = 1; i < num; i++) {
		if (rand() % 3 == 0) {
			continue;
		}
		Key key(a[i]);

		mp.erase(key);
		try {
			Bpt.erase(key);
		}
		catch (...) { cout << i << "erase error:" << a[i] << endl; }
	}
	
	puts("");
	for (int i = 1; i <= num; i++) {
		Key key(a[i]);
		if ((mp.find(key) != mp.end()) != Bpt.search(key).valid())
			cerr << "find error at " << i << endl;
		else
			if (mp.find(key) != mp.end() && Bpt.find(key))
				if (mp[key].val() != (Bpt.search(key)).data().val())
					cerr << "error on " << i << " " << mp[key].val() << " " << (Bpt.search(key)).data().val() << endl;
	}
	puts("No difference with map erase");
}

void test_iterator() {
	puts("Test: iterator");
	for (int i = 1; i <= num; i++) {
		Key key(i);
		try {
			auto itm = mp.find(key);
			auto itb = Bpt.search(key);

			if ((itm != mp.end()) != itb.valid())
				printf("wrong find at %d\n", i);
			else
				for (; itm != mp.end(); itm++, itb++) {
					if ((*itm).second.val() != itb.data().val())
						printf("error on %lld %lld\n", (*itm).second.val(), itb.data().val());
				}

		}
		catch (...) { cout << i  << " index" << endl; }
	}
	puts("iterator no difference with map");
}

void test_lower_bound() {
	puts("Test: lower_bound");
	for (int i = 1; i <= num; i++) {
		Key key(i);
		try {
			auto itm = mp.lower_bound(key);
			auto itb = Bpt.lower_bound(key);
			if ((itm != mp.end()) != itb.valid())
				printf("wrong lower_bound end at %d\n", i);
			else
				if (itb.valid() && (itm->second.val() != itb.data().val()))
					cerr << "error on " << i << " " << mp[key].val() << " " << (Bpt.search(key)).data().val() << endl;

		}
		catch (...) { cout << i << " index" << endl; }
	}
	puts("iterator no difference with map");

}



void test() {
	for (int i = 1; i <= num; i++) {
		Key key(rand() % num + 1);
		Data now(rand());
		if (mp.find(key) != mp.end()) {
			//printf("[%d, %d] is found in map\n", key.num(), mp[key].val());
			auto it = Bpt.search(key);
			//cout << it.data().val() << endl;
			mp[key] = now;
			*it = now;
			it.save();
		}
	}
	auto itp = Bpt.search(Key(1));
	auto itm = mp.begin();
	int cnt = 0;
	for (; itm != mp.end(); itp++, itm++)
		if ((*itp).val() != (*itm).second.val())
			printf("error on: %lld %lld\n", (*itp).val(), (*itm).second.val());
	puts("test finished");
}

int main()
{
	//freopen("test.out", "w", stdout);
	Bpt.clear();
	for (int i = 1; i <= num; i++)
		a[i] = i;
	test_insert();
	test_erase();
	test_lower_bound();
	test_iterator();
	system("pause");
	return 0;
}
