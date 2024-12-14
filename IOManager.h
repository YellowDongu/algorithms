#pragma once
#include <Windows.h>
#include <commdlg.h> // => 파일 탐색기
#include <shlwapi.h> // => PathRemoveFileSpec() 경로 편집
#pragma comment(lib, "Shlwapi.lib")
#include <shlobj.h> // => selectFolderPath()
#include <afxstr.h> // => CString
#include <string>
#include <functional>
#include <stdlib.h>
#include <list>

#include <fstream>

class IOManager
{
private:
	IOManager(void) = default;
	~IOManager(void) = default;
public:
	static std::wstring selectSaveFilePath(void);
	static std::wstring selectLoadFilePath(void);
	static std::wstring selectFolderPath(void);
	static std::list<std::pair<std::wstring, std::wstring>> findAllFiles(const std::wstring& folderPath);

	static HRESULT load(const std::wstring& path, std::function<void(HANDLE)> function);
	static HRESULT save(const std::wstring& path, std::function<void(HANDLE)> function);


	template <typename T>
	static HRESULT serialize(HANDLE hFile, const T& value)
	{
		static_assert(!std::is_same<T, char*>::value, "문자열은 사용할 수 없습니다.");
		if (!WriteFile(hFile, &value, sizeof(T), NULL, NULL))
			return E_FAIL;

		return S_OK;
	}
	template <>
	static HRESULT serialize(HANDLE hFile, const CString& value)
	{
		int length = value.GetLength();
		if (!WriteFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;
#ifdef _UNICODE
		if (!WriteFile(hFile, value.GetString(), length * sizeof(wchar_t), NULL, NULL))
			return E_FAIL;
#elif
		if (!WriteFile(hFile, value.GetString(), length * sizeof(char), NULL, NULL))
			return E_FAIL;
#endif
		return S_OK;
	}
	template <>
	static HRESULT serialize(HANDLE hFile, const std::string& value)
	{
		int length = value.size();
		if (!WriteFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;
		if (!WriteFile(hFile, value.c_str(), length * sizeof(char), NULL, NULL))
			return E_FAIL;

		return S_OK;
	}
	template <>
	static HRESULT serialize(HANDLE hFile, const std::wstring& value)
	{
		int length = value.size();
		if (!WriteFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;
		if (!WriteFile(hFile, value.c_str(), length * sizeof(wchar_t), NULL, NULL))
			return E_FAIL;

		return S_OK;
	}

	template <typename T>
	static HRESULT deserialize(HANDLE hFile, T& value)
	{
		static_assert(!std::is_same<T, char*>::value, "문자열은 사용할 수 없습니다.");
		DWARD readAmount = 0;
		if (!ReadFile(hFile, &value, sizeof(T), &readAmount, NULL))
			return E_FAIL;
		if (readAmount == 0) return E_FAIL;

		return S_OK;
	}
	template <>
	static HRESULT deserialize(HANDLE hFile, CString& value)
	{
		int length;
		if (!ReadFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;

		if (length < 0) return E_FAIL;
		if (length == 0) return S_OK;

#ifdef _UNICODE
			if (!ReadFile(hFile, value.GetBuffer(length), length * sizeof(wchar_t), NULL, NULL))
				return E_FAIL;
#elif
		if (!ReadFile(hFile, value.GetBuffer(length), length * sizeof(char), NULL, NULL))
			return E_FAIL;
#endif
			value.ReleaseBuffer(length);
		return S_OK;
	}
	template <>
	static HRESULT deserialize(HANDLE hFile, std::string& value)
	{
		int length;
		if (!ReadFile(hFile, &length, sizeof(int), NULL, NULL))
			return E_FAIL;

		if (length < 0) return E_FAIL;
		if (length == 0) return S_OK;

		value.resize(length);
		if (!ReadFile(hFile, &value[0], length * sizeof(char), NULL, NULL))
			return E_FAIL;

		return S_OK;
	}
	template <>
	static HRESULT deserialize(HANDLE hFile, std::wstring& value)
	{
		int length;
		if (!ReadFile(hFile, &length, sizeof(length), NULL, NULL))
			return E_FAIL;
		if (length < 0) return E_FAIL;
		if (length == 0) return S_OK;

		value.resize(length);
		if (!ReadFile(hFile, &value[0], length * sizeof(wchar_t), NULL, NULL))
			return E_FAIL;

		return S_OK;

		//#include <tchar.h> // tchar로 쓰는 방법
		//	TCHAR* buffer = new TCHAR[length + 1];  // 널 문자 공간을 추가로 확보
		//	if (ReadFile(hFile, buffer, length * sizeof(TCHAR), NULL, NULL))
		//	{
		//		buffer[length] = _T('\0');
		//		value = std::to_string(*buffer); => std::string용
		// 	    CString value = buffer; => cstring용
		//	}
		//	delete[] buffer;
	}

private:
};


std::wstring IOManager::selectSaveFilePath(void)
{
	OPENFILENAME ofn;
	wchar_t szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);	
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.lpstrFilter = L"All File(*.*)|*.*||";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;


	wchar_t initialDirectory[260] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	ofn.lpstrInitialDir = initialDirectory;

	if (GetSaveFileName(&ofn))
	{
		return std::wstring(ofn.lpstrFile);
	}
	return std::wstring();
}

std::wstring IOManager::selectLoadFilePath(void)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	wchar_t szFile[260] = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.lpstrFilter = L"All File(*.*)|*.*||";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	wchar_t initialDirectory[260] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	//PathRemoveExtension(initialDirectory); => 확장자명 제거(shlwapi.h)
	PathRemoveExtension(initialDirectory);
	ofn.lpstrInitialDir = initialDirectory;

	if (GetOpenFileName(&ofn))
	{
		return std::wstring(ofn.lpstrFile);
	}
	return std::wstring();
}

std::wstring IOManager::selectFolderPath(void)
{
	BROWSEINFO bi = { 0 };
	bi.lpfn = NULL;
	bi.lpszTitle = L"Folder Selection";

	wchar_t initialDirectory[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	ITEMIDLIST* pidlRoot = nullptr;
	if (SUCCEEDED(SHParseDisplayName(initialDirectory, NULL, &pidlRoot, 0, NULL)))
		bi.pidlRoot = pidlRoot; // 초기 경로 설정
	else
		bi.pidlRoot = NULL;

	LPITEMIDLIST itemIDList = SHBrowseForFolder(&bi);

	if (itemIDList != 0)
	{
		wchar_t path[MAX_PATH];
		if (!SHGetPathFromIDList(itemIDList, path))
		{
			return std::wstring();
		}

		CoTaskMemFree(itemIDList);
		return std::wstring(path);
	}
	return std::wstring();
}

//쓰는 방법(람다함수)
//HRESULT res = IOManager::save(IOManager::selectSaveFilePath(), [](HANDLE handle)->void{
//		serialize(handle, value); => 클래스에 맞게 쓰면 됨
//		serialize(handle, value); });
HRESULT IOManager::load(const std::wstring& path, std::function<void(HANDLE)> function)
{
	if (path == L"")
		return E_FAIL;

	HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return E_FAIL;
	}

	function(hFile);

	CloseHandle(hFile);
	return S_OK;
}

//쓰는 방법(람다함수)
//HRESULT res = IOManager::load(IOManager::selectLoadFilePath(), [](HANDLE handle)->void{
//		deserialize(handle, value);
//		deserialize(handle, value); });
HRESULT IOManager::save(const std::wstring& path, std::function<void(HANDLE)> function)
{
	if (path == L"")
		return E_FAIL;

	HANDLE hFile = CreateFile(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return E_FAIL;
	}

	function(hFile);
	
	CloseHandle(hFile);
	return S_OK;
}


CString ConvertRealtivePath(CString strFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);

	// PathRelativePathTo : szCurDirPath에서 strFullPath로 가는 상대 경로를 구해서
	// 	szRelativePath에 저장하는 함수

	PathRelativePathTo(szRelativePath,
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(),
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

std::list<std::pair<std::wstring, std::wstring>> IOManager::findAllFiles(const std::wstring& folderPath)
{
	std::list<std::pair< std::wstring, std::wstring>> result = {};
	std::list<std::wstring> folderQueue = { folderPath };
	WIN32_FIND_DATA findData;

	while (!folderQueue.empty())
	{
		std::wstring path = folderQueue.front();
		folderQueue.pop_front();

		HANDLE hFind = FindFirstFile((path + L"\\*.*").c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE)
			continue;

		do
		{
			if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
				continue;

			wchar_t fullPath[MAX_PATH];
			PathCombine(fullPath, path.c_str(), findData.cFileName);

			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				folderQueue.push_back(fullPath);
			else
				result.push_back({ findData.cFileName, fullPath });

		}
		while (FindNextFile(hFind, &findData) != 0);

		FindClose(hFind);
	}

	return result;
}



#include <fstream>



class CPPIOManager
{
private:
	CPPIOManager(void) = default;
	~CPPIOManager(void) = default;

public:
	static std::wstring selectSaveFilePath(void);
	static std::wstring selectLoadFilePath(void);
	static std::wstring selectFolderPath(void);
	static std::list<std::pair<std::wstring, std::wstring>> findAllFiles(const std::wstring& folderPath);

	static HRESULT load(const std::wstring& path, std::function<void(std::wstring&)> function, bool binary);
	static HRESULT save(const std::wstring& path, std::function<std::wstring(void)> function, bool overwrite, bool binary);
	
private:

};


std::wstring CPPIOManager::selectSaveFilePath(void)
{
	OPENFILENAME ofn;
	wchar_t szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.lpstrFilter = L"All File(*.*)|*.*||";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;


	wchar_t initialDirectory[260] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	ofn.lpstrInitialDir = initialDirectory;

	if (GetSaveFileName(&ofn))
	{
		return std::wstring(ofn.lpstrFile);
	}
	return std::wstring();
}

std::wstring CPPIOManager::selectLoadFilePath(void)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	wchar_t szFile[260] = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = L"All Files\0*.*\0Text Files\0*.TXT\0";
	ofn.lpstrFilter = L"All File(*.*)|*.*||";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	wchar_t initialDirectory[260] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	//PathRemoveExtension(initialDirectory); => 확장자명 제거(shlwapi.h)
	PathRemoveExtension(initialDirectory);
	ofn.lpstrInitialDir = initialDirectory;

	if (GetOpenFileName(&ofn))
	{
		return std::wstring(ofn.lpstrFile);
	}
	return std::wstring();
}

std::wstring CPPIOManager::selectFolderPath(void)
{
	BROWSEINFO bi = { 0 };
	bi.lpfn = NULL;
	bi.lpszTitle = L"Folder Selection";

	wchar_t initialDirectory[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, initialDirectory);
	//PathRemoveFileSpec(initialDirectory);
	//lstrcat(initialDirectory, L"\\Data");
	ITEMIDLIST* pidlRoot = nullptr;
	if (SUCCEEDED(SHParseDisplayName(initialDirectory, NULL, &pidlRoot, 0, NULL)))
		bi.pidlRoot = pidlRoot; // 초기 경로 설정
	else
		bi.pidlRoot = NULL;

	LPITEMIDLIST itemIDList = SHBrowseForFolder(&bi);

	if (itemIDList != 0)
	{
		wchar_t path[MAX_PATH];
		if (!SHGetPathFromIDList(itemIDList, path))
		{
			return std::wstring();
		}

		CoTaskMemFree(itemIDList);
		return std::wstring(path);
	}
	return std::wstring();
}

HRESULT CPPIOManager::load(const std::wstring& path, std::function<void(std::wstring&)> function, bool binary = false)
{
	std::wifstream file;

	std::ios_base::open_mode mode = std::ios::in;

	if (binary)
		mode |= std::ios::binary;

	file.open(path, mode);
	if (file.fail())
		return E_FAIL;

	std::wstring string;

	std::wstring line;
	while (std::getline(file, line))
	{
		string += line + L"\n";
	}

	//#include <sstream>
	//std::ostringstream oss;
	//oss << file.rdbuf(); // 파일의 모든 내용을 스트림 버퍼를 통해 읽음
	//std::string string = oss.str();
	function(string);

	return S_OK;
}

HRESULT CPPIOManager::save(const std::wstring& path, std::function<std::wstring(void)> function, bool overwrite = true, bool binary = false)
{
	std::wofstream file;

	std::ios_base::open_mode mode = std::ios::out;

	if (binary)
		mode |= std::ios::binary;
	if (overwrite)
		mode |= std::ios::trunc;
	else
		mode |= std::ios::app;

	file.open(path, mode);

	if (file.fail())
		return E_FAIL;

	file << function();
	file.close();

	return S_OK;
}

std::list<std::pair<std::wstring, std::wstring>> CPPIOManager::findAllFiles(const std::wstring& folderPath)
{
	std::list<std::pair< std::wstring, std::wstring>> result = {};
	std::list<std::wstring> folderQueue = { folderPath };
	WIN32_FIND_DATA findData;

	while (!folderQueue.empty())
	{
		std::wstring path = folderQueue.front();
		folderQueue.pop_front();

		HANDLE hFind = FindFirstFile((path + L"\\*.*").c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE)
			continue;

		do
		{
			if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
				continue;

			wchar_t fullPath[MAX_PATH];
			PathCombine(fullPath, path.c_str(), findData.cFileName);

			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				folderQueue.push_back(fullPath);
			else
				result.push_back({ findData.cFileName, fullPath });

		}
		while (FindNextFile(hFind, &findData) != 0);

		FindClose(hFind);
	}

	return result;
}
