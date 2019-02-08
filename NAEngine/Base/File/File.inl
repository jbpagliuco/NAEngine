namespace na
{
	template <typename T>
	bool File::Read(T &out)
	{
		return ReadBytes((char*)&out, sizeof(T));
	}

	template <typename T>
	bool File::Write(const T &val)
	{
		return WriteBytes(reinterpret_cast<char*>((void*)&val), sizeof(T));
	}
}

#if defined(_NA_TOOLS) && defined(_NA_WIN32)

#include "OS/NAWindows.h"
#include <shobjidl.h>

namespace na
{
	template <size_t N>
	void OpenFileDialog(char(&buffer)[N], const char *defaultPath)
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
}

#endif // defined(NA_TOOLS) && defined(_NA_WIN32)