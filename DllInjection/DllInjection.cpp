#include <Windows.h>
#include <iostream>
using namespace std;
#define LIBRARY "C:\\Users\\yairb\\source\\repos\\osProject8\\x64\\Release\\osProject8.dll"
typedef void(*PFUNC)(void);
int main()
{
	// Get full path of DLL to inject
	DWORD pathLen = GetFullPathNameA(LIBRARY, NULL, NULL, NULL);
	// Get LoadLibrary function address –
	// the address doesn't change at remote process
	PVOID addrLoadLibrary =
		(PVOID)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),"LoadLibraryA");
			// Open remote process
			HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS,NULL,11272);
	// Get a pointer to memory location in remote process,
	// big enough to store DLL path
	PVOID memAddr = (PVOID)VirtualAllocEx(proc,NULL,pathLen, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (NULL == memAddr) {
		cout << GetLastError() << endl;
		return 0;
	}
	// Write DLL name to remote process memory
	BOOL check = WriteProcessMemory(proc, memAddr, LIBRARY, pathLen,NULL);
	if (0 == check) {
		cout << GetLastError() << endl;
		return 0;
	}
	// Open remote thread, while executing LoadLibrary
	// with parameter DLL name, will trigger DLLMain
	HANDLE hRemote = CreateRemoteThread(proc, NULL, 0, (LPTHREAD_START_ROUTINE)addrLoadLibrary, (LPVOID)memAddr, 0, NULL);
	if (NULL == hRemote) {
		cout << GetLastError() << endl;
		return 0;
	}
	WaitForSingleObject(hRemote, INFINITE);
	check = CloseHandle(hRemote);
	return 0;
}