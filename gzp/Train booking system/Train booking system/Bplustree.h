#ifndef _Bplus
#define _Bplus

#include <iostream>
#include <cstring>
//#include "String.h"
#include "exceptions.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include <cstring>
using namespace std;
//Bplustree

namespace sjtu {
	
	template <class Key, class T>
	class Bplustree {
		friend class Iterator;
	private:
		static const int miniDegree = 2;
		static const int maxDegree = 5;
		static const int miniKeyNum = 2;
		static const int maxKeyNum = 4;
		static const int maxn = 10;

		string path;

		//node
		struct idxNode {
			size_t offset[maxn];
			bool isLeaf;
			int keyNum;
			bool isRead;
			Key key[maxn];

			idxNode() {
				isLeaf = false;
				keyNum = -1;
				isRead = false;
			}

		};

		struct dataNode {
			size_t nextoffset;
			int keyNum;
			Key key[maxn];
			T data[maxn];
			dataNode() {
				keyNum = 0;
			}
		};

		const int idxNodeSize = sizeof(idxNode);
		const int dataNodeSize = sizeof(dataNode);

		T idxSearch(const Key &_k, idxNode *t) {
			int i;
			for (i = 0; i < t->keyNum; i++) {
				if (t->key[i] > _k)	break;
			}
			File.seekg(t->offset[i], ios::beg);
			if (t->isLeaf) {
				if (t != root) delete t;
				dataNode *p = new dataNode;
				File.read(reinterpret_cast<char *>(p), dataNodeSize);
				return dataSearch(_k, p);
			}
			else {
				if (t != root) delete t;
				idxNode *p = new idxNode;
				File.read(reinterpret_cast<char *>(p), idxNodeSize);
				return idxSearch(_k, p);
			}
		}

		T dataSearch(const Key &_k, dataNode *t) {
			bool found = 0;
			for (int i = 0; i < t->keyNum; i++) {
				if (t->key[i] == _k) {
					found = 1;
					T tmp = t->data[i];
					delete t;
					return tmp;
				}
			}
			if (!found) {
				delete t;
				throw index_out_of_bound();
			}
		}

		bool idxFind(const Key &_k, idxNode *t) {
			int i;
			for (i = 0; i < t->keyNum; i++) {
				if (t->key[i] > _k)	break;
			}
			File.seekg(t->offset[i], ios::beg);
			if (t->isLeaf) {
				if (t != root) delete t;
				dataNode *p = new dataNode;
				File.read(reinterpret_cast<char *>(p), dataNodeSize);
				return dataFind(_k, p);
			}
			else {
				if (t != root) delete t;
				idxNode *p = new idxNode;
				File.read(reinterpret_cast<char *>(p), idxNodeSize);
				return idxFind(_k, p);
			}
		}

		bool dataFind(const Key &_k, dataNode *t) {
			bool found = 0;
			for (int i = 0; i < t->keyNum; i++) {
				if (t->key[i] == _k) {
					found = 1;
					delete t;
					return true;
				}
			}
			if (!found) {
				delete t;
				return false;
			}
		}

		idxNode *insert(const Key &_k, const T &_data, idxNode *t) {
			void *newNode;

			int i = 0;
			for (i = 0; i < t->keyNum; i++)
				if (_k < t->key[i]) break;

			if (t->isLeaf) {
				File.seekg(t->offset[i], ios::beg);
				dataNode *p = new dataNode;
				File.read(reinterpret_cast<char *>(p), dataNodeSize);
				newNode = insertData(_k, _data, p);
				File.seekp(t->offset[i], ios::beg);
				File.write(reinterpret_cast<char *>(p), dataNodeSize);
				//delete p;
			}
			else {
				File.seekg(t->offset[i], ios::beg);
				idxNode *p = new idxNode;
				File.read(reinterpret_cast<char *>(p), idxNodeSize);
				newNode = insert(_k, _data, p);
				File.seekp(t->offset[i], ios::beg);
				File.write(reinterpret_cast<char *>(p), idxNodeSize);
				delete p;
			}

			if (newNode == NULL) return NULL;
			else {
				if (!t->isLeaf)	return addIdxBlk((idxNode *)newNode, t);
				else return addDataBlk((dataNode *)newNode, t);
			}
		}

