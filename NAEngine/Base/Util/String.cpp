#include "String.h"

#include <algorithm>
#include <cstring>
#include <cctype>
#include <codecvt>
#include <locale>

#include "Debug/Assert.h"
#include "OS/OSWin32.h"

namespace na
{
	bool StartsWith(const char *prefix, const char *s)
	{
		size_t prefixLen = strlen(prefix);
		size_t sLen = strlen(s);

		return (prefixLen < sLen) ? strncmp(prefix, s, prefixLen) == 0 : false;
	}

	void RemoveWhitespace(char *s)
	{
		char *i = s;
		char *sani = s;

		while (*i != 0) {
			*sani = *i++;
			if (!std::isspace(*sani)) {
				++sani;
			}
		}

		*sani = 0;
	}

	void RemoveCapWhitespace(char *s)
	{
		RemoveLeadingWhitespace(s);
		RemoveTrailingWhitespace(s);
	}

	void RemoveLeadingWhitespace(char *s)
	{
		char *i = s;
		char *sani = s;

		while (*i != 0) {
			*sani = *i++;
			if (!std::isspace(*sani)) {
				++sani;
				break;
			}
		}

		// Now just copy the rest of the string
		while (*i != 0) {
			*sani++ = *i++;
		}

		*sani = 0;
	}

	void RemoveTrailingWhitespace(char *s)
	{
		char *i = s;
		char *lastNonWhitespaceCharacter = nullptr;
		// Go to the last character
		while (*i != 0) {
			if (!std::isspace(*i)) {
				lastNonWhitespaceCharacter = i;
			}

			++i;
		}

		if (lastNonWhitespaceCharacter) {
			*(lastNonWhitespaceCharacter + 1) = 0;
		}
		else if (s != nullptr) {
			// String was all whitespace
			s[0] = 0;
		}
	}

	void ToLowerCase(char *s)
	{
		char *i = s;
		while (*i != 0) {
			*i = tolower(*i);
			++i;
		}
	}

	void ToUpperCase(char *s)
	{
		char *i = s;
		while (*i != 0) {
			*i = toupper(*i);
			++i;
		}
	}

	void ToWideChar(wchar_t *buf, size_t n, const char *s)
	{
		const size_t len = strlen(s);

		MultiByteToWideChar(CP_UTF8, 0, s, (int)len, buf, (int)n);
		buf[min(n - 1, len)] = 0;
	}


	bool StartsWith(const std::string &s, const std::string &prefix)
	{
		return s.size() >= prefix.size() && 0 == s.compare(0, prefix.size(), prefix);
	}

	bool EndsWith(const std::string &s, const std::string &suffix)
	{
		return s.size() >= suffix.size() && 0 == s.compare(s.size() - suffix.size(), suffix.size(), suffix);
	}

	void LTrim(std::string &s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
		}));
	}

	void RTrim(std::string &s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	void Trim(std::string &s)
	{
		LTrim(s);
		RTrim(s);
	}

	std::wstring ToWideString(const std::string &s)
	{
		wchar_t buffer[1024];
		
		const int num = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, STATIC_ARRAY_SIZE(buffer));
		NA_ASSERT_RETURN_VALUE(num > 0, L"", "Failed to convert string \"%s\" to UTF8. Error code: 0x%X", s.c_str(), GetLastError());
		
		return std::wstring(buffer);
	}
}