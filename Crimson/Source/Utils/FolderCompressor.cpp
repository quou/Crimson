#include "FolderCompressor.h"

#include <miniz.h>
#include <miniz_zip.h>

#include "Logger.h"

#include <filesystem>

#if defined(__GNUC__)
  // Ensure we get the 64-bit variants of the CRT's file I/O calls
  #ifndef _FILE_OFFSET_BITS
    #define _FILE_OFFSET_BITS 64
  #endif
  #ifndef _LARGEFILE64_SOURCE
    #define _LARGEFILE64_SOURCE 1
  #endif
#endif

namespace Crimson {
	void CompressFolder(const std::string& folder, const std::string& zipName) {
		const char* comment = "No comment";

		mz_zip_archive archive;
		memset(&archive, 0, sizeof(archive));
		mz_zip_writer_init_file(&archive, zipName.c_str(), 0);

		for (const auto& entry : std::filesystem::recursive_directory_iterator(folder)) {

			if (!entry.is_directory()) {
				mz_bool status = mz_zip_writer_add_file(&archive, entry.path().string().c_str(), entry.path().string().c_str(), comment, (uint16_t)strlen(comment), MZ_BEST_COMPRESSION);
				if (!status) {
					CR_LOG_ERROR("%s", "Failed to write data to archive");
				}
			}
		}

		mz_zip_writer_finalize_archive(&archive);

		mz_zip_writer_end(&archive);
	}

	void CopyFile(const std::string& source, const std::string& destination) {
		std::filesystem::copy(source, destination);
	}
}