		dataNode *insertData(const Key &_k, const T &_data, dataNode *t) {
			/*for (int i = 0; i <= t->keyNum; i++)
			t->data[i].read();*/
			int i = 0;
			for (i = 0; i < t->keyNum; i++) {
				if (t->key[i] == _k) {
					t->data[i] = _data;
					throw insert_error();
				}
			}
			for (i = t->keyNum; i > 0 && _k < t->key[i - 1]; i--) {
				t->key[i] = t->key[i - 1];
				t->data[i] = t->data[i - 1];
			}
			t->key[i] = _k;
			t->data[i] = _data;	
			t->keyNum++;
			Size++;

			if (t->keyNum <= maxKeyNum) return NULL;
			else {
				//split
				size_t newKeyNum = maxKeyNum / 2;
				dataNode *newNode = new dataNode;
				newNode->keyNum = t->keyNum - newKeyNum;
				for (int i = 0; i < newNode->keyNum; i++) {
					newNode->key[i] = t->key[i + newKeyNum];
					newNode->data[i] = t->data[i + newKeyNum];
				}
				t->keyNum = newKeyNum; 
				newNode->nextoffset = t->nextoffset;
				t->nextoffset = _offset;
				return newNode;
			}
		}

		idxNode *addIdxBlk(idxNode *newNode, idxNode *t) {
			idxNode *p = new idxNode;
			*p = *newNode;

			while (p->isLeaf == 0) {
				File.seekg(p->offset[0], ios::beg);
				File.open(reinterpret_cast<char *>(p), idxNodeSize);
			}
			dataNode *d = new dataNode;
			File.seekg(p->offset[0], ios::beg);
			File.open(reinterpret_cast<char *>(d), dataNodeSize);

			Key min = d->key[0];
			delete d;
			delete p;

			int i;
			for (i = t->keyNum; i > 0 && min < t->key[i - 1]; i--) {
				t->key[i] = t->key[i - 1];
				t->offset[i + 1] = t->offset[i];
			}
			t->key[i] = min;
			t->offset[i + 1] = _offset;
			t->keyNum++;
			File.seekp(_offset, ios::beg);
			File.write(reinterpret_cast<char *>(newNode), idxNodeSize);
			_offset += idxNodeSize;
			if (t->keyNum <= maxKeyNum) {
				delete newNode;
				return NULL;
			}
			else {
				//split
				int newKeyNum = Bplustree<Key, T>::maxKeyNum / 2;
				idxNode *newIdx = new idxNode;
				newIdx->isLeaf = 0;
				newIdx->keyNum = t->keyNum - 1 - newKeyNum;
				for (int i = 0; i < newIdx->keyNum; i++) {
					newIdx->key[i] = t->key[i + newKeyNum + 1];
					newIdx->offset[i] = t->offset[i + newKeyNum + 1];
				}
				newIdx->offset[newIdx->keyNum] = t->offset[t->keyNum];
				t->keyNum = newKeyNum;
				
				delete newNode;
				return newIdx;
			}
		}

		idxNode *addDataBlk(dataNode *newNode, idxNode *t) {
			int i;
			for (i = t->keyNum; i > 0 && newNode->key[0] < t->key[i - 1]; i--) {
				t->key[i] = t->key[i - 1];
				t->offset[i + 1] = t->offset[i];
			}
			t->key[i] = newNode->key[0];
			t->offset[i + 1] = _offset;
			t->keyNum++;
			
			File.seekp(_offset, ios::beg);
			File.write(reinterpret_cast<char *>(newNode), dataNodeSize);
			_offset += dataNodeSize;
			if (t->keyNum <= Bplustree<Key, T>::maxKeyNum) {
				delete newNode;
				return NULL;
			}
			else {
				//split
				size_t newKeyNum = Bplustree<Key, T>::maxKeyNum / 2;
				idxNode *newIdx = new idxNode;
				newIdx->keyNum = t->keyNum - newKeyNum - 1;
				newIdx->isLeaf = 1;
				for (int i = 0; i < newIdx->keyNum; i++) {
					newIdx->key[i] = t->key[i + newKeyNum + 1];
					newIdx->offset[i] = t->offset[i + newKeyNum + 1];
				}
				newIdx->offset[newIdx->keyNum] = t->offset[t->keyNum];
				t->keyNum = newKeyNum;
				delete newNode;
				return newIdx;
			}
		}

