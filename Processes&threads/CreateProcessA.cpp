#include<iostream>
#include<Windows.h>
using namespace std;
#define EXE_FILENAME "C:\\Users\\yairb\\source\\repos\\osProject3\\Debug\\osProject3.exe"
#define PROCESS_ARG "8"
int main()
{
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	CHAR exe_filename[] = EXE_FILENAME;
	CHAR process_arg[] = PROCESS_ARG;
	INT size = strlen(exe_filename) + strlen(process_arg) + 2;
	PCHAR param = (PCHAR)malloc(size * sizeof(CHAR));
	sprintf_s(param, size, "%s %s", exe_filename, process_arg);
	bool check = CreateProcessA(NULL,
		param, // Command line
		NULL, // Process handle not inheritable
		NULL, // Thread handle not inheritable
		FALSE, // Set handle inheritance to FALSE
		0, // No creation flags
		NULL, // Use parent's environment block
		NULL, // Use parent's starting directory
		&si, // Pointer to STARTUPINFO structure
		&pi); // Pointer to PROCESS_INFORMATION structure
	if (check == FALSE) {
		cout << "Thread Creation Failed & Error NO --> " << GetLastError() << endl;
		return 0;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	free(param);
	return 0;
}