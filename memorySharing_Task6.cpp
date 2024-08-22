#include<iostream>
#include<Windows.h>
using namespace std;
#define FILENAME "C:\\Users\\yairb\\gibrish.bin"
#define SEARCH_LETTER "A"
int main()
{
	//C:\Users\yairb\source\repos\osProject6\Debug\osProject6.exe
	HANDLE x=OpenFileMappingA(
		FILE_MAP_ALL_ACCESS,
	true,
		"myfile"
		);
	// open an existing file for reading
	if (x == NULL) {

		HANDLE hFile;
		LPCSTR pFileName = FILENAME;
		hFile = CreateFileA(
			pFileName, // file name
			GENERIC_READ | GENERIC_WRITE, // access type
			0, // other processes can't share
			NULL, // security
			OPEN_EXISTING, // open only if file exists
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		// create file mapping object
		if (hFile == NULL) {
			return 0;
		}
		HANDLE hMapFile;
		hMapFile = CreateFileMappingA(
			hFile, // file handle
			NULL, // default security
			PAGE_READWRITE, // read access
			0, // maximum object size (high-order
			// DWORD)
			256, // maximum object size (low-order DWORD)
			// 0 means map the whole file
			"myfile"); // name of mapping object, in case we
			// want to share it
		if (hMapFile == NULL) {
			return 0;
		}
		// read the file, one page at a time
		LPSTR pBuf;
		pBuf = (LPSTR)MapViewOfFile(
			hMapFile, // handle to map object
			FILE_MAP_ALL_ACCESS, // read/write permission
			0, // start point (upper word)
			0, // start point (lower word)
			256); // how many bytes to read
		while (true) {
			if (pBuf[0] != 'A') {

			}
			else {
				break;
			}

		}
	}
	else
	{
		LPSTR pBuf = (LPSTR)MapViewOfFile(
			x, // handle to map object
			FILE_MAP_ALL_ACCESS, // read/write permission
			0, // start point (upper word)
			0, // start point (lower word)
			256); // how many bytes to read
		pBuf[0] = 'A';
	}
	
		return 0;
	}
	
		
	
	







// "C:\\Users\\yairb\\Downloads\\gibrish.bin"