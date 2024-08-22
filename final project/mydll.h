#pragma once
#include<Windows.h>
#ifdef DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
extern "C"
{
	DECLDIR int hook(PCSTR func_to_hook, PCSTR DLL_to_hook, DWORD new_func_address);
	DECLDIR void ShowMsg();
}
