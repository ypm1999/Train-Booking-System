#ifndef _Bplus
#define _Bplus

#include <iostream>
#include <cstring>
#include "exceptions.h"
#include <cmath>
#include <cstdio>
#include <fstream>
#include "mystring.h"

namespace sjtu {

	template <class Key, class T>
	class Bplustree {
		friend class Iterator;
	public:
		class Iterator;
	private:
		static const int maxKeyNum = 20;
		static const int miniKeyNum = maxKeyNum / 2;
		static const int maxn = maxKeyNum + 2;

		mystring<20> path;

		//node
		struct idxNode {
			int offset[maxn];
			bool isLeaf;
			int keyNum;
			Key key[maxn];
			Key miniKey;

			idxNode() {
				isLeaf = false;
				keyNum = -1;
			}

		};

		struct dataNode {
			int Offset;
			int nextoffset;
			int keyNum;
			Key key[maxn];
			T data[maxn];
			dataNode() {
				nextoffset = -1;
				keyNum = 0;
			}
		};

		const int idxNodeSize = sizeof(idxNode);
		const int dataNodeSize = sizeof(dataNode);

		Iterator *idxSearch(const Key &_k, const idxNode &t) {
			int i;
			for (i = 0; i < t.keyNum; i++) {
				if (_k < t.key[i])	break;
			}
			File.seekg(t.offset[i], std::ios::beg);
			if (t.isLeaf) {
				dataNode p;
				File.read(reinterpret_cast<char *>(&p), dataNodeSize);
				return dataSearch(_k, p);
			}
			else {
				idxNode p;
				File.read(reinterpret_cast<char *>(&p), idxNodeSize);
				return idxSearch(_k, p);
			}
		}

		Iterator *dataSearch(const Key &_k, const dataNode &t) {
			Iterator *tmp;
			for (int i = 0; i < t.keyNum; i++) {
				if (t.key[i] == _k) {
					tmp = new Iterator(this, t, i);
					return tmp;
				}
			}
			tmp = new Iterator(this, t, -1);
			return tmp;
		}

		Iterator *antiLower_bound(const Key &_k, const idxNode &t) {
			int i;
			for (i = t.keyNum - 1; i >= 0 && !(_k > t.key[i]); i--);

			File.seekg(t.offset[i + 1], std::ios::beg);
			if (t.isLeaf) {
				dataNode p;
				File.read(reinterpret_cast<char *>(&p), dataNodeSize);
				return antiLower_boundData(_k, p);
			}
			else {
				idxNode p;
				File.read(reinterpret_cast<char *>(&p), idxNodeSize);
				return antiLower_bound(_k, p);
			}
		}

		Iterator *antiLower_boundData(const Key &_k, const dataNode &t) {
			Iterator *tmp;
			for (int i = t.keyNum - 1; i >= 0; i--) {
				if (!(t.key[i] > _k)) {
					tmp = new Iterator(this, t, i);
					return tmp;
				}
			}
			tmp = new Iterator(this, t, 0);
			return tmp;
		}

