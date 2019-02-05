#include "Debug.h"

#include <cstdlib>
#include <stdio.h>

#include "Log.h"
#include "Windows.h"

#define ASSERT_LOG_FILTER "asserts"

namespace na
{
	bool PrintAssertMessage(const char *assert)
	{
		LogError(ASSERT_LOG_FILTER, "%s", assert);

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

	bool PrintAssertMessage(const char *assert, const char *format, ...)
	{
		va_list args;
		va_start(args, format);

		char message[1024];
		vsprintf_s(message, format, args);
		va_end(args);

		LogError(ASSERT_LOG_FILTER, "%s, %s", assert, message);

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

	void PrintErrorMessage(const char *assert)
	{
		LogError(ASSERT_LOG_FILTER, "%s", assert);

		MessageBoxA(nullptr, assert, "Fatal Error", MB_OK);
	}

	void PrintErrorMessage(const char *assert, const char *format, ...)
	{
		va_list args;
		va_start(args, format);

		char message[1024];
		vsprintf_s(message, format, args);
		va_end(args);

		LogError(ASSERT_LOG_FILTER, "%s, %s", assert, message);

		MessageBoxA(nullptr, message, "Fatal Error", MB_OK);
	}
}