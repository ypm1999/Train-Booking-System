#ifndef PBS_ARGP_HPP
#define PBS_ARGP_HPP

#include "PBSBase.hpp"
#include "PBSConfig.hpp"

char PBS_ARGP_ARGS_DOCUMENT[] = "program arg1 arg2 ...";
char PBS_ARGP_DOCUMENT[] = "pbsbox: A ptrace based sandbox supporting measuring functions.";

argp_option PBS_ARGP_OPTIONS[] ={
	{"tl"                 , 'T', "TIME_LIMIT"  , 0, "Set time limit (in microseconds)"                                  ,  1},
	{"ml"                 , 'M', "MEMORY_LIMIT", 0, "Set memory limit (in kilobytes)"                                   ,  2},
	{"ol"                 , 'O', "OUTPUT_LIMIT", 0, "Set output limit (in kilobytes)"                                       ,  3},
	{"sl"                 , 'S', "STACK_LIMIT" , 0, "Set stack limit (in kilobytes)"                                    ,  4},
	{"in"                 , 'i', "IN"          , 0, "Set input page"                                                    ,  5},
	{"out"                , 'o', "OUT"         , 0, "Set output page"                                                   ,  6},
	{"err"                , 'e', "ERR"         , 0, "Set error page"                                                    ,  7},
	{"work-path"          , 'w', "WORK_PATH"   , 0, "Set the work path of the program"                                  ,  8},
	{"type"               , 't', "TYPE"        , 0, "Set the program type (Only built-in options provided here)"        ,  9},
	{"add-readable"       , 500, "FILE"        , 0, "Add a readable file"                                               , 10},
	{"add-writable"       , 501, "FILE"        , 0, "Add a writable file"                                               , 11},
	{"add-statable"       , 502, "FILE"        , 0, "Add a statable file"                                               , 12},
	{"add-unaccess"       , 503, "FILE"        , 0, "Add a unaccessble file"                                            , 13},
	{"unsafe"             , 504, 0             , 0, "Don't check dangerous syscalls"                                    , 14},
	{"detail"             , 505, 0             , 0, "Print the detailed result."                                        , 15},
#if defined(DEBUG1) || defined(DEBUG2)
	{"show-debug-message" , 506, 0             , 0, "Show trace details"                                                , 16},
#endif
	{0}
};

int getPositiveInteger(char *s) {
	int n = strlen(s), ret = 0;
	for (int i = 0; i < n; i++) {
		if (isdigit(s[i])) ret = ret * 10 + s[i] - '0';
		else return -1;
	}
	return ret;
}

error_t pbsArgpParser (int key, char *arg, struct argp_state *state) {
	PBSConfig *config = (PBSConfig *)state->input;
	switch (key) {
		case 'T':
			config -> timeLimit = getPositiveInteger(arg);
			break;
		case 'M':
			config -> memoryLimit = getPositiveInteger(arg);
			break;
		case 'O':
			config -> outputLimit = getPositiveInteger(arg);
			break;
		case 'S':
			config -> stackLimit = getPositiveInteger(arg);
			break;
		case 'i':
			config -> inputPage = arg;
			break;
		case 'o':
			config -> outputPage = arg;
			break;
		case 'e':
			config -> errorPage = arg;
			break;
		case 'w':
			config -> workPath = arg;
			break;
		case 't':
			config -> type = arg;
			break;
		case 500:
			config -> readableFiles.insert(getRealPath(arg));
			break;
		case 501:
			config -> writableFiles.insert(getRealPath(arg));
			break;
		case 502:
			config -> statableFiles.insert(getRealPath(arg));
			break;
		case 503:
			config -> unacsbleFiles.insert(getRealPath(arg));
			break;
		case 504:
			config -> safeMode = false;
			break;
		case 505:
			config -> simpleMode = false;
			break;
#if defined(DEBUG1) || defined(DEBUG2)
		case 506:
			config -> debugMode = true;
			break;
#endif
		case ARGP_KEY_ARG:
			config -> argv.push_back(arg);
			for (int i = state -> next; i < state -> argc; i++) {
				config -> argv.push_back(state -> argv[i]);
			}
			state -> next = state -> argc;
			break;
		case ARGP_KEY_END:
			if (state -> arg_num == 0) {
				argp_usage(state);
			}
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

argp PBS_ARGP = {
	PBS_ARGP_OPTIONS,
	pbsArgpParser,
	PBS_ARGP_ARGS_DOCUMENT,
	PBS_ARGP_DOCUMENT
};

void pbsParseArgs(int argc, char *argv[]) {
	argp_parse(&PBS_ARGP, argc, argv, ARGP_NO_ARGS | ARGP_IN_ORDER, 0, &pbsConfig);
	pbsConfig.stackLimit = std::min(pbsConfig.stackLimit, pbsConfig.memoryLimit);
	
	if (pbsConfig.timeLimit < 0) {
		throw PBSException("Invalid time limit.");
	}
	if (pbsConfig.memoryLimit < 0) {
		throw PBSException("Invalid memory limit.");
	}
	if (pbsConfig.outputLimit < 0) {
		throw PBSException("Invalid output limit.");
	}
	if (pbsConfig.stackLimit < 0) {
		throw PBSException("Invalid stack limit.");
	}
	if (pbsConfig.workPath != "") {
		if (getRealPath(pbsConfig.workPath) == "") {
			throw PBSException("Invalid work path.");
		}
		pbsConfig.workPath = getRealPath(pbsConfig.workPath) + "/";
	}
	if (!pbsConfig.workPath.empty()) {
		if (chdir(pbsConfig.workPath.c_str()) == -1) {
			throw PBSException("Cannot enter the workpath \"" + pbsConfig.workPath + "\".");
		}
	}
	pbsConfig.program = getRealPath(pbsConfig.argv[0]);
	if (pbsConfig.program == "") {
		throw PBSException("Cannot find the program \"" + pbsConfig.argv[0] + "\".");
	}
	if (pbsConfig.workPath.empty()) {
		pbsConfig.workPath = getDirectory(pbsConfig.program);
		pbsConfig.argv[0] = "./" + getBasePath(pbsConfig.program);
		if (chdir(pbsConfig.workPath.c_str()) == -1) {
			throw PBSException("Cannot enter the workpath \"" + pbsConfig.workPath + "\".");
		}
	}
}

#endif
