#include<iostream>
#include<Windows.h>

using namespace std;
DWORD WINAPI ThreadFun(LPVOID pointer) {
	int x = (int)pointer;
	for (int i = 0; i < 1000; i++) {
		printf("Thread %d , var = %d\n", x, i);
		Sleep(50);
	}
	return 0;
}
int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("Incorrect number of arguments\n");
		return 0;
	}

	int size = atoi(argv[1]);
	HANDLE* Thread = new HANDLE[size];
	for (int i = 1; i < size + 1; i++) {

		Thread[i - 1] = CreateThread(
			NULL,
			0,
			ThreadFun,
			(LPVOID)i,
			0,
			NULL
		);
	}
	WaitForMultipleObjects(
		size,
		Thread,
		true,
		INFINITE
	);
	for (int i = 0; i < size; i++) {
		CloseHandle(Thread[i]);
	}
	return 0;

}

