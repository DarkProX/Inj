#include <iostream>
#include <Windows.h>

int main()
{
	// �������� ���� � ���� dll
	LPCSTR DllPath = "C:\Users\��\source\repos\DllForInj\DllForInj\Dll.cpp";

	// �������� ������� �������
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, 17344);

	// � ������� �������� ������� ������ ��� ���� dll
	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, strlen(DllPath) + 1,
		MEM_COMMIT, PAGE_READWRITE);

	// ��������� ���� � ������ � ������� ��������
	WriteProcessMemory(hProcess, pDllPath, (LPVOID)DllPath,
		strlen(DllPath) + 1, 0);

	// ������ ��������� ����� � ������� ��������
	// �������� dll
	HANDLE hLoadThread = CreateRemoteThread(hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
			"LoadLibraryA"), pDllPath, 0, 0);

	
	WaitForSingleObject(hLoadThread, INFINITE);

	std::cout << "Dll path allocated at: " << std::hex << pDllPath << std::endl;
	std::cin.get();

	// ���������� ������, ���������� ��� dll
	VirtualFreeEx(hProcess, pDllPath, strlen(DllPath) + 1, MEM_RELEASE);

	return 0;
}