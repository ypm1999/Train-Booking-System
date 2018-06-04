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
		delete x;
		x = new int(*(other.x));
		return *this;
	} 
	int num()const {
		return *x;
	}
};

Bplustree<Key, Data> Bpt("Bpt");
int num = 100;
int dNN = 1000;
void test_insert() {
	puts("Test: insert");
	std::cout << "empty:" << Bpt.empty() << std::endl;
	std::cout << "size:" << Bpt.size() << endl;
	int num = 100;
	for (int i = 1; i <= num; i++) {
		Key key(rand() % 100);
		Data data(rand());
		cout << "insert:" << key.num() << ' ' << data.num() << endl;
		try {
			Bpt.insert(key, data);
		}
		catch (...) { cout << "insert error" << endl; }
		Bpt.Print();
	}
	puts("");
	
	for (int i = 1; i <= num; i++) {
		int tmp = rand() % 100;
		Key key(tmp);
		//cout << i << ' ' << tmp << endl;
		try {
			std::cout << (Bpt.search(key)).num() << ' ' << endl;
		}
		catch (...) { cout << "index" << endl; }
	}
	puts("");
	
}

void test_erase() {
	dNN -= 1000;
	puts("Test: erase");
	for (int i = 1; i <= 50 && !Bpt.empty(); i++) {
		int p = rand() % 100;
		cout << p << endl;
		Key key(p);
		try {
			Bpt.erase(key);
		}
		catch (...) { /*cout << "erase error" << endl;*/dNN++; }
	}
	puts("");
	for (int i = 1; i <= num; i++) {
		int tmp = rand() % 100;
		Key key(tmp);
		cout << i << ' ' << tmp << endl;
		try {
			std::cout << (Bpt.search(key)).num() << endl;
		}
		catch (...) { /*cout << "index" << endl;*/ }
	}
}

void test_Iterator() {
	puts("Test: Iterator");
	Bplustree<Key, Data>::Iterator it(Bpt.begin());
	int _dNN = 0;
	while (1) {
		try {
			cout << it.itkey().num() << endl;
			it++;
		}
		catch (...) { break; }
	}
	if (_dNN == dNN) puts("passed");
	puts("");
}

int main()
{
	Bpt.clear();
	test_insert();
	test_erase();
	test_Iterator();
	system("pause");
	return 0;
}
