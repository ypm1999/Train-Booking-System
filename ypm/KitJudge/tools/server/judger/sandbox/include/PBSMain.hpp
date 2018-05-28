#ifndef PBS_MAIN_HPP
#define PBS_MAIN_HPP

#include "PBSTrace.hpp"
#include "PBSExecute.hpp"
#include "PBSException.hpp"
#include "PBSArgp.hpp"

PBSProcessManager pbsProcessManager;
PBSConfig pbsConfig;

void pbsMain(int argc, char *argv[]) {
	try{
		initCommonPBSConfig(pbsConfig);
		pbsParseArgs(argc, argv);
		pid_t pbsExecutorPid = fork();
		if (pbsExecutorPid == -1) {
			throw PBSException("Cannot create the executor subprocess.");
		} else if (pbsExecutorPid == 0) {
#ifdef DEBUG1
			if (pbsConfig.debugMode) {
				slog("Child process created, PID = %d, PPID = %d\n", getpid(), getppid());
			}
#endif
			try{
				pbsExecute();
			} catch (const PBSException &e) {
				int shmid = shmget((key_t)SHM_KEY, sizeof(struct PBSException), 0666 | IPC_CREAT);
				if (shmid == -1) {
					exit(SHM_FAIL_CREATE_EXITCODE);
				} else {
					void *shm = shmat(shmid, NULL, 0);
					if (shm == (const void *)-1) {
						if (shmctl(shmid, IPC_RMID, 0) == -1) {
							exit(SHM_FATAL_ERROR_EXITCODE);
						}
						exit(SHM_FAIL_LINK_EXITCODE);
					}
					*static_cast<struct PBSException *>(shm) = e;
					if (shmdt(shm) == -1) {
						if (shmctl(shmid, IPC_RMID, 0) == -1) {
							exit(SHM_FATAL_ERROR_EXITCODE);
						}
						exit(SHM_FAIL_UNLINK_EXITCODE);
					}
				}
			}
		} else {
#ifdef DEBUG1
			if (pbsConfig.debugMode) {
				flog("Father process information, PID = %d, CPID = %d\n", getpid(), pbsExecutorPid);
			}
#endif
			initParentPBSConfig(pbsConfig);
#ifdef DEBUG1
		pbsPrintConfig();
#endif
			PBSResult pbsResult = pbsTrace(pbsExecutorPid);
		    pbsPrintResult(pbsResult);
		}
	} catch (const PBSException &e) {
		e.print();
	}
}

#endif
