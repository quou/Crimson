#pragma once

#include <string>

#include <atomic>

namespace Crimson {
	static std::atomic<float> g_currentCompressProgress = 0.0f;
	static std::atomic<bool> g_isExporting = false;

	void CompressFolder(const std::string& folder, const std::string& zipName);
	void CopyFile(const std::string& source, const std::string& destination);
}
