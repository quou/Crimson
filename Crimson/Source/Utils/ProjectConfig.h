#pragma once

#include <string>

namespace Crimson {
	struct ProjectConfig {
		std::string startup;
	};

	ProjectConfig LoadProjectConfig(const std::string& lua);
}
