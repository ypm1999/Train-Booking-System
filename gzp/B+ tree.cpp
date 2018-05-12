#include "B+ tree.h"

template <class Key, class T>
Bplustree<Key, T>::Bplustree(const String &_file){
	file_name = _file;
	List.open(file_name + ".list");
	Data.open(file_name + ".data");
	root = NULL;
	leftHead = NULL;
}

template <class Key, class T>
Bplustree<Key, T>::~Bplustree() {

}

template <class Key, class T>
bool Bplustree<Key, T>::find(const Key &_k, void *_Node) {
	if (_Node->isLeaf == 1) {
		for (int i = 0; i < _Node->keyNum; i++) {
			if (_Node->key[i] == _k)
				return 1;
		}
		return 0;
	}
	for (int i = 1; i < _Node->keyNum; i++) {
		if (_Node->key[i] > _k) {
			List.seekg(t->offset[i], ios::beg);
			if (_Node->child[i]->isLeaf) List.read(reinterpret_cast<char *>(_Node->child[i]), dataNodeSize);
			else List.read(reinterpret_cast<char *>(_Node->child[i]), idxNodeSize);
			return find(_k, _Node->child[i]);
		}
	}
}

template <class Key, class T>
T Bplustree<Key, T>::search(const Key &_k, void *_Node) {
	if (find(_k)) {
		if (_Node->isLeaf == 1) {
			for (int i = 0; i < _Node->keyNum; i++) {
				if (_Node->key[i] == _k)
					return _Node->data[i];
			}
		}
		for (int i = 1; i < _Node->keyNum; i++) {
			if (_Node->key[i] > _k) {
				List.seekg(t->offset[i], ios::beg);
				if (_Node->child[i]->isLeaf) List.read(reinterpret_cast<char *>(_Node->child[i]), dataNodeSize);
				else List.read(reinterpret_cast<char *>(_Node->child[i]), idxNodeSize);
				return find(_k, _Node->child[i]);
			}
		}
	}
}

