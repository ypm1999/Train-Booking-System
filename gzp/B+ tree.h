#ifndef _Bplus
#define _Bplus

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
using namespace std;

//Bplustree

template <class T>
class Bplustree {
private:
	static const int miniDegree = 2;
	static const int maxDegree = 100;
	static const int miniKeyNum = 1;
	static const int maxKeyNum = 99;

	//data
	struct Data {
		size_t offset;
		size_t dataSize;
	};

	//node
	struct Node {
		Data data[maxKeyNum];
		int keyNum;
		bool isLeaf;
		Node *child[maxDegree];
		T key[maxKeyNum];
		static int file_name;
	};

	void split(Node *_node) {

	}

	Node *root;
	Node *leftHead;

public:
	//constructor
	Bplustree() {
		root = NULL;
		leftHead = NULL;
	}

	//destructor
	~Bplustree() {
		
	}

	//find data
	Data find(T _key) {

	}

	//insert node
	void insert(T _key, Data _data) {

	}

	//erase node
	void erase(T _key) {

	}
};
#endif
