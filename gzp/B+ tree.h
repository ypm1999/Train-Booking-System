#ifndef _Bplus
#define _Bplus

#include <iostream>;
#include "String.h";
#include "exceptions.h"
#include <cmath>;
#include <cstdio>;
#include <fstream>;
using namespace std;

//Bplustree

template <class Key, class T>
class Bplustree {
	friend class iterator;
private:
	static const int miniDegree = 2;
	static const int maxDegree = 100;
	static const int miniKeyNum = 50;
	static const int maxKeyNum = 99;
	static const int maxDataNum = 99;
	static const int miniDataNum = 50;
	static String file_name;
	static const int idxNodeSize = sizeof(idxNode);
	static const int dataNodeSize = sizeof(dataNode);
	//node
	struct idxNode {
		size_t offset[maxDegree];
		bool isLeaf;
		int keyNum;
		bool isRead;
		void *child[maxDegree];
		Key key[maxKeyNum];

		idxNode() : keyNum(0), isRead(false) {}

	};

	struct dataNode {
		size_t nextoffset;
		int keyNum;
		Key key[maxDataNum];
		T data[maxDataNum];
		dataNode *next;
		dataNode():keyNum(0){}
	};

	void makeEmpty(idxNode *t);

	T search(const Key &_k, void * _Node);

	bool find(const Key &_k, void * _Node);

	idxNode *insert(const Key &_k, const T &_data, idxNode *t);

	dataNode *insertData(const Key &_k, const T &_data, dataNode *t);

	idxNode *addIdxBlk(idxNode *n, idxNode *t);

	idxNode *addDataBlk(dataNode *n, idxNode *t);

	idxNode *erase(const Key &_k, idxNode *t);

	dataNode *eraseData(const Key &_k, dataNode *t);

	idxNode *addData(dataNode *n, idxNode *t);

	idxNode *addIdx(idxNode *n, idxNode *t);

	idxNode *root = NULL;
	dataNode *leftHead = NULL;
	size_t idxNodeNum = 0;
	size_t dataNodeNum = 0;
	fstream List;
	fstream Data;

public:

	class iterator {
		friend class Bplustree;
	private:
		Bplustree * tree_ptr = NULL;
		dataNode *node_ptr = NULL;

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
			iterator *tmp = this;
			node_ptr = node_ptr->next;
			return tmp;
		}

		iterator &operator++() {
			node_ptr = node_ptr->next;
			return *this;
		}
	};
	//constructor
	Bplustree(const String &_file);

	//destructor
	~Bplustree();

	//find data
	T search(const Key &_k)const {
		List.seekg(0, ios::beg);
		List.read(reinterpret_cast<char *>, idxNodeSize);
		return search(_k, root);
	}

	bool find(const Key &_k) const {
		List.seekg(0, ios::beg);
		List.read(reinterpret_cast<char *>, idxNodeSize);
		return find(_k, root);
	}

	//insert node
	void insert(const Key &_k, const T &_data);

	//erase node
	void erase(const Key &_k);
};
#endif
