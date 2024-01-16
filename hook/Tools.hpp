#include <Windows.h>
#include <stdio.h>
#include <iostream>

static FILE* steamconsole;


DWORD WINAPI SetConsoleTop(LPVOID lpParameter)
{
	WCHAR consoleTitle[256] = { 0 };

	while (true)
	{
		GetConsoleTitleW(consoleTitle, 256);
		HWND hConsole = FindWindowW(NULL, (LPWSTR)consoleTitle);
		if (hConsole != NULL)
		{
			SetWindowPos(hConsole, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			break;
		}
	}

	return 0;
}


VOID WINAPI SetConsole()
{
	AllocConsole();
	AttachConsole(ATTACH_PARENT_PROCESS);
	freopen_s(&steamconsole, "CONIN$", "r+t", stdin);
	freopen_s(&steamconsole, "CONOUT$", "r+t", stdout);
	SetConsoleTitleW(L"Hijack Test");

	CreateThread(NULL, NULL, SetConsoleTop, NULL, NULL, NULL);

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & ~ENABLE_QUICK_EDIT_MODE);

	std::locale::global(std::locale(""));
}