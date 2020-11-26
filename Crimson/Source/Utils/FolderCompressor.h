#pragma once

#include <string>

#include <atomic>

namespace Crimson {
	void CompressFolder(const std::string& folder, const std::string& zipName);
	void CopyFile(const std::string& source, const std::string& destination);
}