		idxNode *erase(const Key &_k, idxNode *t) {
			void *newNode;

			int i = 0;
			for (i = 0; i < t->keyNum; i++)
				if (_k < t->key[i]) break;

			if (t->isLeaf == 0) {
				File.seekg(t->offset[i], ios::beg);
				idxNode *p = new idxNode;
				File.read(reinterpret_cast<char *>(p), idxNodeSize);
				newNode = erase(_k, (idxNode *)p);
				File.seekp(t->offset[i], ios::beg);
				File.write(reinterpret_cast<char *>(p), idxNodeSize);
			}
			else {
				File.seekg(t->offset[i], ios::beg);
				dataNode *p = new dataNode;
				File.read(reinterpret_cast<char *>(p), dataNodeSize);
				newNode = eraseData(_k, (dataNode *)p);
				File.seekp(t->offset[i], ios::beg);
				File.write(reinterpret_cast<char *>(p), dataNodeSize);
				//delete p;
			}

			if (newNode == NULL) return NULL;
			else {
				if (t->isLeaf == 0)	return addIdx((Bplustree<Key, T>::idxNode *)newNode, t);
				else return addData((Bplustree<Key, T>::dataNode *)newNode, t);
			}
		}

		dataNode *eraseData(const Key &_k, dataNode *t) {
			int i;
			bool found = 0;
			for (i = 0; i < t->keyNum; i++) {
				if (t->key[i] == _k) {
					found = 1;
					break;
				}
			}
			if (!found) throw index_out_of_bound();

			for (; i < t->keyNum; i++) {
				t->key[i] = t->key[i + 1];
				t->data[i] = t->data[i + 1];
			}
			t->keyNum--;
			Size--;

			if (t->keyNum >= miniKeyNum) {
				delete t;
				return NULL;
			}
			else return t;
		}

		idxNode *addData(dataNode *n, idxNode *t) {
			int min;
			if (t == root) min = 2;
			else min = miniKeyNum;

			dataNode *now = new dataNode;

			int i;
			for (i = 0; i <= t->keyNum; i++) {
				File.seekg(t->offset[i], ios::beg);
				File.read(reinterpret_cast<char *>(now), dataNodeSize);
				if (now == n) break;
			}

			if (i != t->keyNum) {
				dataNode *next = new dataNode;
				File.seekg(t->offset[i + 1], ios::beg);
				File.read(reinterpret_cast<char *>(next), dataNodeSize);
				if (next->keyNum > miniKeyNum) {
					//borrow
					n->key[n->keyNum] = next->key[0];
					n->data[n->keyNum] = next->data[0];
					t->key[i] = next->key[1];
					n->keyNum++;
					next->keyNum--;
					for (int j = 0; j < next->keyNum; j++) {
						next->key[j] = next->key[j + 1];
						next->data[j] = next->data[j + 1];
					}
				}
				else {
					//merge
					n->keyNum += next->keyNum;
					for (int j = 0; j < next->keyNum; j++) {
						n->key[n->keyNum + j] = next->key[j];
						n->data[n->keyNum + j] = next->data[j];
					}
					n->nextoffset = next->nextoffset;
					delete next;
					t->keyNum--;
					for (int j = i; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->offset[j + 1] = t->offset[j + 2];
					}
				}
				File.seekp(t->offset[i], ios::beg);
				File.write(reinterpret_cast<char *>(n), dataNodeSize);
				if (next != NULL) {
					File.seekp(t->offset[i + 1], ios::beg);
					File.write(reinterpret_cast<char *>(next), dataNodeSize);
				}	
			}
			else if (i != 0) {
				dataNode *pre = new dataNode;
				File.seekg(t->offset[i - 1], ios::beg);
				File.read(reinterpret_cast<char *>(pre), dataNodeSize);
				if (pre->keyNum > miniKeyNum) {
					//borrow
					for (int j = n->keyNum; j > 0; j--) {
						n->key[j] = n->key[j - 1];
						n->data[j] = n->data[j - 1];
					}
					n->keyNum++;
					pre->keyNum--;
					n->key[0] = pre->key[pre->keyNum];
					n->data[0] = pre->data[pre->keyNum];
					t->key[i - 1] = n->key[0];
				}
				else {
					//merge
					pre->keyNum += n->keyNum;
					for (int j = 0; j < n->keyNum; j++) {
						pre->key[pre->keyNum + j] = n->key[j];
						pre->data[pre->keyNum + j] = n->data[j];
					}
					pre->nextoffset = n->nextoffset;
					delete n;
					t->keyNum--;
					for (int j = i - 1; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->offset[j + 1] = t->offset[j + 2];
					}
				}
				File.seekp(t->offset[i - 1], ios::beg);
				File.write(reinterpret_cast<char *>(pre), dataNodeSize);
				if (n != NULL) {
					File.seekp(t->offset[i], ios::beg);
					File.write(reinterpret_cast<char *>(n), dataNodeSize);
				}	
			}

			if (t->keyNum >= min) {
				delete t;
				return NULL;
			}
			else return t;
		}

