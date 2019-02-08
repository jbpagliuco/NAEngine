#include "File.h"

#include <sstream>

#include "Debug/Assert.h"

namespace na
{
	File::File(const char *filename, int mode)
	{
		strncpy_s(mFilename, filename, MAX_FILEPATH_LENGTH);

		mFile.open(filename, mode);
		
		// If the file is readable, make sure we can open it.
		if ((mode & std::ios::in) != 0) {
			NA_ASSERT(mFile.is_open(), "Unable to open file '%s'", filename);
		}
	}

	File::operator bool()const
	{
		return !mFile.eof();
	}

	File::~File()
	{
		mFile.close();
	}

	const char* File::GetFileExt()const
	{
		return na::GetFileExt(mFilename);
	}

	bool File::ReadBytes(char *buf, size_t n)
	{
		return !!mFile.read(buf, n);
	}

	bool File::WriteBytes(const char *buf, size_t n)
	{
		return !!mFile.write(buf, n);
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