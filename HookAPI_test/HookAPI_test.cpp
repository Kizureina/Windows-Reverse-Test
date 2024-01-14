#include <iostream>
#include <Windows.h>

DWORD orgMsgBoxAddr;
DWORD retMsgBoxAddr;

typedef int (WINAPI* pMessageBoxTimeoutW)(
	IN HWND hWnd,
	IN LPCWSTR lppText,
	IN LPCWSTR lpCaption,
	IN UINT uType,
	IN WORD wLanguageId,
	IN DWORD dwMilliseconds);

pMessageBoxTimeoutW orgMessageBoxTimeoutW;

DWORD GetProcAddr(LPCWSTR dllName, LPCSTR procName)
{
	// 大小写都可以
	HMODULE hDll = LoadLibraryW(dllName);
	if (hDll == NULL)
	{
		return FALSE;
	}
	FARPROC addr = GetProcAddress(hDll, procName);
	if (addr == NULL)
	{
		return NULL;
	}
	FreeLibrary(hDll);

	return (DWORD)addr;
}

void WriteHookCode(DWORD tarAddr, DWORD orgAddr)
{
	DWORD oldProtect = 0;
	VirtualProtect((LPVOID)orgAddr, 0x5, PAGE_EXECUTE_READWRITE, &oldProtect);

	DWORD rvaAddr = tarAddr - orgAddr - 0x5;

	BYTE code[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 };

	memcpy(&code[1], &rvaAddr, 0x4);

	// 将hook代码写入目标地址
	memcpy((void*)orgAddr, code, 0x5);
}

VOID __declspec(naked) MyMessageBoxTimeout(_In_opt_ HWND hWnd,
	_In_opt_ LPCWSTR lpText,
	_In_opt_ LPCWSTR lpCaption,
	_In_ UINT uType)
{
	__asm push ebp
	__asm mov ebp, esp

	__asm
	{
		pushad
		pushfd
	}
	lpText = L"MyHook";

	__asm
	{
		popfd
		popad
		jmp retMsgBoxAddr
	}
}

VOID Hook()
{
	retMsgBoxAddr = (DWORD)orgMessageBoxTimeoutW + 0x5;

	WriteHookCode((DWORD)MyMessageBoxTimeout, (DWORD)orgMessageBoxTimeoutW);
}

int main()
{
	orgMessageBoxTimeoutW = (pMessageBoxTimeoutW)GetProcAddr(L"user32.dll", "MessageBoxTimeoutW");

	Hook();
	orgMessageBoxTimeoutW(NULL, L"hello", L"test", NULL, NULL, 5000);

	MessageBoxA(NULL, "Yoruko", "hello-A", NULL);
	MessageBoxW(NULL, L"Yoruko", L"hello-W", NULL);
	std::cout << "Hello World!\n";
}