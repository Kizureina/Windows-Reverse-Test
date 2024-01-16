#include <iostream>
#include <Windows.h>
#include "detours.h"
#pragma comment(lib, "detours.lib")

STARTUPINFOW si;
PROCESS_INFORMATION pi;


int main()
{
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	LPCSTR dllName[] = { "hook.dll", "hook1.dll" };

	DetourCreateProcessWithDllsW(L"dll_injection_test.exe", NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi, 1, dllName, NULL);
	ResumeThread(pi.hThread);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}