#pragma once

#include <string>

namespace na
{
	// Returns true if the string *s* starts with *prefix*
	bool StartsWith(const char *prefix, const char *s);

	// Removes the whitespace in the string.
	// Includes: " ", "\n", "\t", "\r".
	// s: The string to remove whitespace from. This will be overwritten.
	void RemoveWhitespace(char *s);

	// Removes the whitespace at the beginning and end of the string.
	// Includes: " ", "\n", "\t", "\r".
	// s: The string to remove whitespace from. This will be overwritten.
	void RemoveCapWhitespace(char *s);

	// Removes the whitespace at the beginning of the string.
	// Includes: " ", "\n", "\t", "\r".
	// s: The string to remove whitespace from. This will be overwritten.
	void RemoveLeadingWhitespace(char *s);

	// Removes the whitespace at the end of the string.
	// Includes: " ", "\n", "\t", "\r".
	// s: The string to remove whitespace from. This will be overwritten.
	void RemoveTrailingWhitespace(char *s);

	// Changes the string to be all lower case.
	// @param s - The string to de-capitalize.
	void ToLowerCase(char *s);

	// Changes the string to be all upper case.
	// @param s - The string to capitalize.
	void ToUpperCase(char *s);

	void ToWideChar(wchar_t *buf, size_t n, const char *s);

	bool StartsWith(const std::string &s, const std::string &prefix);
	bool EndsWith(const std::string &s, const std::string &suffix);
	void LTrim(std::string &s);
	void RTrim(std::string &s);
	void Trim(std::string &s);
}