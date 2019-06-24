#include "String.h"

#include <algorithm>
#include <cstring>
#include <cctype>

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
		else {
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
}