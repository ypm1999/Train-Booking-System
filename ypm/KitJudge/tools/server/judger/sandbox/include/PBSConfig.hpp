#ifndef PBS_CONFIG_HPP
#define PBS_CONFIG_HPP

#include "PBSPath.hpp"
#include "PBSBase.hpp"
#include "PBSException.hpp"

struct PBSConfig{
	long timeLimit;
	long memoryLimit;
	long outputLimit;
	long stackLimit;

	std::string type;
	std::string program;
	std::string workPath;
	std::string inputPage;
	std::string outputPage;
	std::string errorPage;

	std::vector<std::string> argv;
	std::set<std::string> readableFiles;
	std::set<std::string> writableFiles;
	std::set<std::string> statableFiles;
	std::set<std::string> unacsbleFiles;

#if defined(DEBUG1) || defined(DEBUG2)
	bool debugMode;
#endif
	bool simpleMode;
	bool safeMode;

	bool softBanSyscall[MAX_SYSCALL];
	int syscallCounter[MAX_SYSCALL];
};

extern PBSConfig pbsConfig;

void pbsCPPCompilerConfig() {
	pbsConfig.syscallCounter[__NR_getpid  ] = -1;
	pbsConfig.syscallCounter[__NR_vfork   ] = -1;
	pbsConfig.syscallCounter[__NR_execve  ] = -1;
	pbsConfig.syscallCounter[__NR_wait4   ] = -1;
	pbsConfig.syscallCounter[__NR_sysinfo ] = -1;
	pbsConfig.syscallCounter[__NR_umask   ] = -1;
	pbsConfig.syscallCounter[__NR_chmod   ] = -1;
	pbsConfig.readableFiles.insert("*/bin/as");
	pbsConfig.statableFiles.insert("/*");
	pbsConfig.readableFiles.insert("/usr/*");
	pbsConfig.writableFiles.insert("/tmp/*");
}

void pbsMakefileConfig() {
	pbsConfig.syscallCounter[__NR_getpid  ] = -1;
	pbsConfig.syscallCounter[__NR_vfork   ] = -1;
	pbsConfig.syscallCounter[__NR_execve  ] = -1;
	pbsConfig.syscallCounter[__NR_wait4   ] = -1;
	pbsConfig.syscallCounter[__NR_sysinfo ] = -1;
	pbsConfig.syscallCounter[__NR_umask   ] = -1;
	pbsConfig.syscallCounter[__NR_chmod   ] = -1;
	pbsConfig.readableFiles.insert("*/bin/as");
	pbsConfig.statableFiles.insert("/*");
	pbsConfig.readableFiles.insert("/usr/*");
	pbsConfig.writableFiles.insert("/tmp/*");
	pbsConfig.writableFiles.insert("/lib/*");
	pbsConfig.writableFiles.insert("/usr/lib/*");
	pbsConfig.writableFiles.insert("/etc/*");
}

void pbsJAVACompilerConfig() {
	pbsConfig.syscallCounter[__NR_set_tid_address  ] = -1;
	pbsConfig.syscallCounter[__NR_set_robust_list  ] = -1;
	pbsConfig.syscallCounter[__NR_futex            ] = -1;
	pbsConfig.syscallCounter[__NR_clone            ] = -1;
	pbsConfig.syscallCounter[__NR_getdents         ] = -1;
	pbsConfig.syscallCounter[__NR_sysinfo          ] = -1;
	pbsConfig.syscallCounter[__NR_clock_getres     ] = -1;
	pbsConfig.syscallCounter[__NR_setrlimit        ] = -1;
	pbsConfig.syscallCounter[__NR_geteuid          ] = -1;
	pbsConfig.syscallCounter[__NR_socket           ] = -1;
	pbsConfig.syscallCounter[__NR_connect          ] = -1;
	pbsConfig.syscallCounter[__NR_fchdir           ] = -1;
	pbsConfig.syscallCounter[__NR_mkdir            ] = -1;
	pbsConfig.syscallCounter[__NR_ftruncate        ] = -1;
	pbsConfig.syscallCounter[__NR_kill             ] = -1;
	pbsConfig.syscallCounter[__NR_sched_getaffinity] = -1;
	pbsConfig.syscallCounter[__NR_gettid           ] = -1;
	pbsConfig.syscallCounter[__NR_uname            ] = -1;
	pbsConfig.syscallCounter[__NR_getuid           ] = -1;
	pbsConfig.syscallCounter[__NR_sched_yield      ] = -1;
	pbsConfig.statableFiles.insert("/*");
	pbsConfig.readableFiles.insert("/usr/lib/jvm/*");
	pbsConfig.readableFiles.insert("/sys/devices/system/cpu/*");
	pbsConfig.readableFiles.insert("/proc/*");
	pbsConfig.readableFiles.insert("/usr/*");
	pbsConfig.unacsbleFiles.insert("/etc/nsswitch.conf");
	pbsConfig.unacsbleFiles.insert("/etc/passwd");
}

