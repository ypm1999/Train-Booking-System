#ifndef _Bplus
#define _Bplus

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
using namespace std;

//Bplustree

template <class Key>
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
		bool isRead;
		Node *child[maxDegree];
		Key key[maxKeyNum];
		static int file_name;
	};

	void split(Node *_node) {

	}

	Node *root;
	Node *leftHead;

public:

	class iterator {
		friend class Bplustree;
	private:
		Bplustree * tree_ptr = NULL;
		Node *node_ptr = NULL;

	public:
		iterator() = default;
		iterator(Bplustree *_t, Node *_n) {
			tree_ptr = _t;
			node_ptr = _n;
		}
		iterator(const iterator &other) {
			tree_ptr = other.tree_ptr;
			node_ptr = other.node_ptr;
		}

		iterator operator++(int) {

		}

		iterator &operator++() {

		}
	};
	//constructor
	Bplustree() {
		root = NULL;
		leftHead = NULL;
	}

	//destructor
	~Bplustree() {
		
	}

	//find data
	Data find(Key _key) {

	}

	//insert node
	void insert(Key _key, Data _data) {

	}

	//erase node
	void erase(Key _key) {

	}
};
#endif