template <class Key, class T>
void Bplustree<Key, T>::insert(const Key &_k, const T &_data) {
	List.seekp(0, ios::beg);
	List.read(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
	if (root == NULL) {
		root->child[0] = new dataNode;
		dataNode *p = (dataNode *)root->child[0];
		p->data[0] = _data;
		root->isLeaf = true;
		List.seekg(0, ios::beg);
		List.write(reinterpret_cast<char *>(root), Bplustree<Key, T>::idxNodeSize);
		List.write(reinterpret_cast<char *>(p), Bplustree<Key, T>::dataNodeSize);
		root->offset[0] = Bplustree<Key, T>::idxNodeSize;
		p->nextoffset = root->offset[0] + Bplustree<Key, T>::dataNodeSize;
		//p->data[0].write();
		idxNodeNum++;
		dataNodeNum++;
		return;
	}
	idxNode *p = insert(_k, _data, root);
	if (p != NULL) {
		size_t _offset = idxNodeNum * Bplustree<Key, T>::idxNodeSize + dataNodeNum * Bplustree<Key, T>::dataNodeSize;
		idxNode *t = root;
		root = new idxNode;
		root->isLeaf = false;
		root->keyNum = 1;
		root->child[0] = t;
		root->child[1] = p;
		while (p->isLeaf == 0)
			p = (idxNode *)p->child[0];
		dataNode *d = (dataNode *)p->child[0];
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

template<class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::insert(const Key &_k, const T &_data, idxNode *t) {
	void *newNode;

	int i;
	for (i = 0; i < t->keyNum; i++)
		if (_k < t->key[i]) break;

	if (t->isLeaf == 0) {
		List.seekg(t->offset[i], ios::beg);
		List.read(reinterpret_cast<char *>(t->child[i]), Bplustree<Key, T>::idxNodeSize);
		newNode = insert(_k, _data, (idxNode *)t->child[i]);
		List.seekp(t->offset[i], ios::beg);
		List.write(reinterpret_cast<char *>(t->child[i]), idxNodeSize);
	}
	else {
		List.seekg(t->offset[i], ios::beg);
		List.read(reinterpret_cast<char *>(t->child[i], Bplustree<Key, T>::dataNodeSize));
		newNode = insertData(_k, _data, (dataNode *)t->child[i]);
		List.seekp(t->offset[i], ios::beg);
		List.write(reinterpret_cast<char *>(t->child[i]), dataNodeSize);
	}

	if (newNode == NULL) return NULL;
	else {
		if (t->isLeaf == 0)	return addIdxBlk((idxNode *)newNode, t);
		else return addDataBlk((dataNode *)newNode, t);
	}
}

template<class Key, class T>
Bplustree<Key, T>::dataNode
* Bplustree<Key, T>::insertData(const Key &_k, const T &_data, dataNode *t) {
	/*for (int i = 0; i <= t->keyNum; i++)
		t->data[i].read();*/
	if (t->keyNum < Bplustree<Key, T>::maxKeyNum) {
		for (i = t->keyNum; i > 0 && x < t->key[i - 1]; i--) {
			t->key[i] = t->key[i - 1];
			t->data[i] = t->data[i - 1];
		}
		t->key[i] = _k;
		t->data[i] = _data;
		//t->data[i].write();	
		++(t->keyNum);
		return NULL;
	}
	//split
	dataNode *newNode = new dataNode;
	int i, j;
	int max = Bplustree<Key, T>::maxDataNum / 2;
	newNode->keyNum = max + 1;
	for (i = max, j = Bplustree<Key, T>::maxDataNum; i >= 0 && t->key[j] > _k; i--) {
		newNode->key[i] = t->key[j--];
		newNode->data[i] = t->data[j--];
	}
	if (i >= 0) {
		newNode->key[i--] = _k;
		newNode->data[i--] = _data;
	}
	for (; i >= 0; i--) newNode->key[i] = t->key[j--];

	t->keyNum = Bplustree<Key, T>::maxDataNum - max;
	if (j < t->keyNum - 1) {
		for (; j >= 0 && _k < t->key[j]; j--) t->key[j + 1] = t->key[j];
		t->key[j + 1] = _k;
	}

	return newNode;	
}

template <class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::addDataBlk(dataNode * newNode, idxNode *t) {
	if (t->keyNum < maxKeyNum) {
		size_t _offset = idxNodeNum * Bplustree<Key, T>::idxNodeSize + dataNodeNum * Bplustree<Key, T>::dataNodeSize;
		for (int i = t->keyNum; i > 0 && newNode->key[0] < t->key[i - 1]; i--) {
			t->key[i] = t->key[i - 1];
			t->child[i + 1] = t->child[i];
			t->offset[i + 1] = t->offset[i];
		}
		t->key[i] = newNode->key[0];
		t->child[i + 1] = newNode;
		t->offset[i + 1] = _offset;
		List.seekp(_offset, ios::beg);
		List.write(reinterpret_cast<char *>(t->child[i + 1]), dataNodeSize);
		++(t->keyNum);
		return NULL;
	}
	//split
	idxNode *newIdx = new idxNode;
	newIdx->isLeaf = 1;
	int max = maxKeyNum / 2;
	newIdx->keyNum = max;
	int i, j;

	if (newNode->key[0] > t->key[maxKeyNum - 1]) {
		newIdx->key[max] = newNode->key[0];
		newIdx->child[max + 1] = newNode;
	}
	else {
		newIdx->key[max] = t->key[maxKeyNum - 1];
		newIdx->child[max + 1] = t->child[maxKeyNum];
		for (int i = t->keyNum - 1; i > 0 && newNOde->key[0] < t->key[i - 1]; i--) {
			t->key[i] = t->key[i - 1];
			t->child[i + 1] = t->child[i];
		}
		t->key[i] = newNode->key[0];
		t->child[i + 1] = newNode;
	}

	for (int i = max - 1, j = maxKeyNum; i > 0; i--, j--) {
		newIdx->child[i] = t->child[j];
		newIdx->key[i - 1] = t->key[j - 1];
	}
	newIdx->child[0] = t->child[j];

	t->keyNum = keyNum - max + 1;
	return newIdx;
}

template <class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::addIdxBlk(idxNode *newNode, idxNode *t) {
	idxNode *p = newNode;

	while (p->isLeaf == 0) p = (idxNode *)p->child[0];
	dataNode *d = (dataNode *)p->child[0];

	T min = d->key[0];

	if (t->keyNum < maxKeyNum) {
		size_t _offset = idxNodeNum * Bplustree<Key, T>::idxNodeSize + dataNodeNum * Bplustree<Key, T>::dataNodeSize;
		for (int i = t->keyNum; i > 0 && min < t->key[i - 1]; i--) {
			t->key[i] = t->key[i - 1];
			t->child[i + 1] = t->child[i];
			t->offset[i + 1] = t->offset[i];
		}
		t->child[i + 1] = newNode;
		t->key[i] = min;
		t->offset[i + 1] = _offset;
		List.seekp(_offset, ios::beg);
		List.write(reinterpret_cast<char *>(t->child[i + 1]), idxNodeSize);
		++(t->KeyNum);
		return NULL;
	}

	//split
	idxNode *newIdx = new idxNode;
	newIdx->isLeaf = 0;
	int max = Bplustree<Key, T>::maxKeyNum / 2;
	newIdx->KeyNum = max;
	int i, j;

	if (min > t->key[maxKeyNum - 1]) {
		newIdx->key[max] = min;
		newIdx->child[max + 1] = newNode;	
	}
	else {
		newIdx->key[max] = t->key[maxKeyNum - 1];
		for (int i = t->keyNum - 1; i > 0 && min < t->key[i - 1]; i--) {
			t->key[i] = t->key[i - 1];
			t->child[i + 1] = t->child[i];
		}
		t->key[i] = min;
		t->child[i + 1] = newNode;
	}

	for (i = max, j = maxKeyNum; i > 0; i--, j--) {
		newIdx->child[i] = t->child[j];
		newIdx->key[i - 1] = t->key[j - 1];
	}
	newIdx->child[0] = t->child[j];

	t->keyNum = maxKeyNum - max + 1;
	return newIdx;
}

template<class Key, class T>
void Bplustree<Key, T>::erase(const Key &_k) {
	List.seekg(0, ios::beg);
	List.read(reinterpret_cast<char *>(root), idxNodeSize);
	if (root != NULL) erase(_k, root);
}

template<class Key, class T>
Bplustree<Key, T>::idxNode
* BPlustree<Key, T>::erase(const Key &_k, idxNode *t) {
	if (!find(_k)) throw index_out_of_bound();
	
	void *Node;
	int i;
	for (int i = 0; i < t->keyNum; ++i)
		if (x < t->key[i]) break;

	if (t->isLeaf == 0) Node = erase(_k, (idxNode *)t->child[i]);
	else Node = eraseData(_k, (dataNode *)t->child[i]);

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

template<class Key, class T>
Bplustree<Key, T>::dataNode
* Bplustree<Key, T>::eraseData(const Key &_k, dataNode *t) {
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

template<class Key, class T>
Bplustree<Key, T>::idxNode
* Bplustree<Key, T>::addData(dataNode *n, idxNode *t) {
	int min;
	if (t == root) min = 2;
	else min = miniDataNum;
	int i;
	for (i = 0; i < t->keyNum; i++)
		if (t->child[i] == n) break;
	dataNode *pre = t->child[i - 1];
	dataNode *next = t->child[i + 1];
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
* Bplustree<Key, T>::addIdx(idxNode *n, idxNode *t) {
	int min;
	if (t == root) min = 2;
	else min = miniDataNum;
	int i;
	for (int i = 0; i < t->keyNum; i++)
		if (t->child[i] == n) break;
	idxNode *pre = t->child[i - 1];
	idxNode *next = t->child[i + 1];
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