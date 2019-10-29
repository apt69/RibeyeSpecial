#pragma once

void * GetImportB(const char * szDll, const char * szFunc);
bool GetImportA(HANDLE hProc, const char * szDll, const char * szFunc, void * &pOut);
bool GetImportA(HINSTANCE hDllEx, const char * szDll, const char * szFunc, void * &pOut);
HINSTANCE GetModuleHandleExA(HANDLE hProc, const char * szDll);

#ifdef _WIN64
bool GetProcAddressA_WOW64(HANDLE hProc, HINSTANCE hDll, const char * szFunc, void * &pOut);
HINSTANCE GetModuleHandleExA_WOW64(HANDLE hProc, const char * szDll);
#endif