		idxNode *addIdx(idxNode *n, idxNode *t) {
			int min;
			if (t == root) min = 2;
			else min = miniKeyNum;

			idxNode *now = new idxNode;

			int i;
			for (i = 0; i <= t->keyNum; i++) {
				File.seekg(t->offset[i], ios::beg);
				File.read(reinterpret_cast<char *>(now), idxNodeSize);
				if (now == n) break;
			}

			if (i != t->keyNum) {
				idxNode *next = new idxNode;
				File.seekg(t->offset[i], ios::beg);
				File.read(reinterpret_cast<char *>(next), idxNodeSize);
				idxNode *d = new idxNode;
				*d = *next;
				while (d->isLeaf == 0) {
					File.seekg(d->offset[0], ios::beg);
					File.read(reinterpret_cast<char *>(d), idxNodeSize);
				}
				dataNode *e = new dataNode;
				File.seekg(d->offset[0], ios::beg);
				File.read(reinterpret_cast<char *>(e), dataNodeSize);
				if (next->keyNum > miniKeyNum) {
					//borrow
					n->key[n->keyNum] = e->key[0];
					n->offset[n->keyNum + 1] = next->offset[0];
					n->keyNum++;
					next->keyNum--;
					int j;
					for (j = 0; j < next->keyNum; j++) {
						next->key[j] = next->key[j + 1];
						next->offset[j] = next->offset[j + 1];
					}
					next->offset[j] = next->offset[j + 1];
				}
				else {
					//merge
					n->key[n->keyNum] = e->key[0];
					int j;
					for (j = 0; j <= next->keyNum; j++) {
						n->key[n->keyNum + j + 1] = next->key[j];
						n->offset[n->keyNum + j + 1] = next->offset[j];
					}
					n->offset[n->keyNum + j + 1] = next->offset[j];
					n->keyNum += next->keyNum + 1;
					delete next;
					t->keyNum--;
					for (int j = i; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->offset[j + 1] = t->offset[j + 2];
					}
				}
				File.seekp(t->offset[i], ios::beg);
				File.write(reinterpret_cast<char *>(n), dataNodeSize);
				if (n != NULL) {
					File.seekp(t->offset[i + 1], ios::beg);
					File.write(reinterpret_cast<char *>(next), dataNodeSize);
				}
			}
			else if (i != 0) {
				idxNode *pre = new idxNode;
				File.seekg(t->offset[i - 1], ios::beg);
				File.read(reinterpret_cast<char *>(pre), idxNodeSize);
				idxNode *d = new idxNode;
				*d = *n;
				while (d->isLeaf == 0) {
					File.seekg(d->offset[0], ios::beg);
					File.read(reinterpret_cast<char *>(d), idxNodeSize);
				}
				dataNode *e = new dataNode;
				File.seekg(d->offset[0], ios::beg);
				File.read(reinterpret_cast<char *>(e), idxNodeSize);
				if (pre->keyNum > miniKeyNum) {
					//borrow
					n->offset[n->keyNum + 1] = n->offset[n->keyNum];
					int j;
					for (j = n->keyNum; j > 0; j--) {
						n->key[j] = n->key[j - 1];
						n->offset[j] = n->offset[j - 1];
					}
					n->key[0] = e->key[0];
					n->offset[0] = pre->offset[pre->keyNum];
					n->keyNum++;
					pre->keyNum--;
				}
				else {
					//merge
					pre->key[pre->keyNum] = e->key[0];
					int j;
					for (j = 0; j <= n->keyNum; j++) {
						pre->key[pre->keyNum + j + 1] = n->key[j];
						pre->offset[pre->keyNum + j + 1] = n->offset[j];
					}
					pre->offset[pre->keyNum + j + 1] = n->offset[j];
					pre->keyNum += n->keyNum + 1;
					delete n;
					t->keyNum--;
					for (int j = i - 1; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];

						t->offset[j + 1] = t->offset[j + 2];
					}
				}
				File.seekp(t->offset[i - 1], ios::beg);
				File.write(reinterpret_cast<char *>(pre), dataNodeSize);
				if (n != NULL) {
					File.seekp(t->offset[i], ios::beg);
					File.write(reinterpret_cast<char *>(n), dataNodeSize);
				}
			}

