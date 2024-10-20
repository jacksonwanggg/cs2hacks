#include "memory.h"
#include <iostream>

uintptr_t memory::GetProcID(const wchar_t* process)
{
	HANDLE handle;
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	
	PROCESSENTRY32 proc;
	proc.dwSize = sizeof(PROCESSENTRY32);
	Process32First(handle, &proc);

	do
	{
		if (!wcscmp(process, proc.szExeFile))
		{
			CloseHandle(handle);
			pID = proc.th32ProcessID;
			gHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, pID);
			return proc.th32ProcessID;
		}
	} while (Process32Next(handle, &proc));

	CloseHandle(handle);
	return 0;
}

uintptr_t memory::GetModuleBaseAddress(uintptr_t procID, const wchar_t* module)
{
	std::cout << "Process ID: " << static_cast<unsigned long long>(procID) << std::endl;
	std::wcout << L"Module: " << module << std::endl;

	HANDLE handle;
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	
	// ye 
	if (handle == INVALID_HANDLE_VALUE) {
		auto error = GetLastError();
		std::cerr << "Failed to create snapshot for process. Error: " << error << std::endl;
		if (error == ERROR_ACCESS_DENIED) {
			std::cerr << "Access denied. Try running the program as an administrator." << std::endl;
		}
		return 0;
	}
	MODULEENTRY32 mod;
	mod.dwSize = sizeof(MODULEENTRY32);
	Module32First(handle, &mod);

	do
	{
		if (!wcscmp(module, mod.szModule))
		{
			CloseHandle(handle);
			return (uintptr_t)mod.modBaseAddr;
		}
	} while (Module32Next(handle, &mod));

	CloseHandle(handle);
	return 0;
}

