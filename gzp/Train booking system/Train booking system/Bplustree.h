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
	private:
		static const int miniDegree = 2;
		static const int maxDegree = 40;
		static const int miniKeyNum = 20;
		static const int maxKeyNum = 39;
		static const int maxn = 110;

		string path;

		//node
		struct idxNode {
			size_t offset[maxn];
			bool isLeaf;
			int keyNum;
			bool isRead;
			void *child[maxn];
			Key key[maxn];

			idxNode() {
				isLeaf = false;
				keyNum = 0;
				isRead = false;
			}

		};

		struct dataNode {
			size_t nextoffset;
			int keyNum;
			Key key[maxn];
			T data[maxn];
			dataNode *Next;
			dataNode() {
				keyNum = 0;
				Next = NULL;
			}
		};

		static const int idxNodeSize = sizeof(idxNode);
		static const int dataNodeSize = sizeof(dataNode);

		//void makeEmpty(idxNode *t);

		T idxSearch(const Key &_k, idxNode *t) {
			int i;
			for (i = 0; i < t->keyNum; i++) {
				if (t->key[i] > _k)	break;
			}
			File.seekg(t->offset[i]);
			if (t->isLeaf) {
				File.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
				return dataSearch(_k, (Bplustree<Key, T>::dataNode *)t->child[i]);
			}
			else {
				File.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
				return idxSearch(_k, (Bplustree<Key, T>::idxNode *)t->child[i]);
			}
		}

		T dataSearch(const Key &_k, dataNode *t) {
			bool found = 0;
			for (int i = 0; i < t->keyNum; i++) {
				if (t->key[i] == _k) {
					found = 1;
					return t->data[i];
				}
			}
			if (!found) throw index_out_of_bound();
		}

		bool idxFind(const Key &_k, idxNode *t) {
			int i;
			for (i = 0; i < t->keyNum; i++) {
				if (t->key[i] > _k)	break;
			}
			File.seekg(t->offset[i]);
			if (t->isLeaf) {
				File.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
				return dataFind(_k, (Bplustree<Key, T>::dataNode *)t->child[i]);
			}
			else {
				File.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
				return idxFind(_k, (Bplustree<Key, T>::idxNode *)t->child[i]);
			}
		}

		bool dataFind(const Key &_k, dataNode *t) {
			bool found = 0;
			for (int i = 0; i < t->keyNum; i++) {
				if (t->key[i] == _k) {
					found = 1;
					return true;
				}
			}
			if (!found) return false;
		}

		idxNode *insert(const Key &_k, const T &_data, idxNode *t) {
			void *newNode;

			int i;
			for (i = 0; i < t->keyNum; i++)
				if (_k < t->key[i]) break;

			if (t->isLeaf == 0) {
				File.seekg(t->offset[i]);
				File.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
				newNode = insert(_k, _data, (Bplustree<Key, T>::idxNode *)t->child[i]);
				File.seekp(t->offset[i]);
				File.write(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
			}
			else {
				File.seekg(t->offset[i]);
				File.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
				newNode = insertData(_k, _data, (Bplustree<Key, T>::dataNode *)t->child[i]);
				File.seekp(t->offset[i]);
				File.write(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
			}

			if (newNode == NULL) return NULL;
			else {
				if (t->isLeaf == 0)	return addIdxBlk((Bplustree<Key, T>::idxNode *)newNode, t);
				else return addDataBlk((Bplustree<Key, T>::dataNode *)newNode, t);
			}
		}

		dataNode *insertData(const Key &_k, const T &_data, dataNode *t) {
			/*for (int i = 0; i <= t->keyNum; i++)
			t->data[i].read();*/
			int i;
			for (i = 0; i < t->keyNum; i++) {
				if (t->key[i] == _k) {
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
			//t->data[i].write();	
			t->keyNum++;
			_offset += dataNodeSize;
			Size++;

			if (t->keyNum <= Bplustree<Key, T>::maxKeyNum) return NULL;
			else {
				//split
				size_t newKeyNum = Bplustree<Key, T>::maxKeyNum / 2;
				Bplustree<Key, T>::dataNode *newNode = new Bplustree<Key, T>::dataNode;
				newNode->keyNum = t->keyNum - newKeyNum;
				for (int i = 0; i < newNode->keyNum; i++) {
					newNode->key[i] = t->key[i + newKeyNum];
					newNode->data[i] = t->data[i + newKeyNum];
				}
				t->keyNum = newKeyNum;
				_offset += idxNodeSize;
				newNode->Next = t->Next;
				newNode->nextoffset = t->nextoffset;
				t->Next = newNode;
				t->nextoffset = _offset;
				return newNode;
			}
		}

		idxNode *addIdxBlk(idxNode *newNode, idxNode *t) {
			Bplustree<Key, T>::idxNode *p = newNode;

			while (p->isLeaf == 0) p = (Bplustree<Key, T>::idxNode *)p->child[0];
			Bplustree<Key, T>::dataNode *d = (Bplustree<Key, T>::dataNode *)p->child[0];

			Key min = d->key[0];

			int i;
			for (i = t->keyNum; i > 0 && min < t->key[i - 1]; i--) {
				t->key[i] = t->key[i - 1];
				t->child[i + 1] = t->child[i];
				t->offset[i + 1] = t->offset[i];
			}
			t->child[i + 1] = newNode;
			t->key[i] = min;
			t->offset[i + 1] = _offset;
			File.seekp(_offset);
			File.write(reinterpret_cast<char *>(newNode), Bplustree<Key, T>::idxNodeSize);
			t->keyNum++;
			if (t->keyNum <= Bplustree<Key, T>::maxKeyNum) return NULL;
			else {
				//split
				int newKeyNum = Bplustree<Key, T>::maxKeyNum / 2;
				Bplustree<Key, T>::idxNode *newIdx = new Bplustree<Key, T>::idxNode;
				newIdx->isLeaf = 0;
				newIdx->keyNum = t->keyNum - 1 - newKeyNum;
				for (int i = 0; i < newIdx->keyNum; i++) {
					newIdx->key[i] = t->key[i + newKeyNum + 1];
					newIdx->child[i] = t->child[i + newKeyNum + 1];
					newIdx->offset[i] = t->offset[i + newKeyNum + 1];
				}
				newIdx->child[newIdx->keyNum] = t->child[t->keyNum];
				newIdx->offset[newIdx->keyNum] = t->offset[t->keyNum];
				t->keyNum = newKeyNum;
				_offset += idxNodeSize;
				return newIdx;
			}
		}

		idxNode *addDataBlk(dataNode *newNode, idxNode *t) {
			int i;
			for (i = t->keyNum; i > 0 && newNode->key[0] < t->key[i - 1]; i--) {
				t->key[i] = t->key[i - 1];
				t->child[i + 1] = t->child[i];
				t->offset[i + 1] = t->offset[i];
			}
			t->key[i] = newNode->key[0];
			t->child[i + 1] = newNode;
			t->offset[i + 1] = _offset;
			File.seekp(_offset);
			File.write(reinterpret_cast<char *>(newNode), Bplustree<Key, T>::dataNodeSize);
			t->keyNum++;
			if (t->keyNum <= Bplustree<Key, T>::maxKeyNum) return NULL;
			else {
				//split
				size_t newKeyNum = Bplustree<Key, T>::maxKeyNum / 2;
				Bplustree<Key, T>::idxNode *newIdx = new Bplustree<Key, T>::idxNode;
				newIdx->keyNum = t->keyNum - newKeyNum - 1;
				newIdx->isLeaf = 1;
				for (int i = 0; i < newIdx->keyNum; i++) {
					newIdx->key[i] = t->key[i + newKeyNum + 1];
					newIdx->child[i] = t->child[i + newKeyNum + 1];
					newIdx->offset[i] = t->offset[i + newKeyNum + 1];
				}
				newIdx->child[newIdx->keyNum] = t->child[t->keyNum];
				newIdx->offset[newIdx->keyNum] = t->offset[t->keyNum];
				t->keyNum = newKeyNum;
				_offset += idxNodeSize;
				return newIdx;
			}
		}

		idxNode *erase(const Key &_k, idxNode *t) {
			void *newNode;

			int i;
			for (i = 0; i < t->keyNum; i++)
				if (_k < t->key[i]) break;

			if (t->isLeaf == 0) {
				File.seekg(t->offset[i]);
				File.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
				newNode = erase(_k, (Bplustree<Key, T>::idxNode *)t->child[i]);
				File.seekp(t->offset[i]);
				File.write(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
			}
			else {
				File.seekg(t->offset[i]);
				File.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
				newNode = eraseData(_k, (Bplustree<Key, T>::dataNode *)t->child[i]);
				File.seekp(t->offset[i]);
				File.write(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
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

			if (t->keyNum >= miniKeyNum) return NULL;
			else return t;
		}

		idxNode *addData(dataNode *n, idxNode *t) {
			int min;
			if (t == root) min = 2;
			else min = miniKeyNum;
			
			int i;
			for (i = 0; i <= t->keyNum; i++) {
				if (t->child[i] == n) break;
			}

			if (i != t->keyNum) {
				dataNode *next = (dataNode *)t->child[i + 1];
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
					n->Next = next->Next;
					n->nextoffset = next->nextoffset;
					delete next;
					t->keyNum--;
					for (int j = i; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->child[j + 1] = t->child[j + 2];
						t->offset[j + 1] = t->offset[j + 2];
					}
				}
				File.seekp(t->offset[i]);
				File.write(reinterpret_cast<char *>(n), Bplustree<Key, T>::dataNodeSize);
				if (next != NULL) {
					File.seekp(t->offset[i + 1]);
					File.write(reinterpret_cast<char *>(next), Bplustree<Key, T>::dataNodeSize);
				}	
			}
			else if (i != 0) {
				dataNode *pre = (dataNode *)t->child[i - 1];
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
					pre->Next = n->Next;
					pre->nextoffset = n->nextoffset;
					delete n;
					t->keyNum--;
					for (int j = i - 1; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->child[j + 1] = t->child[j + 2];
						t->offset[j + 1] = t->offset[j + 2];
					}
				}
				File.seekp(t->offset[i - 1]);
				File.write(reinterpret_cast<char *>(pre), Bplustree<Key, T>::dataNodeSize);
				if (n != NULL) {
					File.seekp(t->offset[i]);
					File.write(reinterpret_cast<char *>(n), Bplustree<Key, T>::dataNodeSize);
				}	
			}

			if (t->keyNum >= min) return NULL;
			else return t;
		}

		idxNode *addIdx(idxNode *n, idxNode *t) {
			int min;
			if (t == root) min = 2;
			else min = miniKeyNum;

			int i;
			for (i = 0; i <= t->keyNum; i++) {
				if (t->child[i] == n) break;
			}

			if (i != t->keyNum) {
				idxNode *next = (idxNode *)t->child[i + 1];
				idxNode *d = next;
				while (d->isLeaf == 0) d = (idxNode *)d->child[0];
				dataNode *e = (dataNode *)d->child[0];
				if (next->keyNum > miniKeyNum) {
					//borrow
					n->key[n->keyNum] = e->key[0];
					n->child[n->keyNum + 1] = next->child[0];
					n->offset[n->keyNum + 1] = next->offset[0];
					n->keyNum++;
					next->keyNum--;
					int j;
					for (j = 0; j < next->keyNum; j++) {
						next->key[j] = next->key[j + 1];
						next->child[j] = next->child[j + 1];
						next->offset[j] = next->offset[j + 1];
					}
					next->child[j] = next->child[j + 1];
					next->offset[j] = next->offset[j + 1];
				}
				else {
					//merge
					n->key[n->keyNum] = e->key[0];
					int j;
					for (j = 0; j <= next->keyNum; j++) {
						n->key[n->keyNum + j + 1] = next->key[j];
						n->child[n->keyNum + j + 1] = next->child[j];
						n->offset[n->keyNum + j + 1] = next->offset[j];
					}
					n->child[n->keyNum + j + 1] = next->child[j];
					n->offset[n->keyNum + j + 1] = next->offset[j];
					n->keyNum += next->keyNum + 1;
					delete next;
					t->keyNum--;
					for (int j = i; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->child[j + 1] = t->child[j + 2];
						t->offset[j + 1] = t->offset[j + 2];
					}
				}
				File.seekp(t->offset[i]);
				File.write(reinterpret_cast<char *>(n), Bplustree<Key, T>::dataNodeSize);
				if (n != NULL) {
					File.seekp(t->offset[i + 1]);
					File.write(reinterpret_cast<char *>(next), Bplustree<Key, T>::dataNodeSize);
				}
			}
			else if (i != 0) {
				idxNode *pre = (idxNode *)t->child[i - 1];
				idxNode *d = n;
				while (d->isLeaf == 0) d = (idxNode *)d->child[0];
				dataNode *e = (dataNode *)d->child[0];
				if (pre->keyNum > miniKeyNum) {
					//borrow
					n->child[n->keyNum + 1] = n->child[n->keyNum];
					n->offset[n->keyNum + 1] = n->offset[n->keyNum];
					int j;
					for (j = n->keyNum; j > 0; j--) {
						n->key[j] = n->key[j - 1];
						n->child[j] = n->child[j - 1];
						n->offset[j] = n->offset[j - 1];
					}
					n->key[0] = e->key[0];
					n->child[0] = pre->child[pre->keyNum];
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
						pre->child[pre->keyNum + j + 1] = n->child[j];
						pre->offset[pre->keyNum + j + 1] = n->offset[j];
					}
					pre->child[pre->keyNum + j + 1] = n->child[j];
					pre->offset[pre->keyNum + j + 1] = n->offset[j];
					pre->keyNum += n->keyNum + 1;
					delete n;
					t->keyNum--;
					for (int j = i - 1; j < t->keyNum; j++) {
						t->key[j] = t->key[j + 1];
						t->child[j + 1] = t->child[j + 2];
						t->offset[j + 1] = t->offset[j + 2];
					}
				}
				File.seekp(t->offset[i - 1]);
				File.write(reinterpret_cast<char *>(pre), Bplustree<Key, T>::dataNodeSize);
				if (n != NULL) {
					File.seekp(t->offset[i]);
					File.write(reinterpret_cast<char *>(n), Bplustree<Key, T>::dataNodeSize);
				}
			}

			if (t->keyNum >= min) return NULL;
			else return t;
		}

		idxNode *root = NULL;
		dataNode *leftHead = NULL;
		size_t _offset;
		size_t Size = 0;
		fstream File;

	public:

		class iterator {
			friend class Bplustree;
		private:
			Bplustree * tree_ptr = NULL;
			dataNode *node_ptr = NULL;

		public:
			iterator() = default;
			iterator(Bplustree *_t, dataNode *_n) {
				tree_ptr = _t;
				node_ptr = _n;
			}
			iterator(const iterator &other) {
				tree_ptr = other.tree_ptr;
				node_ptr = other.node_ptr;
			}

			iterator operator++(int) {
				iterator *tmp = this;
				node_ptr = node_ptr->Next;
				if (node_ptr == NULL) throw index_out_of_bound();
				return *tmp;
			}

			iterator &operator++() {
				node_ptr = node_ptr->Next;
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
		//constructor
		Bplustree(const string &_file) {
			path = "C:\\Users\\gzp\\Desktop\\data structure\\train order system\\Train booking system\\dataBase\\" + _file;
			ofstream newFile(path + ".txt");
			File.open(path + ".txt");
			root = NULL;
			leftHead = NULL;
		}

		//destructor
		~Bplustree() {
			root = NULL;
			leftHead = NULL;

			File.close();
		}

		//find data
		T search(const Key &_k) {
			File.seekg(0);
			File.read(reinterpret_cast<char *>(root), idxNodeSize);
			return idxSearch(_k, root);
		}

		bool find(const Key &_k) {
			File.seekg(0);
			File.read(reinterpret_cast<char *>(root), idxNodeSize);
			return idxFind(_k, root);
		}

		//insert node
		void insert(const Key &_k, const T &_data) {
			File.seekg(0);
			File.read(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
			if (root == NULL) {
				root = new Bplustree<Key, T>::idxNode;
				root->isLeaf = true;
				root->child[0] = new Bplustree<Key, T>::dataNode;
				Bplustree<Key, T>::dataNode *p = (Bplustree<Key, T>::dataNode *)root->child[0];
				leftHead = p;
				p->keyNum = 1;
				p->key[0] = _k;
				p->data[0] = _data;
				root->offset[0] = Bplustree<Key, T>::idxNodeSize;
				File.seekp(0);
				File.write(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
				File.write(reinterpret_cast<char *>(p), Bplustree<Key, T>::dataNodeSize);
				_offset += idxNodeSize;
				_offset += dataNodeSize;
				Size++;
				return;
			}
			Bplustree<Key, T>::idxNode *p = insert(_k, _data, root);
			if (p != NULL) {
				Bplustree<Key, T>::idxNode *t = root;
				root = new Bplustree<Key, T>::idxNode;
				root->keyNum++;
				root->child[0] = t;
				root->child[1] = p;
				while (p->isLeaf == 0)
					p = (Bplustree<Key, T>::idxNode *)p->child[0];
				Bplustree<Key, T>::dataNode *d = (Bplustree<Key, T>::dataNode *)p->child[0];
				root->key[0] = d->key[0];
				root->offset[0] = _offset;
				root->offset[1] = _offset + Bplustree<Key, T>::idxNodeSize;
				File.seekp(_offset);
				File.write(reinterpret_cast<char *>(t), Bplustree<Key, T>::idxNodeSize);
				File.write(reinterpret_cast<char *>(p), Bplustree<Key, T>::idxNodeSize);
			}
			File.seekp(0);
			File.write(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
		}

		//erase node
		void erase(const Key &_k) {
			File.seekg(0);
			File.read(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
			if (root != NULL) {
				idxNode *r = erase(_k, root);
				if (r == root) root = (idxNode *)r->child[0];
				delete r;
			}
			File.seekp(0);
			File.write(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
		}

		bool empty() {
			return root == NULL;
		}

		size_t size() {
			return Size;
		}

		iterator begin() {
			return iterator(this, leftHead);
		}
	};

}
#endif
