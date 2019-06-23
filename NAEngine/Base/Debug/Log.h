#pragma once

#include <stdarg.h>
#include <string>

namespace na
{
	void LogInfo(const std::string &filter, const char *format, ...);
	void LogInfoV(const std::string &filter, const char *format, va_list args);
	void LogError(const std::string &filter, const char *format, ...);
	void LogErrorV(const std::string &filter, const char *format, va_list args);

	void LogLineBreak();
}