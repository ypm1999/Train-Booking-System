#ifndef PBS_PROCESSMANAGER_HPP
#define PBS_PROCESSMANAGER_HPP

#include "PBSBase.hpp"

enum PBS_PROCESS_MODE{
    VISITED_SYSCALL, UNVISITED, VISITED_EXITSYSCALL
};

struct PBSProcess{
    pid_t pid;
    PBS_PROCESS_MODE mode;
    PBSProcess() {}
    PBSProcess(const pid_t &pid, PBS_PROCESS_MODE mode) : pid(pid), mode(mode) {}
};

struct PBSProcessManager{ // TODO: Maybe be optimized.
    int numberOfActiveProcess;
    PBSProcess activeProcess[MAX_PROC];
    bool empty()const {
    	return numberOfActiveProcess == 0;
    }
    void init() {
        numberOfActiveProcess = 0;
    }
    int insertProcess(const pid_t &pid, const PBS_PROCESS_MODE &mode) {
        for (int i = 0; i < numberOfActiveProcess; i++) {
            if (activeProcess[i].pid == pid) return i;
        }
        if (numberOfActiveProcess == MAX_PROC) return -1;
        activeProcess[numberOfActiveProcess++] = PBSProcess(pid, mode);
        return numberOfActiveProcess - 1;
    }
    bool removeProcess(const pid_t &pid) {
        int newNumberOfActiveProcess = 0;
        for (int i = 0; i < numberOfActiveProcess; i++) {
            if (activeProcess[i].pid == pid) continue;
            activeProcess[newNumberOfActiveProcess++] = activeProcess[i];
        }
        numberOfActiveProcess = newNumberOfActiveProcess;
    }
    int findProcess(const pid_t &pid) {
        for (int i = 0; i < numberOfActiveProcess; i++) {
            if (activeProcess[i].pid == pid) return i;
        }
        return -1;
    }
};

extern PBSProcessManager pbsProcessManager;

void pbsKillAll(const pid_t &pbsTimerPid) {
    kill(pbsTimerPid, SIGKILL);
    for (int i = 0; i < pbsProcessManager.numberOfActiveProcess; i++) {
        kill(pbsProcessManager.activeProcess[i].pid, SIGKILL);
    }
}

#endif
