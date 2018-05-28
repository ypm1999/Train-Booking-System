#ifndef PBS_CONTROL_HPP
#define PBS_CONTROL_HPP

#include "PBSException.hpp"
#include "PBSDebug.hpp"
#include "PBSConfig.hpp"

void pbsSetResourceLimit(int resourceId, long long currentResource, long long maximumResource = -1) {
	if (maximumResource == -1) maximumResource = currentResource;
#ifdef DEBUG1
	if (pbsConfig.debugMode) {
		slog("Resource(id = %d) was settled to be current = %lld, maximum = %lld\n", resourceId, currentResource, maximumResource);
	}
#endif
	struct rlimit nowResourceLimit;
	if (getrlimit(resourceId, &nowResourceLimit) == -1) {
		throw PBSException("Unable to fetch the resources limits.");
	}
	nowResourceLimit.rlim_cur = currentResource;
	nowResourceLimit.rlim_max = maximumResource;
	if (setrlimit(resourceId, &nowResourceLimit) == -1) {
		throw PBSException("Unable to setting the resources limits.");
	}
}

void pbsSoftBanSyscall(const pid_t &pid, user_regs_struct reg) {
#ifdef DEBUG2
	if (pbsConfig.debugMode) {
		flog("Soft banned the syscall %lld\n", reg.orig_rax);
	}
#endif
	reg.orig_rax += 1024;
	ptrace(PTRACE_SETREGS, pid, NULL, &reg);
}

bool pbsIsWritableFile(const std::string &filename) {
	if (verdictFileInSet(filename, pbsConfig.writableFiles)) {
		return true;
	} else {
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("IsWritableFile %s.\n", filename.c_str());
		}
#endif
		return false;
	}
}

bool pbsIsReadableFile(const std::string &filename) {
	if (pbsIsWritableFile(filename)) {
		return true;
	}
	if (verdictFileInSet(filename, pbsConfig.readableFiles)) {
		return true;
	} else {
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("IsReadableFile %s.\n", filename.c_str());
		}
#endif
		return false;
	}
}

bool pbsIsStatableFile(const std::string &filename) {
	if (pbsIsReadableFile(filename)) {
		return true;
	}
	if (verdictFileInSet(filename, pbsConfig.statableFiles)) {
		return true;
	} else {
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("IsStatableFile %s.\n", filename.c_str());
		}
#endif
		return false;
	}
}

bool pbsIsUnacsbleFile(const std::string &filename) {
	if (verdictFileInSet(filename, pbsConfig.unacsbleFiles)) {
		return true;
	} else {
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("IsUnacsbleFile %s.\n", filename.c_str());
		}
#endif
		return false;
	}
}

bool pbsDetectExtraFile(const pid_t &pid, user_regs_struct reg, const std::string &filename) {
	if (pbsIsUnacsbleFile(filename)) {
		pbsSoftBanSyscall(pid, reg);
		return true;
	} else {
		return false;
	}
}

void pbsDefendExitSyscall(const pid_t &pid) {
	struct user_regs_struct reg;
	ptrace(PTRACE_GETREGS, pid, NULL, &reg);
#ifdef DEBUG2
	if (pbsConfig.debugMode) {
		if ((long long int)reg.REG_SYSCALL >= 1024) {
			flog("Defended -- BAN SYSCALL %lld\n", reg.orig_rax - 1024);
		} else {
			flog("Defended -- EXIT SYSCALL %lld\n", reg.orig_rax);
		}
	}
#endif
	if ((long long int)reg.REG_SYSCALL >= 1024) {
		reg.orig_rax -= 1024;
		reg.rax = -EACCES;
		ptrace(PTRACE_SETREGS, pid, NULL, &reg);
	}
}

bool pbsCheckSafeSyscall(const pid_t &pid) {
	struct user_regs_struct reg;
	ptrace(PTRACE_GETREGS, pid, NULL, &reg);
	
	int pbsCurrentInstruction = ptrace(PTRACE_PEEKTEXT, pid, reg.rip - 2, NULL) & 0xffff;
	if (pbsCurrentInstruction != 0x050f) {
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("%d Informal syscall founded, instruction was at %d.\n", pid, pbsCurrentInstruction);
		}
#endif
		return false;
	}
	int pbsSyscallId = (int)reg.REG_SYSCALL;
#ifdef DEBUG2
	if (pbsConfig.debugMode) {
		flog("%d Syscall %d was captured.\n", pid, pbsSyscallId);
	}
#endif
	if (pbsSyscallId < 0 || pbsSyscallId >= MAX_SYSCALL) {
		return false;
	}
	if (pbsConfig.softBanSyscall[pbsSyscallId]) {
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("%d Soft ban syscall %d\n", pid, pbsSyscallId);
		}
