#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <fstream>
#include <sstream>
#include <string>
#define EXTENSSION_LENGTH 3

#define EXTENSSION_EXE L"exe"
#define EXTENSSION_TXT L"txt"
#define EXTENSSION_DLL L"dll"

void HandleFile(LPTSTR);
void HandleExe(LPTSTR);
void WatchDirectory(LPTSTR);
void HandleTxt(LPTSTR);

void _tmain(int argc, TCHAR *argv[])
{
	if (argc != 2)
	{
		_tprintf(TEXT("Usage: %s <dir>\n"), argv[0]);
		return;
	}

	WatchDirectory(argv[1]);
}

void WatchDirectory(LPTSTR lpDir)
{


	WCHAR * fullPath;
	FILE_NOTIFY_INFORMATION strFileNotifyInfo[1024];

	HANDLE hDir = CreateFile(	//just getting handler
		lpDir,
		FILE_LIST_DIRECTORY,
		FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL
	);



	while (TRUE)
	{
		// Wait for notification.


		DWORD bytesret = 0;
		BOOL success = ::ReadDirectoryChangesW( //wait until write new file made
			hDir,
			(LPVOID)&strFileNotifyInfo,
			1024,
			FALSE, // monitor children?
			FILE_NOTIFY_CHANGE_LAST_WRITE,
			&bytesret,
			NULL,
			NULL);

		strFileNotifyInfo[0].FileName[strFileNotifyInfo[0].FileNameLength / 2] = '\0';

		//new variable that contain the full path
		fullPath = (WCHAR*) malloc(strFileNotifyInfo[0].FileNameLength		//file name length
								   + (wcslen(lpDir) + 1) * sizeof(WCHAR)		// path length
								   + sizeof(WCHAR));						// for null terminate string

		wsprintf(fullPath, L"%ls\\%ls", lpDir, strFileNotifyInfo[0].FileName);
		HandleFile(fullPath);
		free(fullPath);
	}
}

void HandleFile(LPTSTR lpFile)
{
	// This is where you might place code to refresh your
	// directory listing, but not the subtree because it
	// would not be necessary.

	LPTSTR extenstion = lpFile + wcslen(lpFile) -EXTENSSION_LENGTH;

	if (wcscmp(extenstion, EXTENSSION_EXE) == 0)
	{
		_tprintf(TEXT("Execute file found (%s) \n"), lpFile);

		HandleExe(lpFile);
	}
	else if (wcscmp(extenstion, EXTENSSION_TXT) == 0)
	{
		_tprintf(TEXT("TXT shell file found (%s)\n"), lpFile);
		HandleTxt(lpFile);
	}
	else if (wcscmp(extenstion, EXTENSSION_DLL) == 0)
	{
		_tprintf(TEXT("DLL shell file found (%s)\n"), lpFile);

	}
	else
	_tprintf(TEXT("Unhandeled file found (%s).\n"), lpFile);
}


void HandleTxt(LPTSTR lpFile)
{
	std::wifstream infile(lpFile);	// This is where you might place code to refresh your
	std::wstring line;

	// directory listing, including the subtree.
	while (std::getline(infile, line))
	{

		HINSTANCE  retValue = ShellExecute(0, L"open", L"cmd.exe", line.c_str() , 0, SW_SHOWNORMAL);
		line == line;
	}
	_tprintf(TEXT("Trying to execute exe (%s).\n"), lpFile);
	PROCESS_INFORMATION pi;
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	bool succeed = CreateProcess(lpFile, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

	DWORD error = GetLastError();

}


void HandleExe(LPTSTR lpFile)
{
	// This is where you might place code to refresh your
	// directory listing, including the subtree.

	_tprintf(TEXT("Trying to execute exe.\n"), lpFile);
	PROCESS_INFORMATION pi;
	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	bool succeed = CreateProcess( lpFile, NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	
	DWORD error =  GetLastError();

}
