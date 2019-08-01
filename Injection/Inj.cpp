#include <iostream>
#include <Windows.h>

int main()
{
	// Указываю путь к моей dll
	LPCSTR DllPath = "C:\Users\ПК\source\repos\DllForInj\DllForInj\Dll.cpp";

	// Открываю целевой процесс
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 17344);

	// В целевом процессе выделаю память для моей dll
	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);

	// Указываем путь к адресу в целевом процессе
	WriteProcessMemory(hProcess, pDllPath, (LPVOID)DllPath,
		strlen(DllPath) + 1, 0);

	// Создаю удаленный поток в целевом процессе
	// Загружаю dll
	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
			"LoadLibraryA"), pDllPath, 0, 0);

	
	WaitForSingleObject(hLoadThread, INFINITE);

	std::cout << "Dll path allocated at: " << std::hex << pDllPath << std::endl;
	std::cin.get();

	// Освобождаю память, выделенную для dll
	VirtualFreeEx(hProcess, pDllPath, strlen(DllPath) + 1, MEM_RELEASE);

	return 0;
}