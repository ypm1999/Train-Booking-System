#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include "Bplustree.h"
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
	Key(): x(0){}
	Key(int x) :x(x) {}
	int num()const {
		return x;
	}
	bool operator<(const Key _k) const{
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
	int* x;
	Data() :x(new int(1)) {}
	Data(int p) :x(new int(p)) {}
	Data(const Data &other) :x(new int(*(other.x))) {}
	~Data() {
		delete x;
	}
	Data & operator = (const Data &other) {
		if (this == &other) return *this;
		*x = *(other.x);
		return *this;
	}
	bool operator != (const Data &other){
		return x == other.x;
	}
	int num()const {
		return *x;
	}
};


Bplustree<Key, Data> Bpt("Bpt");
map<Key, Data> mp;
int num = 10;
void test_insert() {
	puts("Test: insert");
	std::cout << "empty: " << Bpt.empty() << std::endl;
	std::cout << "size " << Bpt.size() << endl;
	Key tmp[20];

	for (int i = 1; i <= num; i++) {
		Key key(i * 10 + rand() % 10);
		Data data(rand());
		Bpt.insert(key, data);
		mp[key] = data;
		tmp[i] = key;
		cout << Bpt.search(key).num() << " " << mp[key].num() << endl;
	}
	puts("");
	for(int i = 1; i <= num; i++)
		if(mp[tmp[i]] != Bpt.search(tmp[i]))
			puts("insert error");
}

void test_erase() {
	puts("Test: erase");
	for (int i = 1; i <= 1000 && !Bpt.empty(); i++) {
		int p = rand() % 1000;
		//cout << p << endl;
		Key key(p);
		try {
			Bpt.erase(key);
		}
		catch (...) { /*cout << "erase error" << endl;*/ }
	}
	puts("");
	for (int i = 1; i <= num; i++) {
		int tmp = rand() % 1000;
		Key key(tmp);
		//cout << i << ' ' << tmp << endl;
		try {
			std::cout << (Bpt.search(key)).num() << ' ' << endl;
		}
		catch (...) { /*cout << "index" << endl;*/ }
	}
}

void test_iterator() {

}

int main()
{
	test_insert();

	return 0;
}
