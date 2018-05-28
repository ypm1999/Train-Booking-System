#ifndef PBS_EXECUTE_HPP
#define PBS_EXECUTE_HPP

#include "PBSBase.hpp"
#include "PBSException.hpp"
#include "PBSControl.hpp"
#include "PBSConfig.hpp"
#include "PBSDebug.hpp"

int getSecondByMicrosecond(int microsecond) {
	return (microsecond + 999) / 1000;
}

// TODO Bugs to be fixed:
// I must make a communication between the parent process and the subprocess rather than exceptions,
// which may show the error message "The subprocess is ended before traced."
void pbsExecute() {
	pbsSetResourceLimit(RLIMIT_CPU, getSecondByMicrosecond(pbsConfig.timeLimit), getSecondByMicrosecond(pbsConfig.timeLimit) + 2);
	pbsSetResourceLimit(RLIMIT_FSIZE, 1ll * pbsConfig.outputLimit << 10);
	pbsSetResourceLimit(RLIMIT_STACK, 1ll * pbsConfig.stackLimit << 10);

	char *env_path_str = getenv("PATH");
	char *env_lang_str = getenv("LANG");
	char *env_shell_str = getenv("SHELL");
	std::string env_path = env_path_str ? env_path_str : "";
	std::string env_lang = env_lang_str ? env_lang_str : "";
	std::string env_shell = env_shell_str ? env_shell_str : "";

	clearenv();
	setenv("USER", "poor_program", 1);
	setenv("LOGNAME", "poor_program", 1);
	setenv("HOME", pbsConfig.workPath.c_str(), 1);
	if (env_lang_str) {
		setenv("LANG", env_lang.c_str(), 1);
	}
	if (env_path_str) {
		setenv("PATH", env_path.c_str(), 1);
	}
	setenv("PWD", pbsConfig.workPath.c_str(), 1);
	if (env_shell_str) {
		setenv("SHELL", env_shell.c_str(), 1);
	}

	if (pbsConfig.inputPage != "stdin") {
		if (freopen(pbsConfig.inputPage.c_str(), "r", stdin) == NULL) {
			throw PBSException("Cannot open the input page \"" + pbsConfig.inputPage + "\".");
		}
	}
	if (pbsConfig.outputPage != "stdout" && pbsConfig.outputPage != "stderr") {
		if (freopen(pbsConfig.outputPage.c_str(), "w", stdout) == NULL) {
			throw PBSException("Cannot open the output page \"" + pbsConfig.outputPage + "\".");
		}
	}
	if (pbsConfig.errorPage != "stderr") {
		if (pbsConfig.errorPage == "stdout") {
			if (dup2(1, 2) == -1) {
				throw PBSException("Failed to Duplicate STDOUT to STDERR.");
			}
		} else {
			if (freopen(pbsConfig.errorPage.c_str(), "w", stderr) == NULL) {
				throw PBSException("Cannot open the error page \"" + pbsConfig.errorPage + "\".");
			}
		}

		if (pbsConfig.outputPage == "stderr") {
			if (dup2(2, 1) == -1) {
				throw PBSException("Failed to Duplicate STDERR to STDOUT.");
			}
		}
	}
	char **programArgv = new char*[(int)pbsConfig.argv.size() + 1];
	for (size_t i = 0; i < pbsConfig.argv.size(); i++) {
		programArgv[i] = new char[(int)pbsConfig.argv[i].size() + 1];
		strcpy(programArgv[i], pbsConfig.argv[i].c_str());
	}
	programArgv[pbsConfig.argv.size()] = NULL;
	if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
		throw PBSException("Failed to settle the executor process traced.");
	}
	if (execv(programArgv[0], programArgv) == -1) {
		throw PBSException("Failed to execute the program.");
	}
}

#endif
