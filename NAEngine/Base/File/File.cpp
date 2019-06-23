#include "File.h"

#include <sstream>

#include "Debug/Assert.h"

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
		// Look for the file in these places:
		// 1. Straight up
		// 2. Under the data folder

		// 1
		mFilename = filename;
		mFile.open(mFilename, mode);
		if (mFile.is_open()) {
			return;
		}

		// 2
		mFilename = "data\\" + filename;
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