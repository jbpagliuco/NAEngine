#pragma once

#include <stdint.h>

#if defined(_NA_DEBUG)

#define DEBUG_BREAK __debugbreak

namespace na
{
	bool PrintAssertMessage(const char *assert, const char *file, int line);
	bool PrintAssertMessage(const char *assert, const char *file, int line, const char *format, ...);

	void PrintErrorMessage(const char *assert, const char *file, int line);
	void PrintErrorMessage(const char *assert, const char *file, int ilne, const char *format, ...);
}

#define NA_PRINT_ASSERT_MESSAGE(cond, ...) na::PrintAssertMessage(#cond, __FILE__, __LINE__, __VA_ARGS__)

#define NA_ASSERT(cond, ...)											\
	if (!(cond)) {														\
		if (NA_PRINT_ASSERT_MESSAGE(cond, __VA_ARGS__)) {				\
			DEBUG_BREAK();												\
		}																\
	}																	

#define NA_ASSERT_RETURN(cond, ...)        	    						\
	if (!(cond)) {														\
		if (NA_PRINT_ASSERT_MESSAGE(cond, __VA_ARGS__)) {				\
			DEBUG_BREAK();												\
		}																\
		return; 														\
	}																	

#define NA_ASSERT_RETURN_VALUE(cond, rv, ...)							\
	if (!(cond)) {														\
		if (NA_PRINT_ASSERT_MESSAGE(cond, __VA_ARGS__)) {				\
			DEBUG_BREAK();												\
		}																\
		return rv;														\
	}																	

#define NA_ASSERT_CONTINUE(cond, ...)									\
	if (!(cond)) {														\
		if (NA_PRINT_ASSERT_MESSAGE(cond, __VA_ARGS__)) {				\
			DEBUG_BREAK();												\
		}																\
		continue;														\
	}																	

#define NA_FATAL_ERROR(cond, ...)										\
	if (!(cond)) {														\
		na::PrintErrorMessage(#cond, __FILE__, __LINE__, __VA_ARGS__);	\
		exit(EXIT_FAILURE);												\
	}																	

#else

#define NA_ASSERT(...)
#define NA_ASSERT_RETURN(cond, ...)					if (!(cond)) { return; }
#define NA_ASSERT_RETURN_VALUE(cond, rv, ...)		if (!(cond)) { return rv; }
#define NA_ASSERT_CONTINUE(cond, ...)				if (!(cond)) { continue; }
#define NA_ASSERT_FATAL_ERROR(cond, ...)			if (!(cond)) { exit(EXIT_FAILURE); }

#endif // defined(_NA_DEBUG)