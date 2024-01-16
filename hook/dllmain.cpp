#include <Windows.h>
#include "detours.h"
#pragma comment(lib, "detours.lib")
#include "Tools.hpp"

VOID __declspec(dllexport) MyFunc()
{

};

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


VOID Hook()
{
    orgMessageBoxW = MessageBoxW;

    std::cout << "Hook start!" << std::endl;

    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)orgMessageBoxW, myMessageBoxW);
    DetourTransactionCommit();

    std::cout << "Hook end" << std::endl;

}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SetConsole();
        Hook();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

