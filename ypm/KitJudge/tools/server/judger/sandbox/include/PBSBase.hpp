#ifndef PBS_BASE_HPP
#define PBS_BASE_HPP

#include <cstdio>
#include <set>
#include <fcntl.h>
#include <cstdarg>
#include <asm/unistd.h>
#include <sys/ptrace.h>
#include <sys/time.h>
#include <sys/unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/reg.h>
#include <sys/resource.h>
#include <sys/user.h>
#include <unistd.h>
#include <argp.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>

#define SHM_FAIL_CREATE_EXITCODE 253
#define SHM_FAIL_LINK_EXITCODE 254
#define SHM_FAIL_UNLINK_EXITCODE 255
#define SHM_FATAL_ERROR_EXITCODE 252
#define SHM_KEY 1234

#define MAX_LOG_BUFFER 100000

#ifdef __x86_64__
	typedef unsigned long long int reg_val_t;
	#define REG_SYSCALL orig_rax
	#define REG_RET rax
	#define REG_ARG0 rdi
	#define REG_ARG1 rsi
	#define REG_ARG2 rdx
	#define REG_ARG3 rcx
#else
	typedef long int reg_val_t;
	#define REG_SYSCALL orig_eax
	#define REG_RET eax
	#define REG_ARG0 ebx
	#define REG_ARG1 ecx
	#define REG_ARG2 edx
	#define REG_ARG3 esx
#endif

const int MAX_PROC = 1000;
const int MAX_SYSCALL = 1000; // *** PAY ATTENTION: BE CAREFURUL TO MODIFY THIS CONSTANT.
const size_t MAX_PATH_LENGTH = 200;
const size_t MAX_EXCEPTION_MESSAGE_LENGTH = 2000;

const int PBSDEFAULT_TIMELIMIT = 2000; // in microseconds
const int PBSDEFAULT_MEMORYLIMIT = 256 * 1024; // 256 MB
const int PBSDEFAULT_OUTPUTLIMIT = 5 * 1024 * 1024;  // 1 MB
const int PBSDEFAULT_STACKLIMIT = 1024 * 1024; // 1 GB

const char *READABLE_FILENAME_LIST_DEFAULT[] = {
	"/etc/ld.so.nohwcap",
	"/etc/ld.so.preload",
	"/etc/ld.so.cache",
	"/lib/x86_64-linux-gnu/*",
	"/usr/lib/x86_64-linux-gnu/*",
	"/usr/lib/locale/locale-archive",
	"/proc/self/exe",
	"/etc/timezone",
	"/usr/share/zoneinfo/*",
	"/dev/random",
	"/dev/urandom",
	"/proc/meminfo",
	"/etc/localtime",
	NULL
};

const char *WRITEABLE_FILENAME_LIST_DEFAULT[] = {
	"/dev/null",
	NULL
};

const int SYSCALL_MAXIMUM_COUNT_DEFAULT_LIST[][2] = {
	{__NR_read          , -1},
	{__NR_write         , -1},
	{__NR_readv         , -1},
	{__NR_writev        , -1},
	{__NR_open          , -1},
	{__NR_unlink        , -1},
	{__NR_close         , -1},
	{__NR_readlink      , -1},
	{__NR_openat        , -1},
	{__NR_unlinkat      , -1},
	{__NR_readlinkat    , -1},
	{__NR_stat          , -1},
	{__NR_fstat         , -1},
	{__NR_lstat         , -1},
	{__NR_lseek         , -1},
	{__NR_access        , -1},
	{__NR_dup           , -1},
	{__NR_dup2          , -1},
	{__NR_dup3          , -1},
	{__NR_ioctl         , -1},
	{__NR_fcntl         , -1},

	{__NR_mmap          , -1},
	{__NR_mprotect      , -1},
	{__NR_munmap        , -1},
	{__NR_brk           , -1},
	{__NR_mremap        , -1},
	{__NR_msync         , -1},
	{__NR_mincore       , -1},
	{__NR_madvise       , -1},

	{__NR_rt_sigaction  , -1},
	{__NR_rt_sigprocmask, -1},
	{__NR_rt_sigreturn  , -1},
	{__NR_rt_sigpending , -1},
	{__NR_sigaltstack   , -1},

	{__NR_getcwd        , -1},

	{__NR_exit          , -1},
	{__NR_exit_group    , -1},

	{__NR_arch_prctl    , -1},

	{__NR_gettimeofday  , -1},
	{__NR_getrlimit     , -1},
	{__NR_getrusage     , -1},
	{__NR_times         , -1},
	{__NR_time          , -1},
	{__NR_clock_gettime , -1},

	{__NR_restart_syscall, -1},

	{-1                 , -1}
};

#endif
