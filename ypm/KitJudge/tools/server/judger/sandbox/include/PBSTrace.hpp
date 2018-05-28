#ifndef PBS_TRACE_HPP
#define PBS_TRACE_HPP

#include "PBSBase.hpp"
#include "PBSException.hpp"
#include "PBSControl.hpp"
#include "PBSConfig.hpp"
#include "PBSDebug.hpp"
#include "PBSResult.hpp"
#include "PBSProcessManager.hpp"

PBSResult pbsTrace(const pid_t &pbsExecutorPid) {
	pid_t pbsTimerPid = fork();
	if (pbsTimerPid == -1) {
        return PBSResult(RS_JGF, "Cannot create the timer subprocess.");
	} else if (pbsTimerPid == 0) {
		struct timespec ts;
		ts.tv_sec = pbsConfig.timeLimit / 1000 + 2;
		ts.tv_nsec = (pbsConfig.timeLimit % 1000) * 1000000;
#ifdef DEBUG1
		if (pbsConfig.debugMode) {
			tlog("Timer subprocess created, pid = %d, ppid = %d, sleep time = %.3f seconds.\n", getpid(), getppid(), ts.tv_sec + ts.tv_nsec / 1e9);
		}
#endif
		nanosleep(&ts, NULL);

#ifdef DEBUG1
		if (pbsConfig.debugMode) {
			tlog("Timer stoped.\n");
		}
#endif
		exit(0);
	}

    pbsProcessManager.init();
    pbsProcessManager.insertProcess(pbsExecutorPid, UNVISITED);

	int status, sig = 0, ptraceSettled = false;
	struct rusage ruse;
	while (true) {
		pid_t pid = wait4(-1, &status, __WALL, &ruse);
		if (pid == pbsTimerPid) {
			if (WIFEXITED(status) || WIFSIGNALED(status)) {
#ifdef DEBUG1
				if (pbsConfig.debugMode) {
					flog("Timer subprocess is detected ended.\n");
				}
#endif
                pbsKillAll(pbsTimerPid);
                return PBSResult(RS_TLE);
			}
            continue;
		} else if (pid != -1) {
            int posInProcessManager = pbsProcessManager.findProcess(pid);
            if (posInProcessManager == -1) {
#ifdef DEBUG1
                if (pbsConfig.debugMode) {
                    flog("A new subprocess(PID = %d) is created.\n", pid);
                }
#endif
                if ((posInProcessManager = pbsProcessManager.insertProcess(pid, UNVISITED)) == -1) {
#ifdef DEBUG1
                    if (pbsConfig.debugMode) {
                        flog("Failed to insert process %d into pbsProcessManager.", pid);
                    }
#endif
                    kill(pid, SIGKILL);
                    pbsKillAll(pbsTimerPid);
                    return PBSResult(RS_DGS, "Cannot create more subprocess.");
                }
            }
            int usedTime = ruse.ru_utime.tv_sec * 1000 + ruse.ru_utime.tv_usec / 1000;
            //flog("usedTime = %d\n", usedTime);
            int usedMemory = ruse.ru_maxrss;
            if (usedTime > pbsConfig.timeLimit) {
#ifdef DEBUG1
                if (pbsConfig.debugMode) {
                    flog("Used time is larger than time limit.\n");
                }
#endif
                pbsKillAll(pbsTimerPid);
                return PBSResult(RS_TLE);
            }
            if (usedMemory > pbsConfig.memoryLimit) {
#ifdef DEBUG1
                if (pbsConfig.debugMode) {
                    flog("Used memory is larger than time limit.\n");
                }
#endif
                pbsKillAll(pbsTimerPid);
                return PBSResult(RS_MLE, -1, usedMemory, -1);
            }

			if (WIFEXITED(status)) {
#ifdef DEBUG1
				if (pbsConfig.debugMode) {
					flog("Subprocess %d is detected ended, exitcode = %d.\n", pid, WEXITSTATUS(status));
				}
#endif
                if (pbsProcessManager.activeProcess[posInProcessManager].mode == UNVISITED) {
                    pbsKillAll(pbsTimerPid);
#ifdef DEBUG1
                    if (pbsConfig.debugMode) {
                        flog("Subprocess(PID = %d) is ended before traced.\n", pid);
                    }
#endif
					int shmid = shmget((key_t)SHM_KEY, sizeof(struct PBSException), 0666|IPC_CREAT);
					if (shmid == -1) {
						int exitcode = WEXITSTATUS(status);
						if (exitcode == SHM_FAIL_CREATE_EXITCODE) {
                    		return PBSResult(RS_JGF, "Abnormal ending of executor and the executor process failed to create the shared memory.");
						} else if (exitcode == SHM_FAIL_LINK_EXITCODE) {
                    		return PBSResult(RS_JGF, "Abnormal ending of executor and the executor process failed to link the shared memory.");
						} else if (exitcode == SHM_FAIL_UNLINK_EXITCODE) {
                    		return PBSResult(RS_JGF, "Abnormal ending of executor and the executor process failed to unlink the shared memory.");
						} else if (exitcode == SHM_FATAL_ERROR_EXITCODE){
                    		return PBSResult(RS_JGF, "Abnormal ending of executor and fatal error occured.");
						} else {
                    		return PBSResult(RS_JGF, "Abnormal ending of executor and the reason is unknown.");
						}
					} else {
						const void *shm = shmat(shmid, NULL, 0);
						if (shm == (const void *)-1) {
							if (shmctl(shmid, IPC_RMID, 0) == -1) {
		                		return PBSResult(RS_JGF, "Abnormal ending of executor and fatal error occured.");
							}
                    		return PBSResult(RS_JGF, "Abnormal ending of executor and failed to link the error message.");
						}
						std::string message = static_cast<const struct PBSException *>(shm) -> message;
						if (shmdt(shm) == -1) {
							if (shmctl(shmid, IPC_RMID, 0) == -1) {
		                		return PBSResult(RS_JGF, "Abnormal ending of executor and fatal error occured.");
							}
                    		return PBSResult(RS_JGF, "Abnormal ending of executor and failed to unlink the error message(" + message + ").");
						}
						if (shmctl(shmid, IPC_RMID, 0) == -1) {
                    		return PBSResult(RS_JGF, "Abnormal ending of executor and failed to destory the error message(" + message + ").");
						}
						return PBSResult(RS_JGF, "Captured Exception: " + message);
					}
					return PBSResult(RS_JGF, "A subprocess is ended before traced.");
                } else {
                    if (pid == pbsExecutorPid) {
#ifdef DEBUG1
						if (pbsConfig.debugMode) {
							flog("The executor main subprocess normally ended.\n");
						}
#endif
                        pbsKillAll(pbsTimerPid);
                        return PBSResult(RS_OK, usedTime, usedMemory, WEXITSTATUS(status));
                    } else {
                        pbsProcessManager.removeProcess(pid);
                        continue;
                    }
                }
			}
			if (WIFSIGNALED(status)) {
#ifdef DEBUG1
				if (pbsConfig.debugMode) {
					flog("Found signaled status %d\n", status);
				}
#endif
				if (pid == pbsExecutorPid) {
					switch (WTERMSIG(status)) {
						case SIGXCPU:
							pbsKillAll(pbsTimerPid);
#ifdef DEBUG1
							if (pbsConfig.debugMode) {
								flog("Program exceeds the time limit at informal SIGXCPU.\n");
							}
#endif
							return PBSResult(RS_TLE);
						case SIGXFSZ:
							pbsKillAll(pbsTimerPid);
#ifdef DEBUG1
							if (pbsConfig.debugMode) {
								flog("Program exceeds the output limit at SIGXFSZ.\n");
							}
#endif
							return PBSResult(RS_OLE);
						default:
							pbsKillAll(pbsTimerPid);
#ifdef DEBUG1
							if (pbsConfig.debugMode) {
								flog("Program verdicted to be runtime error at %d.\n", WTERMSIG(status));
							}
#endif
							return PBSResult(RS_RE, getNaturalDescriptionOfSignalCode(WTERMSIG(status)));
					}
				} else {
					pbsProcessManager.removeProcess(pid);
					continue;
				}
			}
			if (WIFSTOPPED(status)) {
				sig = WSTOPSIG(status);
				if (pbsProcessManager.activeProcess[posInProcessManager].mode == UNVISITED) {
					// TODO Unknow Case 1
					if ((posInProcessManager == 0 && sig == SIGTRAP) || (posInProcessManager != 0 && sig == SIGSTOP)) {
						if (posInProcessManager == 0) {
							int pbsPtraceOptions = PTRACE_O_EXITKILL | PTRACE_O_TRACESYSGOOD | PTRACE_O_TRACEEXEC;
							if (pbsConfig.safeMode) {
								pbsPtraceOptions |= PTRACE_O_TRACECLONE | PTRACE_O_TRACEFORK | PTRACE_O_TRACEVFORK;
							}
							if (ptrace(PTRACE_SETOPTIONS, pid, NULL, pbsPtraceOptions) == -1) {
								pbsKillAll(pbsTimerPid);
								return PBSResult(RS_JGF, "Failed to set the ptrace options.");
							}
						}
						sig = 0;
					}
					pbsProcessManager.activeProcess[posInProcessManager].mode = VISITED_SYSCALL;
				} else if (sig == (SIGTRAP | 0x80)) {
					if (pbsProcessManager.activeProcess[posInProcessManager].mode == VISITED_SYSCALL) {
						if (pbsConfig.safeMode) {
							if (!pbsCheckSafeSyscall(pid)) {
								pbsKillAll(pbsTimerPid);
								return PBSResult(RS_DGS, "Dangerous syscall founded.");
							}
						}
						pbsProcessManager.activeProcess[posInProcessManager].mode = VISITED_EXITSYSCALL;
					} else {
						if (pbsConfig.safeMode) {
							pbsDefendExitSyscall(pid);
						}
						pbsProcessManager.activeProcess[posInProcessManager].mode = VISITED_SYSCALL;
					}
					sig = 0;
				} else if (sig == SIGTRAP) {
					// TODO Unknown Case
					switch ((status >> 16) & 0xffff) {
						case PTRACE_EVENT_CLONE:
						case PTRACE_EVENT_FORK:
						case PTRACE_EVENT_VFORK:
							sig = 0;
							break;
						case PTRACE_EVENT_EXEC:
							pbsProcessManager.activeProcess[posInProcessManager].mode = VISITED_EXITSYSCALL;
							sig = 0;
							break;
						case 0:
							break;
						default:
							pbsKillAll(pbsTimerPid);
							return PBSResult(RS_JGF, "Uncompleted cases.");
					}
				}
				switch (sig) {
					case SIGXCPU :
						pbsKillAll(pbsTimerPid);
#ifdef DEBUG1
						if (pbsConfig.debugMode) {
							flog("Program exceeds the time limit at SIGXCPU.\n");
						}
#endif
						return PBSResult(RS_TLE);
					case SIGXFSZ :
						pbsKillAll(pbsTimerPid);
#ifdef DEBUG1
						if (pbsConfig.debugMode) {
							flog("Program exceeds the output limit at SIGXFSZ.\n");
						}
#endif
						return PBSResult(RS_OLE);
#ifdef DEBUG1
					default:
						if (sig != 0 && pbsConfig.debugMode) {
							flog("Captured signal %d.\n", sig);
						}
#endif
				}
			}
		} else {
			pbsKillAll(pbsTimerPid);
			return PBSResult(RS_JGF, "Cannot find the executor subprocess when tracing.");
		}
		ptrace(PTRACE_SYSCALL, pid, NULL, sig);
	}
}

#endif
