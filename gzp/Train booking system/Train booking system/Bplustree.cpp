#include "Bplustree.h"
using namespace sjtu;

template <class Key, class T>
T Bplustree<Key, T>::idxSearch(const Key &_k, idxNode *t) {
	for (int i = 0; i < t->keyNum; i++) {
		if (t->key[i] > _k) {
			List.seekg(t->offset[i], ios::beg);
			if (t->isLeaf) {
				List.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
				return dataSearch(_k, (Bplustree<Key, T>::dataNode *)t->child[i]);
			}
			else {
				List.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
				return idxSearch(_k, (Bplustree<Key, T>::idxNode *)t->child[i]);
			}
		}
	}
}

template <class Key, class T>
T Bplustree<Key, T>::dataSearch(const Key &_k, dataNode *t) {
	for (int i = 0; i < t->keyNum; i++) {
		if (t->key[i] == _k)
			return t->data[i];
	}
}

template <class Key, class T>
bool Bplustree<Key, T>::idxFind(const Key &_k, idxNode *t) {
	for (int i = 0; i < t->keyNum; i++) {
		if (t->key[i] > _k) {
			List.seekg(t->offset[i], ios::beg);
			if (t->isLeaf) {
				List.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
				return dataFind(_k, (Bplustree<Key, T>::dataNode *)t->child[i]);
			}
			else {
				List.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
				return idxFind(_k, (Bplustree<Key, T>::idxNode *)t->child[i]);
			}
		}
	}
}

template <class Key, class T>
bool Bplustree<Key, T>::dataFind(const Key &_k, dataNode *t) {
	bool found = 0;
	for (int i = 0; i < t->keyNum; i++) {
		if (t->key[i] == _k) {
			found = 1;
			return true;
		}
	}
	if (!found) return false;
}

