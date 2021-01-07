#pragma once

#if defined(_WIN32)
	#ifdef CR_LIBRARY_EXPORT
		#define CR_API __declspec(dllexport)
	#else
		#define CR_API __declspec(dllimport)
	#endif
#else
	#define CR_API
#endif