#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include "statement.h"
#include "exceptions.h"
using namespace std;

int main()
{
	Statement st[1000];
	int size = 0;
	ifstream in;
	in.open("file");
	if (!in) throw open_file_error();
	in.read(reinterpret_cast<char *>(st), sizeof(Statement));
	size++;
	while(!in.eof()) {
		in.read(reinterpret_cast<char *>(st + size), sizeof(Statement));
		size++;
	}
	for (int i = 0; i < size; i++)
		st[i].execute();
	return 0;
}