#endif
		pbsSoftBanSyscall(pid, reg);
	} else if (pbsConfig.syscallCounter[pbsSyscallId] != -1) {
		if (pbsConfig.syscallCounter[pbsSyscallId] == 0) {
#ifdef DEBUG2
			if (pbsConfig.debugMode) {
				flog("%d Dangerous syscall %d detected.\n", pid, pbsSyscallId);
			}
#endif
			return false;
		}
		pbsConfig.syscallCounter[pbsSyscallId]--;
	}
	// Security Group Policy
	if (pbsSyscallId == __NR_open || pbsSyscallId == __NR_openat) {
		reg_val_t pbsRegFileNameAddress;
		reg_val_t pbsRegFlags;
		if (pbsSyscallId == __NR_open) {
			pbsRegFileNameAddress = reg.rdi;
			pbsRegFlags = reg.rsi;
		} else {
			pbsRegFileNameAddress = reg.rsi;
			pbsRegFlags = reg.rdi;
		}
		std::string pbsFileName = getAbsPathFromRegAddress(pbsRegFileNameAddress, pid);
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			std::string ways;
			switch (pbsRegFlags & O_ACCMODE) {
				case O_RDONLY:
					ways = "r";
					break;
				case O_WRONLY:
					ways = "w";
					break;
				case O_RDWR:
					ways = "rw";
					break;
				default:
					ways = "??";
					break;
			}
			flog("--- %d open %s by %s\n", pid, pbsFileName.c_str(), ways.c_str());
		}
#endif
		bool pbsSyscallOpenOnly = (pbsRegFlags & O_ACCMODE) == O_RDONLY &&
			(pbsRegFlags & O_CREAT) == 0 && (pbsRegFlags & O_EXCL) == 0 &&
			(pbsRegFlags & O_TRUNC) == 0;
		if (pbsSyscallOpenOnly) {
			if (getRealPath(pbsFileName) != "" && !pbsIsReadableFile(pbsFileName)) {
				return pbsDetectExtraFile(pid, reg, pbsFileName);
			}
		} else {
			if (!pbsIsWritableFile(pbsFileName)) {
				return pbsDetectExtraFile(pid, reg, pbsFileName);
			}
		}
		// Open file security group policy
	} else if (pbsSyscallId == __NR_readlink || pbsSyscallId == __NR_readlinkat) {
		reg_val_t pbsRegFileNameAddress;
		if (pbsSyscallId == __NR_readlink) {
			pbsRegFileNameAddress = reg.rdi;
		} else {
			pbsRegFileNameAddress = reg.rsi;
		}
		std::string pbsFileName = getAbsPathFromRegAddress(pbsRegFileNameAddress, pid);
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("--- %d readlink at %s\n", pid, pbsFileName.c_str());
		}
#endif
		if (!pbsIsReadableFile(pbsFileName)) {
			return pbsDetectExtraFile(pid, reg, pbsFileName);
		}
	} else if (pbsSyscallId == __NR_unlink || pbsSyscallId == __NR_unlinkat) {
		reg_val_t pbsRegFileNameAddress;
		if (pbsSyscallId == __NR_unlink) {
			pbsRegFileNameAddress = reg.rdi;
		} else {
			pbsRegFileNameAddress = reg.rsi;
		}
		std::string pbsFileName = getAbsPathFromRegAddress(pbsRegFileNameAddress, pid);
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("--- %d unlink at %s\n", pid, pbsFileName.c_str());
		}
#endif
		if (!pbsIsWritableFile(pbsFileName)) {
			return pbsDetectExtraFile(pid, reg, pbsFileName);
		}
	} else if (pbsSyscallId == __NR_access) {
		reg_val_t pbsRegFileNameAddress = reg.rdi;
		std::string pbsFileName = getAbsPathFromRegAddress(pbsRegFileNameAddress, pid);
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("--- %d access at %s\n", pid, pbsFileName.c_str());
		}
#endif
		if (!pbsIsStatableFile(pbsFileName)) {
			return pbsDetectExtraFile(pid, reg, pbsFileName);
		}
	} else if (pbsSyscallId == __NR_stat || pbsSyscallId == __NR_lstat) {
		reg_val_t pbsRegFileNameAddress = reg.rdi;
		std::string pbsFileName = getAbsPathFromRegAddress(pbsRegFileNameAddress, pid);
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("--- %d stat %s\n", pid, pbsFileName.c_str());
		}
#endif
		if (!pbsIsStatableFile(pbsFileName)) {
			return pbsDetectExtraFile(pid, reg, pbsFileName);
		}
	} else if (pbsSyscallId == __NR_execve) {
		reg_val_t pbsRegFileNameAddress = reg.rdi;
		std::string pbsFileName = getAbsPathFromRegAddress(pbsRegFileNameAddress, pid);
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("--- %d execve %s\n", pid, pbsFileName.c_str());
		}
#endif
		if (!pbsIsReadableFile(pbsFileName)) {
			return pbsDetectExtraFile(pid, reg, pbsFileName);
		}
	} else if (pbsSyscallId == __NR_chmod || pbsSyscallId == __NR_rename) {
		reg_val_t pbsRegFileNameAddress = reg.rdi;
		std::string pbsFileName = getAbsPathFromRegAddress(pbsRegFileNameAddress, pid);
#ifdef DEBUG2
		if (pbsConfig.debugMode) {
			flog("--- %d change %s\n", pid, pbsFileName.c_str());
		}
#endif
		if (!pbsIsWritableFile(pbsFileName)) {
			return pbsDetectExtraFile(pid, reg, pbsFileName);
		}
	}

	return true;
}

#endif