		idxNode *insert(const Key &_k, const T &_data, idxNode *t) {
			void *newNode;

			int i = 0;
			for (i = 0; i < t->keyNum; i++)
				if (_k < t->key[i]) break;

			if (t->isLeaf) {
				File.seekg(t->offset[i], std::ios::beg);
				dataNode *p = new dataNode;
				File.read(reinterpret_cast<char *>(p), dataNodeSize);
				newNode = insertData(_k, _data, p);
				if (i == 0) t->miniKey = p->key[0];
				else t->key[i - 1] = p->key[0];

				File.seekp(t->offset[i], std::ios::beg);
				File.write(reinterpret_cast<char *>(p), dataNodeSize);
//				File.flush();
				delete p;
			}
			else {
				File.seekg(t->offset[i], std::ios::beg);
				idxNode *p = new idxNode;
				File.read(reinterpret_cast<char *>(p), idxNodeSize);
				newNode = insert(_k, _data, p);
				if (i == 0) t->miniKey = p->miniKey;
				else t->key[i - 1] = p->miniKey;
				File.seekp(t->offset[i], std::ios::beg);
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
		//			puts("-----");
					t->data[i] = _data;
					return NULL;
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
			File.seekp(0, std::ios::beg);
			File.write(reinterpret_cast<char *>(&Size), sizeof(int));
//			File.flush();

			if (t->keyNum <= maxKeyNum) return NULL;
			else {
				//split
				int newKeyNum = maxKeyNum / 2;
				dataNode *newNode = new dataNode;
				newNode->keyNum = t->keyNum - newKeyNum;
				for (int j = 0; j < newNode->keyNum; j++) {
					newNode->key[j] = t->key[j + newKeyNum];
					newNode->data[j] = t->data[j + newKeyNum];
				}
				t->keyNum = newKeyNum;
				newNode->nextoffset = t->nextoffset;
				t->nextoffset = _offset;
				newNode->Offset = _offset;
				return newNode;
			}
		}

		idxNode *addIdxBlk(idxNode *newNode, idxNode *t) {
			Key min = newNode->miniKey;

			int i;
			for (i = t->keyNum; i > 0 && min < t->key[i - 1]; i--) {
				t->key[i] = t->key[i - 1];
				t->offset[i + 1] = t->offset[i];
			}
			t->key[i] = min;
			t->offset[i + 1] = _offset;
			t->keyNum++;
			File.seekp(_offset, std::ios::beg);
			File.write(reinterpret_cast<char *>(newNode), idxNodeSize);
//			File.flush();
			_offset += idxNodeSize;
			File.seekp(sizeof(int), std::ios::beg);
			File.write(reinterpret_cast<char *>(&_offset), sizeof(int));
//			File.flush();
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
				newIdx->miniKey = t->key[newKeyNum];
				for (int j = 0; j < newIdx->keyNum; j++) {
					newIdx->key[j] = t->key[j + newKeyNum + 1];
					newIdx->offset[j] = t->offset[j + newKeyNum + 1];
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

			File.seekp(_offset, std::ios::beg);
			File.write(reinterpret_cast<char *>(newNode), dataNodeSize);
			_offset += dataNodeSize;
			File.seekp(sizeof(int), std::ios::beg);
			File.write(reinterpret_cast<char *>(&_offset), sizeof(int));
//			File.flush();
			if (t->keyNum <= Bplustree<Key, T>::maxKeyNum) {
				delete newNode;
				return NULL;
			}
			else {
				//split
				int newKeyNum = Bplustree<Key, T>::maxKeyNum / 2;
				idxNode *newIdx = new idxNode;
				newIdx->keyNum = t->keyNum - newKeyNum - 1;
				newIdx->miniKey = t->key[newKeyNum];
				newIdx->isLeaf = 1;
				for (int j = 0; j < newIdx->keyNum; j++) {
					newIdx->key[j] = t->key[j + newKeyNum + 1];
					newIdx->offset[j] = t->offset[j + newKeyNum + 1];
				}
				newIdx->offset[newIdx->keyNum] = t->offset[t->keyNum];
				t->keyNum = newKeyNum;
				delete newNode;
				return newIdx;
			}
		}

		idxNode *erase(const Key &_k, idxNode *t) {
			void *newNode;

			int i;
			for (i = 0; i < t->keyNum; i++)
				if (_k < t->key[i]) break;

			if (!t->isLeaf) {
				//putidx(t->offset[i]);
				File.seekg(t->offset[i], std::ios::beg);
				idxNode *p = new idxNode;
				File.read(reinterpret_cast<char *>(p), idxNodeSize);
				newNode = erase(_k, p);
				if (i == 0) t->miniKey = p->miniKey;
				else t->key[i - 1] = p->miniKey;
				File.seekp(t->offset[i], std::ios::beg);
				File.write(reinterpret_cast<char *>(p), idxNodeSize);
				File.flush();
				if (newNode == NULL) delete p;
			}
			else {
				//putData(t->offset[i]);
				File.seekg(t->offset[i], std::ios::beg);
				dataNode *p = new dataNode;
				File.read(reinterpret_cast<char *>(p), dataNodeSize);
				newNode = eraseData(_k, p);
				if (i == 0) t->miniKey = p->key[0];
				else t->key[i - 1] = p->key[0];
				File.seekp(t->offset[i], std::ios::beg);
				File.write(reinterpret_cast<char *>(p), dataNodeSize);
				File.flush();
				if (newNode == NULL) delete p;
			}

			if (newNode == NULL) return NULL;
			else {

				if (t->isLeaf == 0)	return addIdx((idxNode *)newNode, t);
				else return addData((dataNode *)newNode, t);
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
			if (!found) return NULL;

			for (; i < t->keyNum - 1; i++) {
				t->key[i] = t->key[i + 1];
				t->data[i] = t->data[i + 1];
			}
			t->keyNum--;
			Size--;
			File.seekp(0);
			File.write(reinterpret_cast<char *>(&Size), sizeof(int));
			File.flush();
			if (t->keyNum >= miniKeyNum)
				return NULL;
			else return t;
		}

		idxNode *addData(dataNode *n, idxNode *t) {
			int min;
			if (t == root) min = 1;
			else min = miniKeyNum;

			int i;
			for (i = t->keyNum; i > 0 && n->key[0] < t->key[i - 1]; i--);

			if (i != t->keyNum) {
				dataNode *next = new dataNode;
				File.seekg(t->offset[i + 1], std::ios::beg);
				File.read(reinterpret_cast<char *>(next), dataNodeSize);
				if (next->keyNum > miniKeyNum) {
					//borrow
					//cerr << "borrow1 in addData" << endl;
					n->key[n->keyNum] = next->key[0];
					n->data[n->keyNum] = next->data[0];
					n->keyNum++;
					next->keyNum--;
					for (int j = 0; j < next->keyNum; j++) {
						next->key[j] = next->key[j + 1];
						next->data[j] = next->data[j + 1];
					}
					t->key[i] = next->key[0];
					t->key[i - 1] = n->key[0];
					File.seekp(t->offset[i], std::ios::beg);
					File.write(reinterpret_cast<char *>(n), dataNodeSize);
					File.seekp(t->offset[i + 1], std::ios::beg);
					File.write(reinterpret_cast<char *>(next), dataNodeSize);
					File.flush();
				}
				else {
					//merge
					//cerr << "merge1 in addData" << endl;
					for (int j = 0; j < next->keyNum; j++) {
						n->key[n->keyNum + j] = next->key[j];
						n->data[n->keyNum + j] = next->data[j];
					}
					n->keyNum += next->keyNum;
					n->nextoffset = next->nextoffset;

					t->keyNum--;
					//					putData(t->offset[i]);
					//					putData(t->offset[i + 1]);
					for (int j = i; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->offset[j + 1] = t->offset[j + 2];
					}
					t->key[i - 1] = n->key[0];
					File.seekp(t->offset[i], std::ios::beg);
					File.write(reinterpret_cast<char *>(n), dataNodeSize);
					//					putData(t->offset[i]);
					File.flush();
				}
				delete next;
			}
			else if (i != 0) {
				dataNode *pre = new dataNode;
				File.seekg(t->offset[i - 1], std::ios::beg);
				File.read(reinterpret_cast<char *>(pre), dataNodeSize);
				if (pre->keyNum > miniKeyNum) {
					//borrow
					//cerr << "borrow2 in addData" << endl;
					for (int j = n->keyNum; j > 0; j--) {
						n->key[j] = n->key[j - 1];
						n->data[j] = n->data[j - 1];
					}
					n->keyNum++;
					pre->keyNum--;
					n->key[0] = pre->key[pre->keyNum];
					n->data[0] = pre->data[pre->keyNum];
					t->key[i - 1] = n->key[0];
					File.seekp(t->offset[i - 1], std::ios::beg);
					File.write(reinterpret_cast<char *>(pre), dataNodeSize);
					File.seekp(t->offset[i], std::ios::beg);
					File.write(reinterpret_cast<char *>(n), dataNodeSize);
					File.flush();
				}
				else {
					//cerr << "merge2 in addData" << endl;
					//merge
					for (int j = 0; j < n->keyNum; j++) {
						pre->key[pre->keyNum + j] = n->key[j];
						pre->data[pre->keyNum + j] = n->data[j];
					}
					pre->keyNum += n->keyNum;
					pre->nextoffset = n->nextoffset;

					t->keyNum--;
					for (int j = i - 1; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->offset[j + 1] = t->offset[j + 2];
					}
					File.seekp(t->offset[i - 1], std::ios::beg);
					File.write(reinterpret_cast<char *>(pre), dataNodeSize);
					File.flush();
				}
				delete pre;
			}
			delete n;
			if (t->keyNum >= min)
				return NULL;
			else return t;
		}

		idxNode *addIdx(idxNode *n, idxNode *t) {
			int min;
			if (t == root) min = 1;
			else min = miniKeyNum;

			int i;
			for (i = t->keyNum; i > 0 && n->key[0] < t->key[i - 1]; i--);

			if (i != t->keyNum) {
				idxNode *next = new idxNode;
				File.seekg(t->offset[i + 1], std::ios::beg);
				File.read(reinterpret_cast<char *>(next), idxNodeSize);
				if (next->keyNum > miniKeyNum) {
					//borrow
					//cerr << "borrow1 in addIdx" << endl;
					n->key[n->keyNum] = next->miniKey;
					next->miniKey = next->key[0];
					n->offset[n->keyNum + 1] = next->offset[0];
					n->keyNum++;
					next->keyNum--;
					int j;
					for (j = 0; j < next->keyNum; j++) {
						next->key[j] = next->key[j + 1];
						next->offset[j] = next->offset[j + 1];
					}
					next->offset[j] = next->offset[j + 1];
					t->key[i] = next->miniKey;
					File.seekg(t->offset[i], std::ios::beg);
					File.write(reinterpret_cast<char *>(n), idxNodeSize);
					File.seekp(t->offset[i + 1], std::ios::beg);
					File.write(reinterpret_cast<char *>(next), idxNodeSize);
					File.flush();
				}
				else {
					//merge
					//cerr << "merge1 in addIdx" << endl;
					n->key[n->keyNum] = next->miniKey;
					int j;
					for (j = 0; j <= next->keyNum; j++) {
						n->key[n->keyNum + j + 1] = next->key[j];
						n->offset[n->keyNum + j + 1] = next->offset[j];
					}
					n->offset[n->keyNum + j + 1] = next->offset[j];
					n->keyNum += next->keyNum + 1;

					t->keyNum--;
					for (int j = i; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->offset[j + 1] = t->offset[j + 2];
					}
					t->key[i - 1] = n->miniKey;
					File.seekp(t->offset[i], std::ios::beg);
					File.write(reinterpret_cast<char *>(n), idxNodeSize);
					File.flush();
				}
				delete next;
			}
			else if (i != 0) {
				idxNode *pre = new idxNode;
				File.seekg(t->offset[i - 1], std::ios::beg);
				File.read(reinterpret_cast<char *>(pre), idxNodeSize);
				if (pre->keyNum > miniKeyNum) {
					//borrow
					//cerr << "borrow2 in addIdx" << endl;
					n->offset[n->keyNum + 1] = n->offset[n->keyNum];
					int j;
					for (j = n->keyNum; j > 0; j--) {
						n->key[j] = n->key[j - 1];
						n->offset[j] = n->offset[j - 1];
					}
					n->key[0] = n->miniKey;
					n->offset[0] = pre->offset[pre->keyNum];
					n->keyNum++;
					pre->keyNum--;
					n->miniKey = pre->key[pre->keyNum];
					t->key[i - 1] = n->miniKey;
					File.seekg(t->offset[i - 1], std::ios::beg);
					File.write(reinterpret_cast<char *>(pre), idxNodeSize);
					File.seekp(t->offset[i], std::ios::beg);
					File.write(reinterpret_cast<char *>(n), idxNodeSize);
					File.flush();
				}
				else {
					//merge
					//cerr << "merge2 in addIdx" << endl;
					pre->key[pre->keyNum] = n->miniKey;
					int j;
					for (j = 0; j <= n->keyNum; j++) {
						pre->key[pre->keyNum + j + 1] = n->key[j];
						pre->offset[pre->keyNum + j + 1] = n->offset[j];
					}
					pre->offset[pre->keyNum + j + 1] = n->offset[j];
					pre->keyNum += n->keyNum + 1;

					t->keyNum--;
					for (int j = i - 1; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->offset[j + 1] = t->offset[j + 2];
					}
					File.seekp(t->offset[i - 1], std::ios::beg);
					File.write(reinterpret_cast<char *>(pre), idxNodeSize);
					File.flush();
				}
				delete pre;
			}
			delete n;
			if (t->keyNum >= min)
				return NULL;
			else return t;
		}

		idxNode *root = NULL;
		dataNode *leftHead = NULL;
		int _offset = 0;
		int Size = 0;
		std::fstream File;

	public:

		class Iterator {
			friend class Bplustree;
		private:
			Bplustree * tree_ptr;
			dataNode node;
			int pos;

		public:
			Iterator(Bplustree *_t = nullptr, const dataNode &_n = dataNode(), int _pos = -1) :tree_ptr(_t), node(_n), pos(_pos) {}
			Iterator(const Iterator &other) :tree_ptr(other.tree_ptr), node(other.node), pos(other.pos) {}

			Iterator operator++(int) {
				Iterator *tmp = this;
				pos++;
				if (pos >= node.keyNum) {
					if (node.nextoffset == -1) {
						pos = -1;
					}
					else {
						tree_ptr->File.seekg(node.nextoffset, std::ios::beg);
						tree_ptr->File.read(reinterpret_cast<char *>(&node), tree_ptr->dataNodeSize);
						pos = 0;
					}
				}
				return *tmp;
			}

			Iterator &operator++() {
				pos++;
				if (pos >= node.keyNum) {
					if (node.nextoffset == -1) {
						pos = -1;
					}
					else {
						tree_ptr->File.seekg(node.nextoffset, std::ios::beg);
						tree_ptr->File.read(reinterpret_cast<char *>(&node), tree_ptr->dataNodeSize);
						pos = 0;
					}
				}
				return *this;
			}

			T & operator*() {
				return node.data[pos];
			}

			T *operator->() {
				return node.data + pos;
			}

			void save() {
				tree_ptr->File.seekp(node.Offset, std::ios::beg);
				tree_ptr->File.write(reinterpret_cast<char *>(&node), tree_ptr->dataNodeSize);
				tree_ptr->File.flush();
			}


			bool valid() {
				return pos >= 0;
			}
			Key key() {
				return node.key[pos];
			}

			T data() {
				return node.data[pos];
			}
		};

		Bplustree() {
			root = NULL;
			leftHead = NULL;
		}
		//constructor
		Bplustree(const char *file) {
			path = file;

			File.open(path.c_str(), std::ios::binary | std::ios::ate | std::ios::out | std::ios::in);
			leftHead = new dataNode;
			root = new idxNode;
			if (File.is_open()) {
				File.seekg(0, std::ios::beg);
				File.read(reinterpret_cast<char *>(&Size), sizeof(int));
				File.read(reinterpret_cast<char *>(&_offset), sizeof(int));
				File.read(reinterpret_cast<char *>(leftHead), dataNodeSize);
				File.read(reinterpret_cast<char *>(root), idxNodeSize);
			}
			else {
				root = new idxNode;
				leftHead = new dataNode;
				_offset = 2 * sizeof(int) + dataNodeSize + idxNodeSize;
				Size = 0;
				File.open(path.c_str(), std::ios::out);
				File.close();
				File.open(path.c_str(), std::ios::binary | std::ios::ate | std::ios::out | std::ios::in);
				File.seekp(0, std::ios::beg);
				File.write(reinterpret_cast<char *>(&Size), sizeof(int));
				File.write(reinterpret_cast<char *>(&_offset), sizeof(int));
				File.write(reinterpret_cast<char *>(leftHead), dataNodeSize);
				File.write(reinterpret_cast<char *>(root), idxNodeSize);
				File.flush();
			}
		}

		//destructor
		~Bplustree() {
			File.seekp(0, std::ios::beg);
			File.write(reinterpret_cast<char *>(&Size), sizeof(int));
			File.write(reinterpret_cast<char *>(&_offset), sizeof(int));
			//File.write(reinterpret_cast<char *>(leftHead), dataNodeSize);
			File.seekp(sizeof(int) * 2 + dataNodeSize, std::ios::beg);
			File.write(reinterpret_cast<char *>(root), idxNodeSize);
			File.flush();
			delete root;
			delete leftHead;
			File.close();
		}

		Iterator lower_bound(const Key &_k) {
			auto t = antiLower_bound(_k, *root);
			auto tmp = *t;
			delete t;
			if (tmp.key() < _k)
				tmp++;
			return tmp;
		}

		//find data
		Iterator search(const Key &_k) {
			if (empty()) return Iterator(this, dataNode(), -1);
			auto t = idxSearch(_k, *root);
			auto tmp = *t;
			delete t;

			return tmp;
		}

		bool find(const Key &_k) {
			return search(_k).valid();
		}

		//insert node
		void insert(const Key &_k, const T &_data) {
			if (root->keyNum == -1) {
				root->isLeaf = true;
				root->keyNum++;
				root->miniKey = _k;
				leftHead->keyNum = 1;
				leftHead->key[0] = _k;
				leftHead->data[0] = _data;
				leftHead->Offset = 2 * sizeof(int);
				root->offset[0] = 2 * sizeof(int);
				File.seekp(2 * sizeof(int), std::ios::beg);
				File.write(reinterpret_cast<char *>(leftHead), dataNodeSize);
				File.write(reinterpret_cast<char *>(root), idxNodeSize);
				Size++;
				File.seekp(0, std::ios::beg);
				File.write(reinterpret_cast<char *>(&Size), sizeof(int));
				File.flush();
				return;
			}
			idxNode *q = insert(_k, _data, root);
			if (q != NULL) {
				idxNode *t = root;
				root = new idxNode;
				root->keyNum += 2;
				root->miniKey = t->miniKey;
				root->key[0] = q->miniKey;
				root->key[0] = q->miniKey;
				root->offset[0] = _offset;
				root->offset[1] = _offset + idxNodeSize;
				File.seekp(2 * sizeof(int) + dataNodeSize, std::ios::beg);
				File.write(reinterpret_cast<char *>(root), idxNodeSize);
				File.seekp(_offset, std::ios::beg);
				File.write(reinterpret_cast<char *>(t), idxNodeSize);
				File.write(reinterpret_cast<char *>(q), idxNodeSize);
				_offset += 2 * idxNodeSize;
				File.seekp(sizeof(int), std::ios::beg);
				File.write(reinterpret_cast<char *>(&_offset), sizeof(int));
				File.flush();
				delete t;
				delete q;
			}
			File.flush();
		}

		//erase node
		void erase(const Key &_k) {
			if (root->keyNum != -1) {
				idxNode *r = erase(_k, root);
				if (r != NULL) {
					if (r->keyNum != 0) {
						File.seekg(root->offset[0], std::ios::beg);
						File.read(reinterpret_cast<char *>(root), idxNodeSize);
					}
					File.seekp(2 * sizeof(int) + dataNodeSize, std::ios::beg);
					File.write(reinterpret_cast<char *>(root), idxNodeSize);
					File.flush();
				}
			}
		}


		bool empty() {
			return root->keyNum == -1;
		}

		int size() {
			return Size;
		}
		//假begin，不能用
		Iterator begin() {
			return Iterator(this, *leftHead, 0);
		}

		void clear() {
			delete root;
			delete leftHead;
			root = new idxNode;
			leftHead = new dataNode;
			_offset = 2 * sizeof(int) + dataNodeSize + idxNodeSize;
			Size = 0;
			File.open(path.c_str(), std::ios::out);
			File.close();
			File.open(path.c_str(), std::ios::binary | std::ios::ate | std::ios::out | std::ios::in);
			File.seekp(0, std::ios::beg);
			File.write(reinterpret_cast<char *>(&Size), sizeof(int));
			File.write(reinterpret_cast<char *>(&_offset), sizeof(int));
			File.write(reinterpret_cast<char *>(leftHead), dataNodeSize);
			File.write(reinterpret_cast<char *>(root), idxNodeSize);
			File.flush();
		}
		void save(){
			File.flush();
		}
		/*void Print() {
			puts("-------------------------Print Tree----------------------------------");
			Printidx(*root);
			puts("--------------------------End Tree-----------------------------------");
		}

		void Printidx(const idxNode &t) {
			putchar('{');
			printidx(t);
			if (!t.isLeaf) {
				for (int i = 0; i <= t.keyNum; i++) {
					idxNode p;
					File.seekg(t.offset[i]);
					File.read(reinterpret_cast<char *>(&p), idxNodeSize);
					Printidx(p);
				}
			}
			else {
				for (int i = 0; i <= t.keyNum; i++) {
					dataNode p;
					File.seekg(t.offset[i]);
					File.read(reinterpret_cast<char *>(&p), dataNodeSize);
					printdata(p);
				}
			}
			puts("}\n");
		}
		void putData(int offset) {
			dataNode t;
			File.seekg(offset);
			File.read(reinterpret_cast<char *>(&t), dataNodeSize);
			putchar("[");
			printdata(t);
			puts("]");
		}
		void putidx(int offset) {
			idxNode t;
			File.seekg(offset);
			File.read(reinterpret_cast<char *>(&t), idxNodeSize);
			putchar('{');
			cout << endl;
			printidx(t);
			puts("}");
		}
		void printidx(const idxNode &t) {
			printf("idxNode(%d, %d):", t.keyNum, t.miniKey.num());
			for (int i = 0; i < t.keyNum; i++)
				cout << t.key[i].num() << ' ';
			cout << endl;
		}
		void printdata(const dataNode &t) {
			printf("dataNode(%d, %d):", t.keyNum, t.key[0].num());
			for (int i = 0; i < t.keyNum; i++)
				printf("(%d, %d) ", t.key[i].num(), t.data[i].val());
			cout << endl;
		}*/
	};
}
#endif
