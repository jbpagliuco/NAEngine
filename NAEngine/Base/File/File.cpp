#include "File.h"

#include <experimental/filesystem>
#include <sstream>

#include "Debug/Assert.h"
#include "Util/String.h"

namespace na
{
	File::File(const std::string &filename, int mode)
	{
		Open(filename, mode);

		// If the file is readable, make sure we can open it.
		if ((mode & std::ios::in) != 0) {
			NA_ASSERT(mFile.is_open(), "Unable to open file '%s'", filename.c_str());
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

	std::string File::GetFileExt()const
	{
		return na::GetFileExt(mFilename);
	}

	bool File::ReadBytes(std::string &buf)
	{
		char cBuf[1024];
		const bool success = !!mFile.read(cBuf, 1024);

		buf = cBuf;

		return success;
	}

	bool File::ReadLine(std::string &buf)
	{
		return !!std::getline(mFile, buf);
	}

	bool File::WriteBytes(const char *buf, size_t n)
	{
		return !!mFile.write(buf, n);
	}

	void File::Open(const std::string &filename, int mode)
	{
		if (IsFilePath(filename)) {
			mFilename = filename;
		} else {
			FindFileRecursively(mFilename, "data", filename);
		}

		mFile.open(mFilename, mode);
	}

	std::string GetFileExt(const std::string &filename)
	{
		const size_t lastDot = filename.find_last_of(".");
		if (lastDot == filename.npos) {
			return "";
		}

		return filename.substr(lastDot + 1);
	}


	bool IsFilePath(const std::string &filename)
	{
		return filename.find('\\') != filename.npos;
	}

	bool FileExists(const std::string &filename)
	{
		if (IsFilePath(filename)) {
			return std::experimental::filesystem::exists(filename);
		} else {
			std::string temp;
			return FindFileRecursively(temp, "data", filename);
		}
	}
	
	bool FindFileRecursively(std::string &out, const std::string &directory, const std::string &filename)
	{
		const std::string suffix = "/" + filename;

		for (auto &entry : std::experimental::filesystem::recursive_directory_iterator(directory)) {
			const std::string f = entry.path().generic_string();
			if (EndsWith(f, suffix)) {
				out = f;

#if defined(_NA_WIN32)
				std::replace(out.begin(), out.end(), '/', '\\');
#endif
				return true;
			}
		}

		return false;
	}

	bool GetFullFilepath(std::string &out, const std::string &filename)
	{
		if (IsFilePath(filename)) {
			if (FileExists(filename)) {
				out = filename;
				return true;
			}

			return false;

		} else {
			return FindFileRecursively(out, "data", filename);
		}
	}

	bool IsAbsoluteFilePath(const std::string &path)
	{
#if defined(_NA_WIN32)
		return path.length() > 1 && path[1] == ':';
#endif
	}



#if defined(_NA_TOOLS) && defined(_NA_WIN32)

#include "OS/NAWindows.h"
#include <shobjidl.h>

	void OpenFileDialog(std::string &buf, const std::string &defaultPath)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
			COINIT_DISABLE_OLE1DDE);
		if (SUCCEEDED(hr)) {
			IFileOpenDialog *pFileOpen;

			// Create the FileOpenDialog object.
			hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
				IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

			if (SUCCEEDED(hr)) {
				// Show the Open dialog box.
				hr = pFileOpen->Show(NULL);

				// Get the file name from the dialog box.
				if (SUCCEEDED(hr)) {
					IShellItem *pItem;
					hr = pFileOpen->GetResult(&pItem);
					if (SUCCEEDED(hr)) {
						PWSTR pszFilePath;
						hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

						// Display the file name to the user.
						if (SUCCEEDED(hr)) {
							WideCharToMultiByte(CP_ACP, 0, pszFilePath, MAX_FILEPATH_LENGTH, buffer, N, 0, 0);
							CoTaskMemFree(pszFilePath);
						}
						pItem->Release();
					}
				}
				pFileOpen->Release();
			}
			CoUninitialize();
		}
	}

#endif // defined(NA_TOOLS) && defined(_NA_WIN32)

}