#include <Windows.h>


VOID WINAPI NewThreadFunc(LPVOID lpParamate)
{
	MessageBox(NULL, L"NewThreadFunc", NULL, NULL);
}


extern "C" VOID __declspec(dllexport) MyFunc()
{

}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			MessageBoxW(NULL, L"DLL_PROCESS_ATTACH", NULL, NULL);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NewThreadFunc, NULL, NULL, NULL);
			break;
		case DLL_THREAD_ATTACH:
			MessageBoxW(NULL, L"DLL_THREAD_ATTACH", NULL, NULL);
			break;
		case DLL_THREAD_DETACH:
			MessageBoxW(NULL, L"DLL_THREAD_DETACH", NULL, NULL);
			break;
		case DLL_PROCESS_DETACH:
			MessageBoxW(NULL, L"DLL_PROCESS_DETACH", NULL, NULL);
			break;
	}
	return TRUE;
}