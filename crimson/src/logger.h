#pragma once

namespace Crimson {
	enum class LogType {
		INFO,
		ERROR,
		WARNING,
	};

	void InitLogger();
	void CloseLogger();
	
	void Log(LogType severity, const char* fmt, ...);
}
