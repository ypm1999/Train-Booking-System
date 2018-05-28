#include <cstdio>
#include <cstring>
#include <algorithm>
#include "spjbase.hpp"

int main(int argc, char *argv[]) {	
	try{
		FILE *indat = fopen(argv[1], "r");
		FILE *instd = fopen(argv[2], "r");
		FILE *insrc = fopen(argv[3], "r");
		if (instd == NULL) {
			throw SPJException("Cannot open the stdfile.", SPJ_STD_IO_ERROR);
		}
		if (insrc == NULL) {
			throw SPJException("Cannot open the srcfile.", SPJ_SRC_IO_ERROR);
		}
		int size = 0;
		while (true) {
			long long stdans, srcans;
			int stdres = fscanf(instd, "%lld", &stdans);
			int srcres = fscanf(insrc, "%lld", &srcans);
			if (stdres < 0 && srcres < 0) {
				break;
			}
			if ((stdres == 1) ^ (stdres == 1)) {
				printf("Different number of integers.\n");
				fclose(instd);
				fclose(insrc);
				return SPJ_WA;
			}
			size++;
			if (stdans != srcans) {
				if (size % 20 == 1) {
					printf("Read %lld, but expects %lld at the %d-st token.\n", srcans, stdans, size);
				} else if (size % 20 == 2) {
					printf("Read %lld, but expects %lld at the %d-nd token.\n", srcans, stdans, size);
				} else {
					printf("Read %lld, but expects %lld at the %d-th token.\n", srcans, stdans, size);
				}
				fclose(instd);
				fclose(insrc);
				putScore(argc, argv, 0.0, SPJ_SCR_IO_ERROR);
				return SPJ_WA;
			}
		}
		printf("OK, %d tokens.\n", size);
		fclose(instd);
		fclose(insrc);
		fclose(indat);
		putScore(argc, argv, getScore(argc, argv, SPJ_SCR_IO_ERROR), SPJ_SCR_IO_ERROR);
		return SPJ_OK;
	} catch(SPJException e) {
		e.print();
		return e.exitcode;
	}
	return 0;
}
