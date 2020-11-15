#pragma once

#include <stdlib.h>

#include <vector>
#include <string>

enum LogType {
	CR_LOGTYPE_INFO,
	CR_LOGTYPE_ERROR,
	CR_LOGTYPE_FATAL_ERROR,
	CR_LOGTYPE_WARNING,
};

namespace Crimson {
	void Log(LogType logType, const char* fmt, ...);
	void MemoryLog(LogType logType, const char* fmt, ...);
	void FlushLog();
	const std::vector<std::pair<LogType, std::string>>& GetLog();
}

#define CR_LOG(fmt, ...) {\
	Crimson::Log(CR_LOGTYPE_INFO, fmt, __VA_ARGS__);\
	Crimson::MemoryLog(CR_LOGTYPE_INFO, fmt, __VA_ARGS__);\
}
#define CR_PRINTF(fmt, ...)(printf(fmt, __VA_ARGS__))

#define CR_LOG_ERROR(fmt, ...){\
	Crimson::Log(CR_LOGTYPE_ERROR, fmt, __VA_ARGS__);\
	Crimson::MemoryLog(CR_LOGTYPE_ERROR, fmt, __VA_ARGS__);\
}
#define CR_LOG_FATAL_ERROR(fmt, ...){\
	Crimson::Log(CR_LOGTYPE_FATAL_ERROR, fmt, __VA_ARGS__);\
	Crimson::MemoryLog(CR_LOGTYPE_FATAL_ERROR, fmt, __VA_ARGS__);\
}
#define CR_LOG_WARNING(fmt, ...){\
	Crimson::Log(CR_LOGTYPE_WARNING, fmt, __VA_ARGS__);\
	Crimson::MemoryLog(CR_LOGTYPE_WARNING, fmt, __VA_ARGS__);\
}
#define CR_ASSERT(condition, fmt, ...)if (!(condition)) { CR_LOG_FATAL_ERROR(fmt, __VA_ARGS__); abort();}
