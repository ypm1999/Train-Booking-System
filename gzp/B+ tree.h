#ifndef _Bplus
#define _Bplus

#include <iostream>;
#include "String.h";
#include <cmath>;
#include <cstdio>;
#include <fstream>;
using namespace std;

//Bplustree

template <class Key, class T>
class Bplustree {
private:
	static const int miniDegree = 2;
	static const int maxDegree = 100;
	static const int miniKeyNum = 1;
	static const int maxKeyNum = 99;
	static String file_name;
	//node
	struct Node {
		size_t offset;
		size_t size;
		int keyNum;
		bool isLeaf;
		bool isRead;
		Node *child[maxDegree];
		Key key[maxKeyNum];
		T data[maxKeyNum];

		Node() : keyNum(0), isLeaf(false), isRead(false) {}

		void write() {
			List.seekp(offset, ios::beg);
			List.write(reinterpret_cast<const char *> this, sizeof(*this));
			if (isLeaf) {
				for (int i = 0; i < keyNum; i++) {
					data[i].write();
				}
			}
		}
	};

	void split(Node *_node) {

	}

	Node *root;
	Node *leftHead;
	fstream List;
	fstream Data;

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
	Bplustree(const String &_file) {}

	//destructor
	~Bplustree() {}

	//find data
	T find(Key _key) {

	}

	//insert node
	void insert(const Key &x, const T &_data) {}

	

	//erase node
	void erase(Key _key) {

	}
};
#endif
