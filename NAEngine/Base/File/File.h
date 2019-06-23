#pragma once

#include <fstream>

#define MAX_FILEPATH_SIZE 260
#define MAX_FILEPATH_LENGTH (MAX_FILEPATH_SIZE - 1)

namespace na
{
	struct File
	{
	public:
		File(const std::string &filename, int mode);
		virtual ~File();

		operator bool()const;

		std::string GetFileExt()const;

		template <typename T>
		bool Read(T &out);
		bool ReadBytes(std::string &buf);
		bool ReadLine(std::string &buf);
		
		template <typename T>
		bool Write(const T &val);
		bool WriteBytes(const char *buf, size_t n);

	public:
		std::string mFilename;
		std::fstream mFile;

	private:
		void Open(const std::string &filename, int mode);
	};

	// Dot (.) not included.
	std::string GetFileExt(const std::string &filename);

	bool IsAbsoluteFilePath(const std::string &path);

#if defined(_NA_TOOLS)
	void OpenFileDialog(std::string &buf, const std::string &defaultPath = "");
#endif
}

#include "File.inl"