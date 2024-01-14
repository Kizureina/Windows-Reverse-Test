#include <iostream>
#include <Windows.h>
#include "test.hpp"

void WINAPI myTest(int a, int b, int c)
{
}

void __declspec(naked) My_fun()
{
	__asm mov eax, eax
	__asm ret
}

DWORD orgAddr = 0x0041247E; // 0x0041243E;
DWORD retAddr = 0x00412483;

void __declspec(naked) MyHook()
{
	__asm
	{
		pushad
		pushfd
	}

	MessageBoxW(NULL, L"Hello!", L"this is a test box", MB_OK);

	// 注意push和pop顺序
	__asm
	{
		popfd
		popad
	}

	__asm push 0xF1F4
	__asm jmp retAddr
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

int main()
{
	WriteHookCode((DWORD)MyHook, orgAddr);
	RunMethod1();

	std::cout << "Hello, world!\n";
	return 0;
}