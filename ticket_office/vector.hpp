#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.h"
#include "sort.h"

#include <climits>
#include <cstddef>

template<typename T>
class vector {
private:
	void doubleSpace() {
		T *tmp = data;
		maxSize *= 2;
		data = (T*) ::operator new[](sizeof(T) * maxSize);
		for (size_t i = 0; i < currentLength; i++)
			new (data + i) T(tmp[i]);
		for (size_t i = 0; i < maxSize / 2; ++i)
			tmp[i].~T();
		::operator delete[](tmp);
	}
public:
	vector(): maxSize(10), currentLength(0) {
		data = (T*) ::operator new[](sizeof(T) * maxSize);
	}
	vector(size_t initSize) {
		data = (T*) ::operator new[](sizeof(T) * initSize);
		currentLength = initSize;
		maxSize = initSize;
	}
	vector(const vector &other) {
		maxSize = other.maxSize;
		data = (T*) ::operator new[](sizeof(T) * maxSize);
		currentLength = other.currentLength;
		for (size_t i = 0; i < currentLength; i++) {
			data[i] = other.data[i];
		}
	}

	~vector() {
		if (data)
		{
			for (size_t i = 0; i < maxSize; ++i)
				data[i].~T();
			::operator delete[](data);
		}
		currentLength = 0;
		maxSize = 0;
	}
	vector &operator=(const vector &other) {
		if (data) {
			for (size_t i = 0; i < maxSize; ++i)
				data[i].~T();
			::operator delete[](data);
			currentLength = 0;
			maxSize = 0;
		}
		//T *tmp = data;
		data = (T*) ::operator new[](sizeof(T) * other.currentLength);
		maxSize = other.maxSize;
		currentLength = other.currentLength;
		for (size_t i = 0; i < other.currentLength; i++) {
			data[i] = other.data[i];
		}
		/*for (int i = 0; i < maxSize / 2; ++i)
			tmp[i].~T();
		::operator delete[](tmp);*/
		return *this;
	}
	T & at(const size_t &pos) {
		if (pos >= currentLength || pos < 0) throw -1;
		else return data[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos >= currentLength || pos < 0) throw -1;
		else return data[pos];
	}
	T & operator[](const size_t &pos) {
		return data[pos];
	}
	const T & operator[](const size_t &pos) const {
		return data[pos];
	}
	const T & front() const {
		return data[0];
	}
	const T & back() const {
		return data[currentLength - 1];
	}
	void push_back(const T &value) {
		if (currentLength == maxSize) doubleSpace();
		new (data + currentLength) T(value);
		currentLength++;
	}
	void pop_back() {
		currentLength--;
	}
	void clear() {
		for (size_t i = 0; i < maxSize; ++i)
			data[i].~T();
		::operator delete[](data);
		maxSize = 10, currentLength = 0;
		data = (T*) ::operator new[](sizeof(T) * maxSize);
	}
	bool empty() const {
		return currentLength == 0;
	}
	size_t size() const {
		return currentLength;
	}
	size_t capacity() const {
		return maxSize;
	}
	class const_iterator;
	class iterator {
		friend class vector;
	private:
		T *pos;
	public:
		iterator(T *obj = NULL) { pos = obj; }
		iterator operator+(const size_t &n) const {
			iterator tmp;
			tmp.pos = pos + n;
			return tmp;
		}
		iterator operator-(const size_t &n) const {
			iterator tmp;
			tmp.pos = pos + n;
			return tmp;
		}
		size_t operator-(const iterator &rhs) const {
			return pos - rhs.pos;
		}
		iterator operator+=(const size_t &n) {
			pos += n;
			return *this;
		}
		iterator operator-=(const size_t &n) {
			pos -= n;
			return *this;
		}
		iterator operator++(int) {
			iterator tmp;
			tmp.pos = pos + 1;
			return tmp;
		}
		iterator& operator++() {
			++pos;
			return *this;
		}
		iterator operator--(int) {
			iterator tmp;
			tmp.pos = pos - 1;
			return tmp;
		}
		iterator& operator--() {
			--pos;
			return *this;
		}
		T& operator*() const{
			return *pos;
		}
		bool operator==(const iterator &rhs) const {
			return pos == rhs.pos;
		}
		bool operator==(const const_iterator &rhs) const {
			return pos == rhs.pos;
		}
		bool operator!=(const iterator &rhs) const {
			return pos != rhs.pos;
		}
		bool operator!=(const const_iterator &rhs) const {
			return pos != rhs.pos;
		}
	};

	class const_iterator {
	private:
		T * pos;
	public:
		const_iterator(T *obj = NULL) {
			pos = obj;
		}
		bool operator==(const iterator &rhs) const {
			return pos == rhs.pos;
		}
		bool operator==(const const_iterator &rhs) const {
			return pos == rhs.pos;
		}
		bool operator!=(const iterator &rhs) const {
			return pos != rhs.pos;
		}
		bool operator!=(const const_iterator &rhs) const {
			return pos != rhs.pos;
		}
		const_iterator operator++(int) const {
			iterator tmp;
			tmp.pos = pos + 1;
			return tmp;
		}
		const_iterator& operator++() {
			++pos;
			return *this;
		}
		const_iterator operator--(int) {
			iterator tmp;
			tmp.pos = pos - 1;
			return tmp;
		}
		const_iterator& operator--() {
			--pos;
			return *this;
		}
		T& operator*() {
			return *pos;
		}
	};
	
	
	iterator begin() {
		return iterator(data);
	}
	const_iterator cbegin() const {
		return const_iterator(data);
	}
	iterator end() {
		return iterator(data + currentLength);
	}
	const_iterator cend() const {
		return const_iterator(data + currentLength);
	}
	
	
	iterator insert(iterator p, const T &value) {
		T *q;
		if (currentLength == maxSize)
		{
			size_t offset = p.pos - data;
			doubleSpace();
			p.pos = data + offset;
		}
		q = data + currentLength;
		while (q > p.pos) {
			*q = *(q - 1);
			--q;
		}

		*p.pos = value;
		currentLength++;
		return p;
	}
	iterator erase(iterator p) {
		size_t ind = p - data;
		for (size_t i = ind; i < currentLength; i++)
			data[i] = data[i + 1];
		currentLength--;
		return iterator(data + ind);
	}
	iterator erase(const size_t &ind) {
		for (size_t i = ind; i < currentLength; i++)
			data[i] = data[i + 1];
		currentLength--;
		return iterator(data + ind);
	}
	void sort() {
		::sort(data, 0, currentLength - 1);
	}
	void sort(bool (*cmp)(const T&, const T&)) {
		::sort(data, 0, currentLength - 1, cmp);
	}
private:
	T *data;
	size_t maxSize;
	size_t currentLength;
};

#endif
