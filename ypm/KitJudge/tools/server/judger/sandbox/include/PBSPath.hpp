#ifndef PBS_PATH_HPP
#define PBS_PATH_HPP

#include "PBSBase.hpp"

std::string getCWDPath(pid_t pid) {
	char s[20];
	char cwd[MAX_PATH_LENGTH + 1];
	if (pid != 0) {
		sprintf(s, "/proc/%lld/cwd", (long long int)pid);
	} else {
		sprintf(s, "/proc/self/cwd");
	}
	int l = readlink(s, cwd, MAX_PATH_LENGTH);
	if (l == -1) {
		return "";
	}
	cwd[l] = '\0';
	return cwd;
}

std::string getAbsPath(pid_t pid, const std::string &path) {
	if (path.size() > MAX_PATH_LENGTH) {
		return "";
	}
	if (path.empty()) {
		return path;
	}
	std::string s;
	std::string b;
	size_t st;
	if (path[0] == '/') {
		s = "/";
		st = 1;
	} else {
		s = getCWDPath(pid) + "/";
		st = 0;
	}
	for (size_t i = st; i < path.size(); i++) {
		b += path[i];
		if (path[i] == '/') {
			if (b == "../" && !s.empty()) {
				if (s == "./") {
					s = "../";
				} else if (s != "/") {
					size_t p = s.size() - 1;
					while (p > 0 && s[p - 1] != '/') {
						p--;
					}
					if (s.size() - p == 3 && s[p] == '.' && s[p + 1] == '.' && s[p + 2] == '/') {
						s += b;
					} else {
						s.resize(p);
					}
				}
			} else if (b != "./" && b != "/") {
				s += b;
			}
			b.clear();
		}
	}
	if (b == ".." && !s.empty()) {
		if (s == "./") {
			s = "..";
		} else if (s != "/") {
			size_t p = s.size() - 1;
			while (p > 0 && s[p - 1] != '/') {
				p--;
			}
			if (s.size() - p == 3 && s[p] == '.' && s[p + 1] == '.' && s[p + 2] == '/') {
				s += b;
			} else {
				s.resize(p);
			}
		}
	} else if (b != ".") {
		s += b;
	}
	if (s.size() >= 2 && s[s.size() - 1] == '/') {
		s.resize(s.size() - 1);
	}
	return s;
}

std::string getRealPath(const std::string &path) {
	char real[MAX_PATH_LENGTH + 1] = {};
	if (realpath(path.c_str(), real) == NULL) {
		return "";
	}
	return real;
}

std::string getStringFromRegs(reg_val_t addr, pid_t pid) {
	char res[MAX_PATH_LENGTH + 1], *ptr = res;
	while (ptr != res + MAX_PATH_LENGTH) {
		*(reg_val_t*)ptr = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
		for (int i = 0; i < sizeof(reg_val_t); i++, ptr++, addr++) {
			if (*ptr == 0) {
				return res;
			}
		}
	}
	res[MAX_PATH_LENGTH] = 0;
	return res;
}

std::string getAbsPathFromRegAddress(const reg_val_t &addr, const pid_t &pid) {
	return getAbsPath(pid, getStringFromRegs(addr, pid));
}

std::string getBasePath(const std::string &abspath) {
	std::string ret;
	for (int i = (int)abspath.length() - 1; i >= 0; i--) {
		if (abspath[i] == '/') break;
		ret = ret + abspath[i];
	}
	std::reverse(ret.begin(), ret.end());
	return ret;
}

std::string getDirectory(const std::string &abspath) {
	std::string tmp = getBasePath(abspath);
	return abspath.substr(0, abspath.length() - tmp.length());
}

// ``/home'' is only accept the directory(or file) ``/home''
// ``/home/'' can accept ``/home/1.txt'' but reject ``/home/2/1.txt''
// ``/home/*'' can accept ``/home/2/1.txt''
// ``/home/*'' contains ``/home/'' contains ``/home''
// ATTENTION: Be sure that path is not ended with '/' if it is a directory and it is a absolute path.
bool smartStringCounter(const std::string &path, const std::set<std::string> &set) {
	if (set.count(path)) {
		return true;
	}
	if (set.count(path + "/")) {
		return true;
	}
	if (set.count(path + "/*")) {
		return true;
	}
	for (int i = 0; i + 1 < (int)path.length(); i++) { // Reject the non-absolute paths.
		if (path[i] == '.' && path[i + 1] == '/') {
			return false;
		}
		if (i + 1 < (int)path.length() && path[i] == '.' && path[i + 1] == '.' && path[i + 2] == '/') {
			return false;
		}
	}
	for (int i = (int)path.length(); i >= 0; i--) {
		if (path[i] == '/') {
			if (set.count(path.substr(0, i) + "/*")) {
				return true;
			}
			if (set.count(path.substr(0, i) + "/")) {
				return true;
			}
			if (set.count("*" + path.substr(i, (int)path.size() - i + 1))) {
				//std::cerr << "--------------" << path << std::endl;
				return true;
			}
		}
	}
	return false;
}

bool verdictFileInSet(const std::string &filename, const std::set<std::string> &set) {
	if (smartStringCounter(filename, set)) {
		return true;
	} else if (smartStringCounter(getRealPath(filename), set)) {
		return true;
	}
	return false;
}

#endif
