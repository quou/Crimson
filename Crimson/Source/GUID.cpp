#include "GUID.h"

#include <sstream>
#include <random>
#include <string>

namespace Crimson {
	unsigned int RandomChar() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 255);
		return dis(gen);
	}

	GUID GenerateGUID(unsigned int length) {
		std::stringstream ss;
		for (auto i = 0; i < length; i++) {
			const auto rc = RandomChar();
			std::stringstream hexstream;
			hexstream << std::hex << rc;
			auto hex = hexstream.str();
			ss << (hex.length() < 2 ? '0' + hex : hex);
		}
		return ss.str();
	}
}
