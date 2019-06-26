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
		bool ReadBytes(char *buf, size_t size);
		bool ReadLine(std::string &buf);
		
		template <typename T>
		bool Write(const T &val);
		bool WriteBytes(const char *buf, size_t n);

		// Returns the file size in bytes.
		size_t GetFileSize();

	public:
		std::string mFilename;
		std::fstream mFile;

	private:
		void Open(const std::string &filename, int mode);
	};

	// Dot (.) not included.
	std::string GetFileExt(const std::string &filename);

	bool IsFilePath(const std::string &filename);
	bool FileExists(const std::string &filename);
	bool FindFileRecursively(std::string &out, const std::string &directory, const std::string &filename);
	bool GetFullFilepath(std::string &out, const std::string &filename);

	bool IsAbsoluteFilePath(const std::string &path);

#if defined(_NA_TOOLS)
	void OpenFileDialog(std::string &buf, const std::string &defaultPath = "");
#endif
}

#include "File.inl"