#ifndef _TIMER_H_
#define _TIMER_H_

#include <ctime>
#include <iostream>

struct timer
{
	std::string var;
	int st, ed;
	void begin() {
		st = clock();
	}
	void stop() {
		ed = clock();
	}
	void print() {
		std::cerr << "Time use of " << var << " is ";
		std::cerr << 1.0 * (ed - st) / CLOCKS_PER_SEC << "s" << std::endl;
	}
	void end() {
		stop();
		print();
	}
	timer(const std::string &s = "A"): var(s) {
		begin();
	}
	~timer() {
		end();
	}
};

#endif // _TIME_H_
