#include<iostream>
#include<Windows.h>
using namespace std;
CRITICAL_SECTION* ghCriticalSection = new CRITICAL_SECTION[5];

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	int chopstick1 = (int)lpParameter;
	int chopstick2 = (int)lpParameter-1;
	int counter = 1; 
	if (chopstick1 == 0) {
		chopstick2 = 4;
	}
	while (counter<10) {
		EnterCriticalSection(&ghCriticalSection[chopstick1]);
		BOOL success =	TryEnterCriticalSection(&ghCriticalSection[chopstick2]);
		if (success)
		{
			printf("philosof  %d is eating\n", chopstick1);
			counter++;
			LeaveCriticalSection(&ghCriticalSection[chopstick2]);
		}
		LeaveCriticalSection(&ghCriticalSection[chopstick1]);
	}

	return 0;
}
int main()
	{
	clock_t start = clock();
	
	HANDLE* Thread = new HANDLE[5];
	for (int i = 0; i < 5; i++) {
		InitializeCriticalSection(&ghCriticalSection[i]);
		Thread[i] = CreateThread(
			NULL,
			0,
			ThreadProc,
			(LPVOID)i,
			0,
			NULL
		);
	}
	WaitForMultipleObjects(
		5,
		Thread,
		true,
		INFINITE
	);
	for (int i = 0; i < 5; i++) {
		CloseHandle(Thread[i]);
		DeleteCriticalSection(&ghCriticalSection[i]);

	}
	clock_t end = clock();
	cout << "the time is  "<<(float)(end-start)/CLOCKS_PER_SEC << endl;
	return 0;
}



