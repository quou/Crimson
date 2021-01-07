#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"

/* Do not include colours on Windows, since the Windows
 * Command Prompt doesn't support them, it will just 
 * come out as garbled text. */
#ifdef _WIN32
	#define CONSOLE_COLOR_RESET ""
	#define CONSOLE_COLOR_GREEN ""
	#define CONSOLE_COLOR_RED ""
	#define CONSOLE_COLOR_PURPLE ""
	#define CONSOLE_COLOR_CYAN ""
#else
	#define CONSOLE_COLOR_RESET "\033[0m"
	#define CONSOLE_COLOR_GREEN "\033[1;32m"
	#define CONSOLE_COLOR_RED "\033[1;31m"
	#define CONSOLE_COLOR_PURPLE "\033[1;35m"
	#define CONSOLE_COLOR_CYAN "\033[0;36m"
#endif

static FILE* logFile;

namespace Crimson {
	void InitLogger() {
		/* Obtain the current date and time */
		time_t rawTime;
		struct tm* timeInfo;
		char timeBuffer[80];

		time(&rawTime);
		timeInfo = localtime(&rawTime);

		strftime(timeBuffer, sizeof(timeBuffer), "[%d-%m-%Y %H:%M:%S]", timeInfo);

		/* Open the log file and print the current date and time into it */
		logFile = fopen("log.txt", "a");
		fprintf(logFile, "\n === RESTART @ %s === \n", timeBuffer);
	}

	void Log(LogType severity, const char* fmt, ...) {
		/* Determine strings for the type and colour */
		const char* type;
		const char* color;
		switch (severity) {
			case LogType::INFO:
				type = "info";
				color = CONSOLE_COLOR_GREEN;
				break;
			case LogType::ERROR:
				type = "error";
				color = CONSOLE_COLOR_RED;
				break;
			case LogType::WARNING:
				type = "warning";
				color = CONSOLE_COLOR_PURPLE;
				break;
			default:
				break; /* Unreachable */
		}

		/* Obtain the current date and time */
		time_t rawTime;
		struct tm* timeInfo;
		char timeBuffer[80];

		time(&rawTime);
		timeInfo = localtime(&rawTime);

		strftime(timeBuffer, sizeof(timeBuffer), "[%d-%m-%Y %H:%M:%S]", timeInfo);

		/* Format for printing to the console (with colours) */
		char consoleFormat[1024];
		snprintf(consoleFormat, 1024, "%s%s %s%s%s: %s\n", CONSOLE_COLOR_CYAN, 
				timeBuffer, color, type, CONSOLE_COLOR_RESET, fmt);

		/* Format for printing to the log file (without colours) */
		char fileFormat[1024];
		snprintf(fileFormat, 1024, "%s %s: %s\n", timeBuffer, type, fmt);

		va_list argptr;
		
		/* Print to the console */
		va_start(argptr, fmt);
		vprintf(consoleFormat, argptr);
		va_end(argptr);
	
		if (logFile) {
			/* Print to the log file */
			va_start(argptr, fmt);
			vfprintf(logFile, fileFormat, argptr);
			va_end(argptr);
		}
	}

	void CloseLogger() {
		/* Close the log file */
		fclose(logFile);
	}
}
