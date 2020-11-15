#include "Logger.h"

#include <stdio.h>
#include <stdarg.h>

#include <string>
#include <ctime>

namespace Crimson {
	static std::vector<std::pair<LogType, std::string>> g_logMessages;

	const std::vector<std::pair<LogType, std::string>>& GetLog() {
		return g_logMessages;
	}

	void MemoryLog(LogType logType, const char* fmt, ...) {
		char buffer[1024];

		va_list argptr;
		va_start(argptr, fmt);

		vsprintf(buffer, fmt, argptr);

		va_end(argptr);

		g_logMessages.push_back({logType, buffer});

		if (g_logMessages.size() > 999) {
			g_logMessages.erase(g_logMessages.begin());
		}
	}

	void FlushLog() {
		g_logMessages.clear();
	}

	void Log(LogType logType, const char* fmt, ...) {
		const char* type;
		switch (logType) {
		case CR_LOGTYPE_INFO:
			type = "\033[1;32minfo: \033[0m";
			break;
		case CR_LOGTYPE_ERROR:
			type = "\033[1;31merror: \033[0m";
			break;
		case CR_LOGTYPE_FATAL_ERROR:
			type = "\033[1;31mfatal error: \033[0m";
			break;
		case CR_LOGTYPE_WARNING:
			type = "\033[1;35mwarning: \033[0m";
			break;
		default:
			break;
		}

		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer,sizeof(buffer),"\033[0;36m[%d-%m-%Y %H:%M:%S]\033[0m ",timeinfo);
		std::string timeStr(buffer);

		std::string absFormat = timeStr + std::string(type) + std::string(fmt) + "\n";

		va_list argptr;
		va_start(argptr, fmt);

		vprintf(absFormat.c_str(), argptr);

		va_end(argptr);
	}
}
