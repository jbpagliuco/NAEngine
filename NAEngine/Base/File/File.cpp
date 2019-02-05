#include "File.h"

#include <sstream>

#include "Debug/Debug.h"

namespace na
{
	File::File(const char *filename, int mode)
	{
		strncpy_s(mFilename, filename, MAX_FILEPATH_LENGTH);

		mode = (~std::ios::out) & mode;
		mode |= std::ios::in;

		mFile.open(filename, mode);
		NA_ASSERT(mFile.is_open(), "File '%s' not found", filename);
	}

	File::~File()
	{
		mFile.close();
	}

	bool File::PeekChar(char *s)
	{
		if (mFile.eof()) {
			return false;
		}

		*s = mFile.peek();
		return true;
	}

	bool File::ReadChar(char *s)
	{
		if (mFile.eof()) {
			return false;
		}

		*s = mFile.get();
		return true;
	}

	bool File::ReadLine(char *s, size_t n)
	{
		std::string lineData;
		if (std::getline(mFile, lineData)) {
			strncpy_s(s, n - 1, lineData.c_str(), _TRUNCATE);
			return true;
		}

		return false;
	}

	bool File::ReadBytes(void *buffer, size_t n)
	{
		return !!mFile.read((char*)buffer, n);
	}

	const char* File::GetFileExt()const
	{
		return na::GetFileExt(mFilename);
	}



	const char* GetFileExt(const char *filename)
	{
		const char *i = filename;
		const char *lastDot = nullptr;
		while (*i != 0) {
			if (*i == '.') {
				lastDot = i;
			}

			++i;
		}

		if (lastDot == nullptr) {
			return "";
		}

		return lastDot + 1;
	}

	bool IsAbsoluteFilePath(const char *path)
	{
#if defined(_NA_WIN32)
		return strlen(path) > 1 && path[1] == ':';
#endif
	}
}