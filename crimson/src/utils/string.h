#pragma once

#include <vector>
#include <string>

#include "core.h"

namespace Crimson {
	std::vector<std::string> CR_API SplitString(const std::string& string, const std::string& delimiters);
}