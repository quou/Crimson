#pragma once

#include <cassert>

enum LogType {
	CR_LOGTYPE_INFO,
	CR_LOGTYPE_ERROR,
	CR_LOGTYPE_FATAL_ERROR,
	CR_LOGTYPE_WARNING,
};

namespace Crimson {
	void Log(LogType logType, const char* fmt, ...);
	void Assert(bool condition, const char* fmt, ...);
}

#define CR_LOG(fmt, args)(Crimson::Log(CR_LOGTYPE_INFO, fmt, args))
#define CR_PRINTF(fmt, ...)(printf(fmt, __VA_ARGS__))
#define CR_LOG_ERROR(fmt, ...)(Crimson::Log(CR_LOGTYPE_ERROR, fmt, __VA_ARGS__))
#define CR_LOG_FATAL_ERROR(fmt, ...)(Crimson::Log(CR_LOGTYPE_FATAL_ERROR, fmt, __VA_ARGS__))
#define CR_LOG_WARNING(fmt, ...)(Crimson::Log(CR_LOGTYPE_WARNING, fmt, __VA_ARGS__))
#define CR_ASSERT(condition, fmt, ...)(Crimson::Assert(condition, fmt, __VA_ARGS__))
