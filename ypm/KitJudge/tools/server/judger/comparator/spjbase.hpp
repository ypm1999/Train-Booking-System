#ifndef SPJ_BASE
#define SPJ_BASE

#include <string>
#include <iostream>

#define SPJ_OK 0
#define SPJ_WA 1
#define SPJ_STD_IO_ERROR 2
#define SPJ_SRC_IO_ERROR 3
#define SPJ_SCR_IO_ERROR 4

struct SPJException{
	std::string message;
	int exitcode;
	SPJException() {}
	SPJException(const std::string &message, int exitcode) : message(message), exitcode(exitcode) {}
	void print() {
		std::cout << message << std::endl;
	}
};

long getFileSize(FILE *stream, int exitcode) {
	long curpos, ret;
	if ((curpos = ftell(stream)) < 0) {
		throw SPJException("Failed to read the file.", exitcode);
	}
	if (fseek(stream, 0L, SEEK_END) < 0) {
		throw SPJException("Failed to read the file.", exitcode);
	}
	if ((ret = ftell(stream)) < 0) {
		throw SPJException("Failed to read the file.", exitcode);
	}
	if (fseek(stream, 0L, curpos) < 0) {
		throw SPJException("Failed to read the file.", exitcode);
	}
	return ret;
}

double getScore(int argc, char **argv, int exitcode)  {
	if (argc == 6) {
		FILE *inscr = fopen(argv[4], "r");
		if (!inscr) {
			throw SPJException("Failed to read the score file.", exitcode);
		}
		double buffer;
		fscanf(inscr, "%lf", &buffer);
		fclose(inscr);
		return buffer;
	}
	return 0;
}

void putScore(int argc, char **argv, double score, int exitcode) {
	if (argc == 6) {
		FILE *ouscr = fopen(argv[5], "w");
		if (!ouscr) {
			throw SPJException("Failed to write the score file.", exitcode);
		}
		fprintf(ouscr, "%.20f", score);
		fclose(ouscr);
	}
}

#endif