void pbsValgrindConfig() {
	pbsConfig.syscallCounter[__NR_execve           ] = 1;
	pbsConfig.syscallCounter[__NR_setrlimit        ] = 2;
	pbsConfig.syscallCounter[__NR_getxattr         ] = 1;
	pbsConfig.syscallCounter[__NR_geteuid          ] = -1;
	pbsConfig.syscallCounter[__NR_getegid          ] = -1;
	pbsConfig.syscallCounter[__NR_pread64          ] = -1;
	pbsConfig.syscallCounter[__NR_pipe             ] = 1;
	pbsConfig.syscallCounter[__NR_gettid           ] = -1;
	pbsConfig.syscallCounter[__NR_getitimer        ] = -1;
	pbsConfig.syscallCounter[__NR_execve           ] = 2;
	pbsConfig.syscallCounter[__NR_getgroups        ] = -1;
	pbsConfig.syscallCounter[__NR_getppid          ] = -1;
	pbsConfig.syscallCounter[__NR_prctl            ] = 1;
	pbsConfig.syscallCounter[__NR_mknod            ] = 2;
	pbsConfig.syscallCounter[__NR_uname            ] = 1;
	pbsConfig.syscallCounter[__NR_gettid           ] = -1;
	pbsConfig.syscallCounter[__NR_getuid           ] = -1;
	pbsConfig.syscallCounter[__NR_getgid           ] = -1;
	pbsConfig.syscallCounter[__NR_getpid           ] = -1;
	pbsConfig.syscallCounter[__NR_kill             ] = 1;
	pbsConfig.syscallCounter[__NR_rt_sigtimedwait  ] = -1;
	pbsConfig.readableFiles.insert("/usr/lib/valgrind/*");
	pbsConfig.readableFiles.insert("/usr/bin/*");
	pbsConfig.readableFiles.insert("/proc/*");
	pbsConfig.readableFiles.insert("/usr/lib/debug/*");
	pbsConfig.writableFiles.insert("/tmp/*");
}

void pbsJAVARunnerConfig() {
	pbsConfig.syscallCounter[__NR_set_tid_address  ] = -1;
	pbsConfig.syscallCounter[__NR_set_robust_list  ] = -1;
	pbsConfig.syscallCounter[__NR_futex            ] = -1;
	pbsConfig.syscallCounter[__NR_clone            ] = -1;
	pbsConfig.syscallCounter[__NR_getdents         ] = -1;
	pbsConfig.syscallCounter[__NR_sysinfo          ] = -1;
	pbsConfig.syscallCounter[__NR_clock_getres     ] = -1;
	pbsConfig.syscallCounter[__NR_setrlimit        ] = -1;
	pbsConfig.syscallCounter[__NR_geteuid          ] = -1;
	pbsConfig.syscallCounter[__NR_socket           ] = -1;
	pbsConfig.syscallCounter[__NR_connect          ] = -1;
	pbsConfig.syscallCounter[__NR_fchdir           ] = -1;
	pbsConfig.syscallCounter[__NR_mkdir            ] = -1;
	pbsConfig.syscallCounter[__NR_ftruncate        ] = -1;
	pbsConfig.syscallCounter[__NR_kill             ] = -1;
	pbsConfig.syscallCounter[__NR_sched_getaffinity] = -1;
	pbsConfig.syscallCounter[__NR_gettid           ] = -1;
	pbsConfig.syscallCounter[__NR_uname            ] = -1;
	pbsConfig.syscallCounter[__NR_getuid           ] = -1;
	pbsConfig.syscallCounter[__NR_sched_yield      ] = -1;
	pbsConfig.syscallCounter[__NR_prctl            ] = -1;
	pbsConfig.syscallCounter[__NR_pread64          ] = -1;
	pbsConfig.syscallCounter[__NR_nanosleep        ] = -1;
	pbsConfig.readableFiles.insert("/usr/lib/jvm/java-8-openjdk-amd64/*");
	pbsConfig.readableFiles.insert("/proc/*");
	pbsConfig.unacsbleFiles.insert("/sys/devices/system/cpu/*");
	pbsConfig.unacsbleFiles.insert("/etc/passwd");
	pbsConfig.unacsbleFiles.insert("/etc/nsswitch.conf");
	pbsConfig.statableFiles.insert("/usr/");
	pbsConfig.statableFiles.insert("/var/");
	pbsConfig.writableFiles.insert("/tmp/");
}