			if (t->keyNum >= min) {
				delete t;
				return NULL;
			}
			else return t;
		}

		idxNode *root = NULL;
		dataNode *leftHead = NULL;
		size_t _offset = 0;
		size_t Size = 0;
		fstream File;

	public:

		class Iterator {
			friend class Bplustree;
		private:
			Bplustree * tree_ptr = NULL;
			dataNode *node_ptr = NULL;

		public:
			Iterator() = default;
			Iterator(Bplustree *_t, dataNode *_n) {
				tree_ptr = _t;
				node_ptr = _n;
			}
			Iterator(const Iterator &other) {
				tree_ptr = other.tree_ptr;
				node_ptr = other.node_ptr;
			}

			Iterator operator++(int) {
				Iterator *tmp = this;
				tree_ptr->File.seekg(node_ptr->nextoffset, ios::beg);
				tree_ptr->File.read(reinterpret_cast<char *>(node_ptr), tree_ptr->dataNodeSize);
				if (node_ptr == NULL) throw index_out_of_bound();
				return *tmp;
			}

			Iterator &operator++() {
				tree_ptr->File.seekg(node_ptr->nextoffset, ios::beg);
				tree_ptr->File.read(reinterpret_cast<char *>(node_ptr), tree_ptr->dataNodeSize);
				if (node_ptr == NULL) throw index_out_of_bound();
				return *this;
			}

			Key itkey() {
				return node_ptr->key[0];
			}

			size_t itkeyNum() {
				return node_ptr->keyNum;
			}
		};

		Bplustree(){
			root = NULL;
			leftHead = NULL;
		}
		//constructor
		Bplustree(const string &_file) {
			path = _file;
			File.open(path.c_str(), ios::binary | ios::ate | ios::out | ios::in);
			leftHead = new dataNode;
			root = new idxNode;
			if (File.is_open()) {
				File.seekg(0, ios::beg);
				File.read(reinterpret_cast<char *>(&_offset), sizeof(size_t));
				File.read(reinterpret_cast<char *>(&Size), sizeof(size_t));
				File.read(reinterpret_cast<char *>(leftHead), dataNodeSize);
				File.read(reinterpret_cast<char *>(root), idxNodeSize);
			}
			else {
				root = new idxNode;
				leftHead = new dataNode;
				_offset = 2 * sizeof(size_t) + dataNodeSize + idxNodeSize;
				Size = 0;
				File.open(path.c_str(), ios::app | ios::out);
				File.close();
				File.open(path.c_str(), ios::binary | ios::ate | ios::out | ios::in);
				File.seekp(0, ios::beg);
				File.write(reinterpret_cast<char *>(&_offset), sizeof(size_t));
				File.write(reinterpret_cast<char *>(&Size), sizeof(size_t));
				File.write(reinterpret_cast<char *>(leftHead), dataNodeSize);
				File.write(reinterpret_cast<char *>(root), idxNodeSize);
			}
		}

