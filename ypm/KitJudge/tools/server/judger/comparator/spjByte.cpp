#include <cstdio>
#include <cstring>
#include <algorithm>
#include "spjbase.hpp"
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
	try{
		ifstream indat(argv[1]);
		ifstream instd(argv[2]);
		ifstream insrc(argv[3]);
		if (!indat) throw SPJException("Cannot open the input data.", SPJ_SRC_IO_ERROR);
		if (!instd) throw SPJException("Cannot open the stdfile.", SPJ_STD_IO_ERROR);
		if (!insrc) throw SPJException("Cannot open the srcfile.", SPJ_SRC_IO_ERROR);
		int size = 0;
		while (true) {
			string stdans, srcans;
			if (getline(instd, stdans))
			{
				if (getline(insrc, srcans))
				{
					size++;
					if (stdans != srcans) 
					{
						cout << "Line " << size << "\n";
						cout << "Read \"" << srcans << "\"\n";
						cout << "Expect \"" << stdans << "\"\n";
						instd.close();
						insrc.close();
						indat.close();
						putScore(argc, argv, 0.0, SPJ_SCR_IO_ERROR);
						return SPJ_WA;
					}
				}
				else 
				{
					cout << "The stdfile's size differs from srcfile.";
					putScore(argc, argv, 0.0, SPJ_SCR_IO_ERROR);
					instd.close();
					insrc.close();
					indat.close();
					return SPJ_WA;
				}
			}
			else
			{
				if (getline(insrc, srcans))
				{
					cout << "The stdfile's size differs from srcfile.";
					putScore(argc, argv, 0.0, SPJ_SCR_IO_ERROR);
					instd.close();
					insrc.close();
					indat.close();
					return SPJ_WA;
				}
				else break;
			}
		}
		printf("OK, %d lines tokens.\n", size);
		instd.close();
		insrc.close();
		indat.close();
		putScore(argc, argv, getScore(argc, argv, SPJ_SCR_IO_ERROR), SPJ_SCR_IO_ERROR);
		return SPJ_OK;
	} catch(SPJException e) {
		e.print();
		return e.exitcode;
	}
	return 0;
}
