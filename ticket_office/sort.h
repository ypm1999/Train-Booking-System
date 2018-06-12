#ifndef _SORT_H_
#define _SORT_H_

template<class T>
inline void swap(T &x, T &y) {
	T t = x; x = y; y = t;
}

template<class T>
inline T min(const T &a, const T &b) {
	if (a < b) return a;
	return b;
}

template<class T>
int partition(T a[], int l, int r)
{
	swap(a[l], a[(l + r) / 2]);
	T pivot = a[l];
	int i = l;
	for (int j = l + 1; j <= r; j++)
		if (a[j] < pivot)
			swap(a[++i], a[j]);
	swap(a[l], a[i]);
	return i;
}

template<class T>
void sort(T a[], int l, int r)
{
	if (l >= r) return;
	int mid = partition(a, l, r);
	sort(a, l, mid - 1);
	sort(a, mid + 1, r);
}

template<class T>
int partition(T a[], int l, int r, bool (*cmp)(const T&, const T&))
{
	swap(a[l], a[(l + r) / 2]);
	T pivot = a[l];
	int i = l;
	for (int j = l + 1; j <= r; j++)
		if (cmp(a[j], pivot))
			swap(a[++i], a[j]);
	swap(a[l], a[i]);
	return i;
}

template<class T>
void sort(T a[], int l, int r, bool (*cmp)(const T&, const T&))
{
	if (l >= r) return;
	int mid = partition(a, l, r, cmp);
	sort(a, l, mid - 1, cmp);
	sort(a, mid + 1, r, cmp);
}
#endif // _SORT_H_
