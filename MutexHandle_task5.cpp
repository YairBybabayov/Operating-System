#include<iostream>
#include<Windows.h>
using namespace std; 
#define EXE_FILENAME "C:\\Users\\yairb\\source\\repos\\osProject5\\Debug\\osProject5.exe"
#define NUM_PROCESSES 5
int main()
{
	HANDLE* hMyMutex = new HANDLE[5];
	INT size = strlen(EXE_FILENAME) + 3; // length is increased by 3:
	 // space character- 1 byte,
	 // one digit number- 1 byte,
	 // string NULL terminator- 1 byte
	PCHAR param = (PCHAR)malloc(size * sizeof(CHAR));
	STARTUPINFOA si;
	PROCESS_INFORMATION pi[NUM_PROCESSES];
	for (int i = 0; i < NUM_PROCESSES; i++) {
		 hMyMutex[i] = CreateMutexA(NULL, FALSE, "0"+i);
		sprintf_s(param, size, "%s %d", EXE_FILENAME, i);
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi[i], sizeof(pi[i]));
		CreateProcessA(
			NULL,
			param,
			NULL,
			NULL,
			FALSE,
			0,
			NULL,
			NULL,
			&si,
			&pi[i]);
	}
	// Wait for every process to finish, close everything before return
	
		for (int i = 0; i < NUM_PROCESSES; i++) {
			WaitForSingleObject(pi[i].hProcess, INFINITE);
		}
	ReleaseMutex(hMyMutex);
	for (int i = 0; i < NUM_PROCESSES; i++)
	{
		CloseHandle(pi[i].hProcess);
		CloseHandle(pi[i].hThread);
	}
	return 0;
}
