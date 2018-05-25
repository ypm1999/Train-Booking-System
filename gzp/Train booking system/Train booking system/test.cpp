#include "Bplustree.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
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
	int num()const {
		return *x;
	}
};

Bplustree<Key, Data> Bpt("Bpt");
int num = 1000;
void test_insert() {
	puts("Test: insert");
	std::cout << "empty:" << Bpt.empty() << std::endl;
	std::cout << "size" << Bpt.size() << endl;
	int num = 1000;
	for (int i = 1; i <= num; i++) {
		Key key(rand() % 1000);
		Data data(rand());
		//cout << key.num() << ' ' << data.num() << ' '  << Bpt.size() << endl;
		try {
			Bpt.insert(key, data);
		}
		catch (...) { /*cout << "insert error" << endl;*/ }
		
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
	puts("");
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
	test_erase();
	system("pause");
	return 0;
}
