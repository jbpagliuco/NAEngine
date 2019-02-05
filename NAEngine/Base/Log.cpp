#include "Log.h"

#include <stdio.h>
#include <string>

#include "Windows.h"
#include "Util.h"

namespace na
{
	static void LogToOutputWindow(const char *filter, const char *message)
	{
		size_t garbage;

		wchar_t wFilter[256];
		mbstowcs_s(&garbage, wFilter, filter, STATIC_ARRAY_SIZE(wFilter));
		OutputDebugString(wFilter);

		wchar_t wMessage[1024];
		mbstowcs_s(&garbage, wMessage, message, STATIC_ARRAY_SIZE(wMessage));
		OutputDebugString(wMessage);

		OutputDebugString(L"\r\n");
	}

	void LogInfoV(const char *filter, const char *format, va_list args)
	{
		char filterFormatted[64];
		sprintf_s(filterFormatted, "[%s] ", filter);

		char message[1024];
		vsprintf_s(message, format, args);

		printf("%s%s\r\n", filterFormatted, message);

		LogToOutputWindow(filterFormatted, message);
	}

	void LogInfo(const char *filter, const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		LogInfoV(filter, format, args);
		va_end(args);
	}

	void LogErrorV(const char *filter, const char *format, va_list args)
	{
		char filterFormatted[64];
		sprintf_s(filterFormatted, "[%s] ", filter);

		char message[1024];
		vsprintf_s(message, format, args);

		// Print with red color
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		WORD saved_attributes;

		// Save current attributes
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
		saved_attributes = consoleInfo.wAttributes;

		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		printf("%s%s\r\n", filterFormatted, message);

		// Restore original attributes
		SetConsoleTextAttribute(hConsole, saved_attributes);

		LogToOutputWindow(filterFormatted, message);
	}

	void LogError(const char *filter, const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		LogErrorV(filter, format, args);
		va_end(args);
	}
}