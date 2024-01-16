#include <iostream>
#include <Windows.h>

int main()
{
	//LoadLibraryW(L"Dll1.dll");
	MessageBoxW(NULL, L"Test", NULL, NULL);

	std::cout << "Hello World!\n";
}