template <class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::insert(const Key &_k, const T &_data, Bplustree<Key, T>::idxNode *t) {
	void *newNode;

	int i;
	for (i = 0; i < t->keyNum; i++)
		if (_k < t->key[i]) break;

	if (t->isLeaf == 0) {
		List.seekg(t->offset[i], ios::beg);
		List.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
		newNode = insert(_k, _data, (Bplustree<Key, T>::idxNode *)t->child[i]);
		List.seekp(t->offset[i], ios::beg);
		List.write(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
	}
	else {
		List.seekg(t->offset[i], ios::beg);
		List.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
		newNode = insertData(_k, _data, (Bplustree<Key, T>::dataNode *)t->child[i]);
		List.seekp(t->offset[i], ios::beg);
		List.write(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::dataNodeSize);
	}

	if (newNode == NULL) return NULL;
	else {
		Bplustree<Key, T>::idxNode *newIdx;
		if (t->isLeaf == 0)	return addIdxBlk((Bplustree<Key, T>::idxNode *)newNode, t);
		else return addDataBlk((Bplustree<Key, T>::dataNode *)newNode, t);
	}
}

template <class Key, class T>
Bplustree<Key, T>::dataNode
* Bplustree<Key, T>::insertData(const Key &_k, const T &_data, Bplustree<Key, T>::dataNode *t) {
	/*for (int i = 0; i <= t->keyNum; i++)
	t->data[i].read();*/
	int i;
	for (i = t->keyNum; i > 0 && _k < t->key[i - 1]; i--) {
		t->key[i] = t->key[i - 1];
		t->data[i] = t->data[i - 1];
	}
	t->key[i] = _k;
	t->data[i] = _data;
	//t->data[i].write();	
	++(t->keyNum);

	if (t->keyNum <= Bplustree<Key, T>::maxKeyNum) return NULL;
	else {
		//split
		size_t newKeyNum = Bplustree<Key, T>::maxKeyNum / 2;
		Bplustree<Key, T>::dataNode *newNode = new Bplustree<Key, T>::dataNode(t->key[newKeyNum], t->data[newKeyNum]);
		newNode->keyNum = t->keyNum - newKeyNum;
		for (int i = 1; i < newNode->keyNum; i++) {
			newNode->key[i] = t->key[i + newKeyNum];
			newNode->data[i] = t->data[i + newKeyNum];
		}
		t->keyNum = newKeyNum;
		return newNode;
	}
}

template <class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::addIdxBlk(Bplustree<Key, T>::idxNode *newNode, Bplustree<Key, T>::idxNode *t) {
	Bplustree<Key, T>::idxNode *p = newNode;

	while (p->isLeaf == 0) p = (Bplustree<Key, T>::idxNode *)p->child[0];
	Bplustree<Key, T>::dataNode *d = (Bplustree<Key, T>::dataNode *)p->child[0];

	Key min = d->key[0];

	int i;
	size_t _offset = idxNodeNum * Bplustree<Key, T>::idxNodeSize + dataNodeNum * Bplustree<Key, T>::dataNodeSize;
	for (i = t->keyNum; i > 0 && min < t->key[i - 1]; i--) {
		t->key[i] = t->key[i - 1];
		t->child[i + 1] = t->child[i];
		t->offset[i + 1] = t->offset[i];
	}
	t->child[i + 1] = newNode;
	t->key[i] = min;
	t->offset[i + 1] = _offset;
	List.seekp(_offset, ios::beg);
	List.write(reinterpret_cast<char *>(newNode), Bplustree<Key, T>::idxNodeSize);
	++(t->keyNum);
	if (t->keyNum <= Bplustree<Key, T>::maxKeyNum) return NULL;
	else {
		//split
		int newKeyNum = Bplustree<Key, T>::maxKeyNum / 2 - 1;
		Bplustree<Key, T>::idxNode *newIdx = new Bplustree<Key, T>::idxNode(t->key[newKeyNum + 1]);
		newIdx->isLeaf = 0;
		newIdx->keyNum = Bplustree<Key, T>::maxKeyNum - 1 - newKeyNum;
		for (int i = 1; i < newIdx->keyNum; i++) {
			newIdx->key[i] = t->key[i + newKeyNum + 1];
			newIdx->child[i] = t->child[i + newKeyNum + 1];
		}
		newIdx->child[0] = t->child[newKeyNum + 1];
		newIdx->child[newIdx->keyNum] = t->child[Bplustree<Key, T>::maxKeyNum];
		t->keyNum = newKeyNum;
		return newIdx;
	}
}

template <class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::addDataBlk(Bplustree<Key, T>::dataNode *newNode, Bplustree<Key, T>::idxNode *t) {
	size_t _offset = idxNodeNum * Bplustree<Key, T>::idxNodeSize + dataNodeNum * Bplustree<Key, T>::dataNodeSize;
	int i;
	for (i = t->keyNum; i > 0 && newNode->key[0] < t->key[i - 1]; i--) {
		t->key[i] = t->key[i - 1];
		t->child[i + 1] = t->child[i];
		t->offset[i + 1] = t->offset[i];
	}
	t->key[i] = newNode->key[0];
	t->child[i + 1] = newNode;
	t->offset[i + 1] = _offset;
	List.seekp(_offset, ios::beg);
	List.write(reinterpret_cast<char *>(newNode), Bplustree<Key, T>::dataNodeSize);
	++(t->keyNum);
	if (t->keyNum <= Bplustree<Key, T>::maxKeyNum) return NULL;
	else {
		//split
		size_t newKeyNum = Bplustree<Key, T>::maxKeyNum / 2 - 1;
		Bplustree<Key, T>::idxNode *newIdx = new Bplustree<Key, T>::idxNode(t->key[newKeyNum + 1]);
		newIdx->keyNum = Bplustree<Key, T>::maxKeyNum - newKeyNum - 1;
		newIdx->isLeaf = 1;
		for (int i = 1; i < newIdx->keyNum; i++) {
			newIdx->key[i] = t->key[i + newKeyNum + 1];
			newIdx->child[i] = t->child[i + newKeyNum + 1];
		}
		newIdx->child[0] = t->child[newKeyNum + 1];
		newIdx->child[newIdx->keyNum] = t->child[Bplustree<Key, T>::maxKeyNum];
		t->keyNum = newKeyNum;
		return newIdx;
	}
}

template<class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::erase(const Key &_k, Bplustree<Key, T>::idxNode *t) {
	if (t->keyNum > miniDataNum) {
		int i;
		for (i = 0; i < t->keyNum; i++)
			if (t->key[i] == _k) break;
		--(t->keyNum);
		for (; i < t->keyNum; i++)
			t->child[i] = t->child[i + 1];
		return NULL;
	}
	else return t;
}

template <class Key, class T>
Bplustree<Key, T>::dataNode
* Bplustree<Key, T>::eraseData(const Key &_k, Bplustree<Key, T>::dataNode *t) {
	if (!find(_k)) throw index_out_of_bound();

	void *Node;
	int i;
	for (int i = 0; i < t->keyNum; ++i)
		if (x < t->key[i]) break;

	if (t->isLeaf == 0) Node = erase(_k, (Bplustree<Key, T>::idxNode *)t->child[i]);
	else Node = eraseData(_k, (Bplustree<Key, T>::dataNode *)t->child[i]);

	if (Node == NULL) return NULL;
	else {
		if (t == root) {
			root = t->child[0];
			idxNodeNum--;
		}
		else if (t->isLeaf == 0) return addIdx(Node, t);
		else return addData(Node, t);
	}
}

template <class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::addData(Bplustree<Key, T>::dataNode *n, Bplustree<Key, T>::idxNode *t) {
	int min;
	if (t == root) min = 2;
	else min = miniDataNum;
	int i;
	for (i = 0; i < t->keyNum; i++)
		if (t->child[i] == n) break;
	Bplustree<Key, T>::dataNode *pre = t->child[i - 1];
	Bplustree<Key, T>::dataNode *next = t->child[i + 1];
	if (pre->keyNum == min) {
		for (int j = 0; j < n->keyNum; j++) {
			n->key[j + pre->keyNum] = n->key[j];
			n->data[j + pre->keyNum] = n->data[j];
		}
		for (int j = 0; j < pre->keyNum; j++) {
			n->key[j] = pre->key[j];
			n->data[j] = pre->data[j];
		}
		n->keyNum += pre->keyNum;
		for (int j = i - 1; j < t->keyNum; j++) {
			t->offset[j] = t->offset[j + 1];
			t->child[j] = t->child[j + 1];
			t->key[j] = t->key[j + 1];
		}
		t->keyNum--;
		if (t->keyNum < miniKeyNum) return t;
		else return NULL;
	}
	else if (next->keyNum == min) {
		for (int j = 0; j < next->keyNum; j++) {
			n->key[j + n->keyNum] = next->key[j];
			n->data[j + n->keyNum] = next->data[j];
		}
		n->keyNum += next->keyNum;
		for (int j = i + 1; j < t->keyNum; j++) {
			t->offset[j] = t->offset[j + 1];
			t->child[j] = t->child[j + 1];
			t->key[j] = t->key[j + 1];
		}
		t->keyNum--;
		if (t->keyNum < min) return t;
		else return NULL;
	}
	else {
		n->key[n->keyNum] = next->key[0];
		n->data[n->keyNum] = next->data[0];
		n->keyNum++;
		next->keyNum--;
		for (int j = 0; j < next->keyNum; j++) {
			next->key[j] = next->key[j + 1];
			next->data[j] = next->data[j + 1];
		}
		return NULL;
	}
}

template <class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::addIdx(Bplustree<Key, T>::idxNode *n, Bplustree<Key, T>::idxNode *t) {
	int min;
	if (t == root) min = 2;
	else min = miniDataNum;
	int i;
	for (int i = 0; i < t->keyNum; i++)
		if (t->child[i] == n) break;
	Bplustree<Key, T>::idxNode *pre = t->child[i - 1];
	Bplustree<Key, T>::idxNode *next = t->child[i + 1];
	if (pre->keyNum == min) {
		for (int j = 0; j < n->keyNum; j++) {
			n->key[j + pre->keyNum] = n->key[j];
			n->data[j + pre->keyNum] = n->data[j];
		}
		for (int j = 0; j < pre->keyNum; j++) {
			n->key[j] = pre->key[j];
			n->data[j] = pre->data[j];
		}
		n->keyNum += pre->keyNum;
		for (int j = i - 1; j < t->keyNum; j++) {
			t->offset[j] = t->offset[j + 1];
			t->child[j] = t->child[j + 1];
			t->key[j] = t->key[j + 1];
		}
		t->keyNum--;
		if (t->keyNum < min) return t;
		else return NULL;
	}
	else if (next->keyNum == min) {
		for (int j = 0; j < next->keyNum; j++) {
			n->key[j + n->keyNum] = next->key[j];
			n->data[j + n->keyNum] = next->data[j];
		}
		n->keyNum += next->keyNum;
		for (int j = i + 1; j < t->keyNum; j++) {
			t->offset[j] = t->offset[j + 1];
			t->child[j] = t->child[j + 1];
			t->key[j] = t->key[j + 1];
		}
		t->keyNum--;
		if (t->keyNum < miniKeyNum) return t;
		else return NULL;
	}
	else {
		n->key[n->keyNum] = next->key[0];
		n->data[n->keyNum] = next->data[0];
		n->keyNum++;
		next->keyNum--;
		for (int j = 0; j < next->keyNum; j++) {
			next->key[j] = next->key[j + 1];
			next->data[j] = next->data[j + 1];
		}
		return NULL;
	}
}

template <class Key, class T>
Bplustree<Key, T>::Bplustree(const String &_file) {
	file_name = _file;
	List.open((file_name + ".list").c_str());
	Data.open((file_name + ".data").c_str());
	root = NULL;
	leftHead = NULL;
}

template <class Key, class T>
T Bplustree<Key, T>::search(const Key &_k) {
	List.seekg(0, ios::beg);
	List.read(reinterpret_cast<char *>(root), idxNodeSize);
	if (find(_k)) return idxSearch(_k, root);
	else throw index_out_of_bound();
}

template <class Key, class T>
bool Bplustree<Key, T>::find(const Key &_k) {
	List.seekg(0, ios::beg);
	List.read(reinterpret_cast<char *>(root), idxNodeSize);
	return idxFind(_k, root);
}

template <class Key, class T>
void Bplustree<Key, T>::insert(const Key &_k, const T &_data) {
	List.seekg(0, ios::beg);
	List.read(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
	if (root == NULL) {
		root = new Bplustree<Key, T>::idxNode(_k);
		root->isLeaf = true;
		root->child[0] = new Bplustree<Key, T>::dataNode(_k, _data);
		Bplustree<Key, T>::dataNode *p = (Bplustree<Key, T>::dataNode *)root->child[0];
		List.seekp(0, ios::beg);
		List.write(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
		List.write(reinterpret_cast<char *>(p), Bplustree<Key, T>::dataNodeSize);
		root->offset[0] = Bplustree<Key, T>::idxNodeSize;
		p->nextoffset = root->offset[0] + Bplustree<Key, T>::dataNodeSize;
		//p->data[0].write();
		idxNodeNum++;
		dataNodeNum++;
		return;
	}
	Bplustree<Key, T>::idxNode *p = insert(_k, _data, root);
	if (p != NULL) {
		size_t _offset = idxNodeNum * Bplustree<Key, T>::idxNodeSize + dataNodeNum * Bplustree<Key, T>::dataNodeSize;
		Bplustree<Key, T>::idxNode *t = root;
		root = new Bplustree<Key, T>::idxNode(_k);
		root->isLeaf = true;
		root->child[0] = t;
		root->child[1] = p;
		while (p->isLeaf == 0)
			p = (Bplustree<Key, T>::idxNode *)p->child[0];
		Bplustree<Key, T>::dataNode *d = (Bplustree<Key, T>::dataNode *)p->child[0];
		root->key[0] = d->key[0];
		List.seekp(_offset, ios::beg);
		List.write(reinterpret_cast<char *>(t), Bplustree<Key, T>::idxNodeSize);
		List.write(reinterpret_cast<char *>(p), Bplustree<Key, T>::idxNodeSize);
		root->offset[0] = _offset;
		root->offset[1] = _offset + Bplustree<Key, T>::idxNodeSize;
	}
	List.seekp(0, ios::beg);
	List.write(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
}

template <class Key, class T>
void Bplustree<Key, T>::erase(const Key &_k) {
	List.seekg(0, ios::beg);
	List.read(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
	if (root != NULL) erase(_k, root);
}