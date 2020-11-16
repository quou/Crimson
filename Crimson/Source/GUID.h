#pragma once

#include <string>

namespace Crimson {
	typedef std::basic_string<char> GUID;

	GUID GenerateGUID(unsigned int length=100);
}