void initCommonPBSConfig(PBSConfig &pbsConfig) {
	pbsConfig.timeLimit = PBSDEFAULT_TIMELIMIT;
	pbsConfig.memoryLimit = PBSDEFAULT_MEMORYLIMIT;
	pbsConfig.outputLimit = PBSDEFAULT_OUTPUTLIMIT;
	pbsConfig.stackLimit = PBSDEFAULT_STACKLIMIT;
	pbsConfig.inputPage = "stdin";
	pbsConfig.outputPage = "stdout";
	pbsConfig.errorPage = "stderr";
	pbsConfig.type = "default";
	pbsConfig.workPath = "";
	pbsConfig.safeMode = true;
	pbsConfig.simpleMode = true;
#if defined(DEBUG1) || defined(DEBUG2)
	pbsConfig.debugMode = false;
#endif
	pbsConfig.argv.clear();
}

void initParentPBSConfig(PBSConfig &pbsConfig) {
	pbsConfig.readableFiles.clear();
	pbsConfig.writableFiles.clear();
	pbsConfig.statableFiles.clear();
	pbsConfig.unacsbleFiles.clear();

	for (int i = 0; READABLE_FILENAME_LIST_DEFAULT[i]; i++) {
		pbsConfig.readableFiles.insert(READABLE_FILENAME_LIST_DEFAULT[i]);
	}
	for (int i = 0; WRITEABLE_FILENAME_LIST_DEFAULT[i]; i++) {
		pbsConfig.writableFiles.insert(WRITEABLE_FILENAME_LIST_DEFAULT[i]);
	}
	pbsConfig.writableFiles.insert(pbsConfig.workPath);
	memset(pbsConfig.syscallCounter, 0, sizeof(pbsConfig.syscallCounter));
	memset(pbsConfig.softBanSyscall, 0, sizeof(pbsConfig.softBanSyscall));
	for (int i = 0; SYSCALL_MAXIMUM_COUNT_DEFAULT_LIST[i][0] != -1; i++) {
		pbsConfig.syscallCounter[SYSCALL_MAXIMUM_COUNT_DEFAULT_LIST[i][0]] = SYSCALL_MAXIMUM_COUNT_DEFAULT_LIST[i][1];
	}
	if (pbsConfig.type == "compiler-c++") {
		pbsCPPCompilerConfig();
	} else if (pbsConfig.type == "compiler-java") {
		pbsJAVACompilerConfig();
	} else if (pbsConfig.type == "runner-java") {
		pbsJAVARunnerConfig();
	} else if (pbsConfig.type == "valgrind") {
		pbsValgrindConfig();
	} else if (pbsConfig.type == "makefile") {
		pbsMakefileConfig();
	}
}
#ifdef DEBUG1
void pbsPrintConfig() {
	flog("Time limit = %ld ms\n", pbsConfig.timeLimit);
	flog("Memory limit = %ld KB\n", pbsConfig.memoryLimit);
	flog("Stack limit = %ld KB\n", pbsConfig.stackLimit);
	flog("Output limit = %ld KB\n", pbsConfig.outputLimit);
	flog("WorkPath = %s\n", pbsConfig.workPath.c_str());
	flog("Readable Files\n");
	for (auto x : pbsConfig.readableFiles) {
		flog("%s\n", x.c_str());
	}
	flog("Writable Files\n");
	for (auto x : pbsConfig.writableFiles) {
		flog("%s\n", x.c_str());
	}
}
#endif

#endif
