#pragma once

#include <vector>
#include <string>

#include "core.h"

namespace Crimson {
	enum class CR_API LogType {
		INFO,
		ERROR,
		WARNING,
	};

	struct CR_API LogEntry {
		LogType type;
		std::string message;
	};

	const std::vector<LogEntry>& CR_API GetLogs();
	void ClearLogs();

	void CR_API InitLogger();
	void CR_API CloseLogger();
	
	void CR_API Log(LogType severity, const char* fmt, ...);
}
