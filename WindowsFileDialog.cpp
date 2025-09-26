#include "pch.h"  // Include the precompiled header
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>  // For folder picker

extern "C" __declspec(dllexport) const wchar_t* OpenWindowsFile(const wchar_t* initialDir = NULL)
{
	// Save the current working directory
	wchar_t currentDir[260];
	GetCurrentDirectoryW(260, currentDir);

	OPENFILENAMEW ofn;  // Use the wide version of OPENFILENAME
	static wchar_t szFile[260] = { 0 };  // Make static for valid return after function exits
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = initialDir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&ofn) == TRUE) {
		SetCurrentDirectoryW(currentDir);
		return ofn.lpstrFile;
	}

	SetCurrentDirectoryW(currentDir);
	return L"";
}

extern "C" __declspec(dllexport) const wchar_t* OpenFileWithExtension(const wchar_t* filter, const wchar_t* initialDir = NULL)
{
	// Save the current working directory
	wchar_t currentDir[260];
	GetCurrentDirectoryW(260, currentDir);

	static wchar_t szFile[260] = { 0 };
	ZeroMemory(&szFile, sizeof(szFile));

	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = initialDir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&ofn) == TRUE)
	{
		SetCurrentDirectoryW(currentDir);
		return ofn.lpstrFile;
	}

	SetCurrentDirectoryW(currentDir);
	return L"";
}

extern "C" __declspec(dllexport) const wchar_t* OpenFolderDialog(const wchar_t* initialDir = NULL)
{
	static wchar_t folderPath[260] = { 0 };
	IFileDialog* pfd = NULL;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(hr))
		return L"";

	hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	if (FAILED(hr) || !pfd)
	{
		CoUninitialize();
		return L"";
	}

	// Set options to select folders only
	DWORD dwOptions;
	pfd->GetOptions(&dwOptions);
	pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

	// Set initial folder if provided
	if (initialDir)
	{
		IShellItem* psi = NULL;
		hr = SHCreateItemFromParsingName(initialDir, NULL, IID_PPV_ARGS(&psi));
		if (SUCCEEDED(hr))
		{
			pfd->SetFolder(psi);
			psi->Release();
		}
	}

	hr = pfd->Show(NULL);
	if (SUCCEEDED(hr))
	{
		IShellItem* psiResult = NULL;
		hr = pfd->GetResult(&psiResult);
		if (SUCCEEDED(hr))
		{
			PWSTR pszFolderPath = NULL;
			hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
			if (SUCCEEDED(hr))
			{
				wcsncpy_s(folderPath, pszFolderPath, sizeof(folderPath) / sizeof(wchar_t) - 1);
				CoTaskMemFree(pszFolderPath);
			}
			psiResult->Release();
		}
	}

	pfd->Release();
	CoUninitialize();
	return folderPath;
}