#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
using namespace std;

#define DLL_PATH "C:\\Users\\yairb\\source\\repos\\osFinalProject1\\Release\\osFinalProject1.dll"

typedef void(*PFUNC)(void);

int main()
{
	DWORD processId = 0;

	PROCESSENTRY32 Procees32;
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Procees32.dwSize = sizeof(PROCESSENTRY32);
	Process32First(snap, &Procees32);

	if (Process32First(snap, &Procees32) == TRUE)
		while (Process32Next(snap, &Procees32) == TRUE)
			if (lstrcmpW((LPCWSTR)Procees32.szExeFile, (LPCWSTR)L"notepad.exe") == 0)
				processId = Procees32.th32ProcessID;

	CloseHandle(snap);
	cout << "porcees id " << processId << endl;
	DWORD pathLen = GetFullPathNameA(DLL_PATH, NULL, NULL, NULL);
	if (NULL == pathLen) {
		cout << " ERROR  in  GetFullPathNameA " << GetLastError() << endl;
		return 0;
	}

	PVOID addrLoadLibrary = (PVOID)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");
	if (NULL == addrLoadLibrary) {
		cout << " ERROR  in  GetProcAddress " << GetLastError() << endl;
		return 0;
	}

	HANDLE	proc = OpenProcess(PROCESS_ALL_ACCESS, NULL, processId);
	if (NULL == proc) {
		cout << " ERROR  in  OpenProcess " << GetLastError() << endl;
		return 0;
	}
	PVOID memAddr = (PVOID)VirtualAllocEx(proc, NULL, pathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (NULL == memAddr) {
		cout << " ERROR   in VirtualAllocEx  " << GetLastError() << endl;
		return 0;
	}

	bool check = WriteProcessMemory(proc, memAddr, DLL_PATH, pathLen, NULL);


	if (0 == check) {
		cout << " ERROR   in  WriteProcessMemory " << GetLastError() << endl;
		return 0;
	}

	HANDLE hRemote = CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)addrLoadLibrary, (LPVOID)memAddr, 0, NULL);

	if (NULL == hRemote) {
		cout << " ERROR  in  CreateRemoteThread " << GetLastError() << endl;
		return 0;
	}

	WaitForSingleObject(hRemote, INFINITE);
	check = CloseHandle(hRemote);
	return 0;
}