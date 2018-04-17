#ifndef _Bplus
#define _Bplus

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
using namespace std;
//B+Ê÷

template <class Key, class T>
class Bplustree {
private:
	struct Node{};
	Node *root;
	int nowsize;

public:
	typedef pair<const Key, T> value_type;

	class const_iterator;
	class iterator {
	public:
		iterator() {};
		iterator(const iterator &other) {};
		iterator operator++(int) {};
		iterator & operator++() {};
		iterator operator--(int) {};
		iterator & operator--() {};
		value_type & operator*() const {};
		bool operator==(const iterator &rhs) const {};
		bool operator==(const const_iterator &rhs) const {};
		bool operator!=(const iterator &rhs) const {};
		bool operator!=(const const_iterator &rhs) const {};
		value_type* operator->() const noexcept {};
	};

	class const_iterator {};

	Bplustree() {};
	Bplustree(const Bplustree &other) {};
	Bplustree & operator=(const Bplustree &other) {};
	~Bplustree() {};

	T & at(const Key &key) {};
	const T & at(const Key &key) const {};

	T & operator[](const Key &key) {};
	const T & operator[](const Key &key) const {};

	iterator begin() {};
	const_iterator cbegin() const {};
	iterator end() {};
	const_iterator cend() const {};

	bool empty() const {};

	size_t size() const {};

	void clear() {};

	pair<iterator, bool> insert(const value_type &pvalue) {};

	void erase(iterator it) {};

	size_t count(const Key &key) const {};

	iterator find(const Key &key) {};

	const_iterator find(const Key &key) const {};
};
#endif
