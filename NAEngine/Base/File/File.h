#pragma once

#include <fstream>

#define MAX_FILEPATH_SIZE 260
#define MAX_FILEPATH_LENGTH (MAX_FILEPATH_SIZE - 1)

namespace na
{
	struct File
	{
	public:
		File(const char *filename, int mode);
		virtual ~File();

		operator bool()const;

		const char* GetFileExt()const;

		template <typename T>
		bool Read(T &out);
		bool ReadBytes(char *buf, size_t n);
		bool ReadLine(char *buf, size_t n);
		
		template <typename T>
		bool Write(const T &val);
		bool WriteBytes(const char *buf, size_t n);

	public:
		char mFilename[MAX_FILEPATH_SIZE];
		std::fstream mFile;

	private:
		void Open(const char *filename, int mode);
	};

	// Dot (.) not included.
	const char* GetFileExt(const char *filename);

	bool IsAbsoluteFilePath(const char *path);

#if defined(_NA_TOOLS)
	template <size_t N>
	void OpenFileDialog(char(&buffer)[N], const char *defaultPath = "");
#endif
}

#include "File.inl"