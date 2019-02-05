#pragma once

#include <fstream>

#define MAX_FILEPATH_SIZE 260
#define MAX_FILEPATH_LENGTH (MAX_FILEPATH_SIZE - 1)

namespace na
{
	class File
	{
	public:
		File(const char *filename, int mode = std::ios::in);
		virtual ~File();

		bool PeekChar(char *s);

		bool ReadChar(char *s);
		bool ReadLine(char *s, size_t n);
		bool ReadBytes(void *buffer, size_t n);

		const char* GetFileExt()const;

	private:
		char mFilename[MAX_FILEPATH_SIZE];
		std::ifstream mFile;
	};

	template <size_t N>
	void OpenFileDialog(char(&buffer)[N], const char *defaultPath = "");


	const char* GetFileExt(const char *filename);
	bool IsAbsoluteFilePath(const char *path);
}

#include "File.inl"