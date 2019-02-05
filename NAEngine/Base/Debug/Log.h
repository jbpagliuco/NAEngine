#pragma once

#include <stdarg.h>

namespace na
{
	void LogInfo(const char *filter, const char *format, ...);
	void LogInfoV(const char *filter, const char *format, va_list args);
	void LogError(const char *filter, const char *format, ...);
	void LogErrorV(const char *filter, const char *format, va_list args);
}