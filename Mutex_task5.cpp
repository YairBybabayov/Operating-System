#include<iostream>
#include<Windows.h>
using namespace std;

int main(int argc,char* argv[])
{
	if (argc != 2) {
		cout << "error";
		return 0;
	}
	HANDLE* hMyMutex = new HANDLE[5];
	for (int i = 0; i < 5; i++) {
		hMyMutex[i] = CreateMutexA(NULL, FALSE, "0" + i);
	}
	int id = atoi(argv[1]);
	int chopstick1 = id;
	int chopstick2 = id - 1;
	int counter = 1;
	if (chopstick1 == 0) {
		chopstick2 = 4;
	}
	while (counter < 3) {
		DWORD waitResult1 = WaitForSingleObject(
			hMyMutex[chopstick1],
			INFINITE); // no time-out interval
		DWORD waitResult2 = WaitForSingleObject(
			hMyMutex[chopstick2],
			0); // zero time-out interval
		if (waitResult2== WAIT_OBJECT_0)
		{
			printf("philosof  %d is eating\n", chopstick1);
			counter++;
			ReleaseMutex(hMyMutex[chopstick2]);
		}
		ReleaseMutex(hMyMutex[chopstick1]);

	}

	return 0;
}



