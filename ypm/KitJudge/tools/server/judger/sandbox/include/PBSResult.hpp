#ifndef PBS_RESULT_HPP
#define PBS_RESULT_HPP

#include "PBSBase.hpp"

enum PBSVerdict{
    RS_OK, RS_TLE, RS_RE, RS_OLE, RS_MLE, RS_DGS, RS_JGF, // RS_WA, RS_QUEUEING, RS_RUNNING, RS_CE, RS_PE, RS_COMPILING, RS_MLEAK
};

struct PBSResult{
    PBSVerdict verdict;
    std::string extraMessage;
    int time, memory, exitcode;

    PBSResult() {}
    PBSResult(const PBSVerdict &verdict) : verdict(verdict), time(-1), memory(-1), exitcode(-1), extraMessage("") {}
    PBSResult(const PBSVerdict &verdict, const std::string &extraMessage) : verdict(verdict), time(-1), memory(-1), exitcode(-1), extraMessage(extraMessage) {}
    PBSResult(const PBSVerdict &verdict, int time, int memory, int exitcode) :
        verdict(verdict), time(time), memory(memory), exitcode(exitcode), extraMessage("") {}
};

std::string getNaturalDescriptionOfSignalCode(const int &code) {
	switch (code) {
		case SIGFPE:
			return "SIGFPE: Erroneous arithmetic operation.";
		case SIGABRT:
			return "SIGABRT: Process abort signal.";
		case SIGBUS:
			return "SIGBUS: Access to an undefined portion of a memory object.";
		case SIGILL:
			return "SIGILL: Illegal instruction.";
		case SIGQUIT:
			return "SIGQUIT: Terminal quit signal.";
		case SIGSEGV:
			return "SIGSEGV: Invalid memory reference.";
		case SIGSYS:
			return "SIGSYS: Bad system call.";
		default:
			return "Unknown signals.";
	}
}

void pbsPrintResult(const PBSResult &result) {
	if (pbsConfig.simpleMode) {
		fprintf(stdout, "%d\n", result.verdict);
		fprintf(stdout, "%s\n", result.extraMessage.c_str());
		fprintf(stdout, "%d %d %d\n", result.time, result.memory, result.exitcode);
	} else {
		switch (result.verdict) {
			case RS_TLE:
			    fprintf(stdout, "Verdict: Time limit exceeded.\n");
			    break;
			case RS_RE:
			    fprintf(stdout, "Verdict: Runtime Error.\n");
				fprintf(stdout, "Message: %s\n", result.extraMessage.c_str());
			    break;
			case RS_OLE:
			    fprintf(stdout, "Verdict: Output limit exceeded.\n");
			    break;
			case RS_MLE:
			    fprintf(stdout, "Verdict: Memory limit exceeded.\n");
			    break;
			case RS_DGS:
			    fprintf(stdout, "Verdict: Dangerous syscall founded.\n");
			    fprintf(stdout, "Message: %s\n", result.extraMessage.c_str());
			    break;
			case RS_JGF:
			    fprintf(stdout, "Verdict: Judgment failed.\n");
			    fprintf(stdout, "Message: %s\n", result.extraMessage.c_str());
			    break;
			default:
			    fprintf(stdout, "Verdict: Accepted.\n");
		}
		fprintf(stdout, "Timer used: %d ms\n", result.time);
		fprintf(stdout, "Memory used: %d KB\n", result.memory);
		fprintf(stdout, "Exit code: %d\n", result.exitcode);
	}
}

#endif
