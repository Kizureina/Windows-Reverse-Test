#include <iostream>
#include <Windows.h>
#include "detours.h"
#pragma comment(lib, "detours.lib")

typedef int (WINAPI* pMessageBoxW)(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCWSTR lpText,
    _In_opt_ LPCWSTR lpCaption,
    _In_ UINT uType);
pMessageBoxW orgMessageBoxW;

int WINAPI myMessageBoxW(
    _In_opt_ HWND hWnd,
    _In_opt_ LPCWSTR lpText,
    _In_opt_ LPCWSTR lpCaption,
    _In_ UINT uType)
{
    lpText = L"Yoruko-test";
    return orgMessageBoxW(hWnd, lpText, lpCaption, uType);
}


int main()
{
    orgMessageBoxW = MessageBoxW;

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)orgMessageBoxW, myMessageBoxW);
    DetourTransactionCommit();

    MessageBoxW(NULL, NULL, NULL, NULL);


    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)orgMessageBoxW, myMessageBoxW);
    DetourTransactionCommit();

    MessageBoxW(NULL, NULL, NULL, NULL);

    std::cout << "Hello World!\n";
}