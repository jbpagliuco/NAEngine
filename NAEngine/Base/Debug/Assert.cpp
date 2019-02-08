#include "Assert.h"

#include <cstdlib>
#include <stdio.h>

#include "Log.h"
#include "OS/NAWindows.h"

#define ASSERT_LOG_FILTER "Asserts"

#define LOG_ASSERT_MESSAGE(fmt, ...)						\
	LogError(ASSERT_LOG_FILTER, fmt, __VA_ARGS__);			\
	LogError(ASSERT_LOG_FILTER, "Filename: %s", file);		\
	LogError(ASSERT_LOG_FILTER, "Line: %d", line)		

namespace na
{
	bool PrintAssertMessage(const char *assert, const char *file, int line)
	{
		LOG_ASSERT_MESSAGE("%s", assert);

		switch (MessageBoxA(nullptr, assert, "Assert Failed", MB_YESNOCANCEL)) {
		case IDYES:
			return true;

		case IDNO:
			return false;

		case IDCANCEL:
			exit(-1);
			return false;
		}

		return true;
	}

	bool PrintAssertMessage(const char *assert, const char *file, int line, const char *format, ...)
	{
		va_list args;
		va_start(args, format);

		char message[1024];
		vsprintf_s(message, format, args);
		va_end(args);

		LOG_ASSERT_MESSAGE("%s, %s", assert, message);

		switch (MessageBoxA(nullptr, message, "Assert Failed", MB_YESNOCANCEL)) {
		case IDYES:
			return true;

		case IDNO:
			return false;

		case IDCANCEL:
			exit(-1);
			return false;
		}

		return true;
	}

	void PrintErrorMessage(const char *assert, const char *file, int line)
	{
		LOG_ASSERT_MESSAGE("%s", assert);

		MessageBoxA(nullptr, assert, "Fatal Error", MB_OK);
	}

	void PrintErrorMessage(const char *assert, const char *file, int line, const char *format, ...)
	{
		va_list args;
		va_start(args, format);

		char message[1024];
		vsprintf_s(message, format, args);
		va_end(args);

		LOG_ASSERT_MESSAGE("%s, %s", assert, message);

		MessageBoxA(nullptr, message, "Fatal Error", MB_OK);
	}
}