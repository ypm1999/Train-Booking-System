#ifndef PBS_DEBUG_HPP
#define PBS_DEBUG_HPP

#include "PBSBase.hpp"

#if defined(DEBUG1) or defined(DEBUG2)

#define stdlog stderr

static char __kit_log_buffer[MAX_LOG_BUFFER];

std::string getLogTime() {
	static char buffer[256];
	struct tm *timeNowTMObject;
	time_t timeNowTimeTObject;
	time(&timeNowTimeTObject);
	timeNowTMObject = localtime(&timeNowTimeTObject);
	strftime(buffer, sizeof(buffer), "[%F %T]", timeNowTMObject);
	return buffer;
}

void flog(const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	int length = strlen(fmt);
	__kit_log_buffer[0] = '\0';
	sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "\033[;32m%s \033[;35mFATHER PROCESS:\033[;0m ", getLogTime().c_str());
	for (int i = 0; i < length; i++) {
		if (i + 1 < length && fmt[i] == '%' && fmt[i + 1] == 'd') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%d", va_arg(argp, int));
			i++;
		} else if (i + 1 < length && fmt[i] == '%' && fmt[i + 1] == 's') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%s", va_arg(argp, const char *));
			i++;
		} else if (i + 1 < length && fmt[i] == '%' && fmt[i + 1] == 'c') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%c", va_arg(argp, int));
			i++;
		} else if (i + 3 < length && fmt[i] == '%' && fmt[i + 1] == 'l' && fmt[i + 2] == 'l' && fmt[i + 3] == 'd') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%lld", va_arg(argp, long long));
			i += 3;
		} else if (i + 2 < length && fmt[i] == '%' && fmt[i + 1] == 'l' && fmt[i + 2] == 'd') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%ld", va_arg(argp, long));
			i += 2;
		} else {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%c", fmt[i]);
		}
	}
	fprintf(stdlog, "%s", __kit_log_buffer);
	va_end(argp);
}

void slog(const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	int length = strlen(fmt);
	__kit_log_buffer[0] = '\0';
	sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "\033[;32m%s \033[;36mCHILD PROCESS:\033[;0m ", getLogTime().c_str());
	for (int i = 0; i < length; i++) {
		if (i + 1 < length && fmt[i] == '%' && fmt[i + 1] == 'd') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%d", va_arg(argp, int));
			i++;
		} else if (i + 1 < length && fmt[i] == '%' && fmt[i + 1] == 's') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%s", va_arg(argp, const char *));
			i++;
		} else if (i + 1 < length && fmt[i] == '%' && fmt[i + 1] == 'c') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%c", va_arg(argp, int));
			i++;
		} else if (i + 3 < length && fmt[i] == '%' && fmt[i + 1] == 'l' && fmt[i + 2] == 'l' && fmt[i + 3] == 'd') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%lld", va_arg(argp, long long));
			i += 3;
		} else if (i + 2 < length && fmt[i] == '%' && fmt[i + 1] == 'l' && fmt[i + 2] == 'd') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%ld", va_arg(argp, long));
			i += 2;
		} else {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%c", fmt[i]);
		}
	}
	fprintf(stdlog, "%s", __kit_log_buffer);
	va_end(argp);
}

void tlog(const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	int length = strlen(fmt);
	__kit_log_buffer[0] = '\0';
	sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "\033[;32m%s \033[;33mTIMER PROCESS:\033[;0m ", getLogTime().c_str());
	for (int i = 0; i < length; i++) {
		if (i + 1 < length && fmt[i] == '%' && fmt[i + 1] == 'd') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%d", va_arg(argp, int));
			i++;
		} else if (i + 1 < length && fmt[i] == '%' && fmt[i + 1] == 's') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%s", va_arg(argp, const char *));
			i++;
		} else if (i + 1 < length && fmt[i] == '%' && fmt[i + 1] == 'c') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%c", va_arg(argp, int));
			i++;
		} else if (i + 3 < length && fmt[i] == '%' && fmt[i + 1] == 'l' && fmt[i + 2] == 'l' && fmt[i + 3] == 'd') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%lld", va_arg(argp, long long));
			i += 3;
		} else if (i + 2 < length && fmt[i] == '%' && fmt[i + 1] == 'l' && fmt[i + 2] == 'd') {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%ld", va_arg(argp, long));
			i += 2;
		} else {
			sprintf(__kit_log_buffer + strlen(__kit_log_buffer), "%c", fmt[i]);
		}
	}
	fprintf(stdlog, "%s", __kit_log_buffer);
	va_end(argp);
}

#endif

#endif