		//destructor
		~Bplustree() {
			delete root;
			leftHead = NULL;
			File.close();
		}

		//find data
		T search(const Key &_k) {
			return idxSearch(_k, root);
		}

		bool find(const Key &_k) {
			return idxFind(_k, root);
		}

		//insert node
		void insert(const Key &_k, const T &_data) {
			if (root->keyNum == -1) {
				root->isLeaf = true;
				root->keyNum++;
				leftHead->keyNum = 1;
				leftHead->key[0] = _k;
				leftHead->data[0] = _data;
				root->offset[0] = 2 * sizeof(size_t);
				File.seekp(2 * sizeof(size_t), ios::beg);
				File.write(reinterpret_cast<char *>(leftHead), dataNodeSize);
				File.write(reinterpret_cast<char *>(root), idxNodeSize);
				Size++;
				return;
			}
			idxNode *q = insert(_k, _data, root);
			if (q != NULL) {
				idxNode *t = root;
				root = new idxNode;
				root->keyNum += 2;
				idxNode *e = new idxNode;
				*e = *q;
				while (e->isLeaf == 0) {
					File.seekg(e->offset[0], ios::beg);
					File.read(reinterpret_cast<char *>(e), idxNodeSize);
				}
				dataNode *d = new dataNode;
				File.seekg(e->offset[0], ios::beg);
				File.read(reinterpret_cast<char *>(d), dataNodeSize);
				root->key[0] = d->key[0];
				//delete d;
				delete e;
				root->offset[0] = _offset;
				root->offset[1] = _offset + idxNodeSize;
				File.seekp(_offset, ios::beg);
				File.write(reinterpret_cast<char *>(t), idxNodeSize);
				File.write(reinterpret_cast<char *>(q), idxNodeSize);
				_offset += 2 * idxNodeSize;
				delete t;
				delete q;
			}
		}

		//erase node
		void erase(const Key &_k) {
			if (root->keyNum != -1) {
				idxNode *r = erase(_k, root);
				if (r == root) {
					File.seekg(root->offset[0], ios::beg);
					File.read(reinterpret_cast<char *>(root), idxNodeSize);
				}
				delete r;
			}
		}

		bool empty() {
			return root == NULL;
		}

		size_t size() {
			return Size;
		}

		Iterator begin() {
			return Iterator(this, leftHead);
		}

		void clear() {
			ofstream file;
			file.open(path.c_str(), ios::app);
			file.close();
		}

		void Print() {
			Printidx(root);
		}

		void Printidx(idxNode *t) {
			cout << t->keyNum << ' ';
			if (t->keyNum != 0 && t->keyNum != -1) cout << (t->key[0]).num() << endl;
			for (int i = 0; i < t->keyNum; i++)
				cout << (t->key[i]).num() << ' ';
			cout << endl;
			if (!t->isLeaf) {
				for (int i = 0; i <= t->keyNum; i++) {
					idxNode *p = new idxNode;
					File.seekg(t->offset[i]);
					File.read(reinterpret_cast<char *>(p), idxNodeSize);
					Printidx(p);
					delete p;
				}
			}
			else {
				for (int i = 0; i <= t->keyNum; i++) {
					dataNode *p = new dataNode;
					File.seekg(t->offset[i]);
					File.read(reinterpret_cast<char *>(p), dataNodeSize);
					Printdata(p);
					//delete p;
				}
			}
		}

		void Printdata(dataNode *t) {
			cout << t->keyNum << ' ' << (t->key[0]).num() << endl;
			for (int i = 0; i < t->keyNum; i++)
				cout << (t->key[i]).num() << ' ' << (t->data[i]).num() << ' ';
			cout << endl;
		}
	};
}
#endif
