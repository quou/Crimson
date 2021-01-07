#pragma once

#include "core.h"

namespace Crimson {
	enum class CR_API LogType {
		INFO,
		ERROR,
		WARNING,
	};

	void CR_API InitLogger();
	void CR_API CloseLogger();
	
	void CR_API Log(LogType severity, const char* fmt, ...);
}
