#include <Windows.h>
#include <cstdio>
#include <time.h>
#include <intrin.h>
#include <tchar.h>
#include "ImportHandler.h"
#include "xorstr.hpp"
#include "Anti.h"

typedef NTSTATUS(WINAPI *pNtDelayExecution)(IN BOOLEAN, IN PLARGE_INTEGER);
typedef VOID(WINAPI * pSleep)(IN DWORD);
typedef VOID(WINAPI * pGSI)(IN LPSYSTEM_INFO);
typedef BOOL(WINAPI * pMemoryStatus)(IN LPMEMORYSTATUSEX);
typedef DWORD(WINAPI * pNtQuerySystemInformation)(IN PVOID, IN DWORD, IN PDWORD);
typedef BOOL(WINAPI * pGetCursorPos)(IN LPPOINT);

#define LOWORD(_dw)     ((WORD)(((DWORD_PTR)(_dw)) & 0xffff))
#define HIWORD(_dw)     ((WORD)((((DWORD_PTR)(_dw)) >> 16) & 0xffff))
#define LODWORD(_qw)    ((DWORD)(_qw))
#define HIDWORD(_qw)    ((DWORD)(((_qw) >> 32) & 0xffffffff))

int prime(int loopCount)
{
	int prime_temp = 0;
	for (int num = 1; num <= 500000; num++) {
		int count = 0;
		for (int i = 2; i <= num / 2; i++) {
			if (num%i == 0) {
				count++;
				break;
			}
		}

		if (count == 0 && num != 1)
		{
			prime_temp = num;
		}
	}
	return prime_temp;
}

int ntSleep(int milisecond)
{
	LARGE_INTEGER DelayInterval;
	
	char* NtApi = xorstr("NtDelayExecution").crypt_get(); //{ 'N', 't', 'D', 'e', 'l', 'a', 'y', 'E', 'x', 'e', 'c', 'u', 't', 'i', 'o', 'n', '\0'};

	char* ModuleN = xorstr("ntdll.dll").crypt_get();

	auto NtDelayExecution = static_cast<pNtDelayExecution>(GetImportB(ModuleN, NtApi));

	int delayInMillis_divided = milisecond / 1000;

	LONGLONG llDelay = delayInMillis_divided * 10000LL;
	DelayInterval.QuadPart = -llDelay;

	for (int i = 0; i < 1000; i++) {
		NtDelayExecution(FALSE, &DelayInterval);
	}

	return FALSE;
}

bool chk_acceleratedsleep()
{
	const char* NtApi = xorstr("Sleep").crypt_get(); //{ 'S', 'l', 'e', 'e','p', '\0' };

	const char* ModuleN = xorstr("KERNEL32.DLL").crypt_get();

	auto SleepFunc = static_cast<pSleep>(GetImportB(ModuleN, NtApi));

	auto start_s = clock();

	// use normal sleep to make sure sleep is being messed with
	SleepFunc(10000);

	auto end_s = clock();

	auto dif = difftime(end_s, start_s);

	if (dif < 10000.f)
	{
		return true;
	}

	return false;
}

bool chk_mouse()
{
	POINT Cursor_One;

	char * NtApi = xorstr("GetCursorPos").crypt_get();

	char* ModuleN = xorstr("USER32.dll").crypt_get();

	auto GetCursorFunc = static_cast<pGetCursorPos>(GetImportB(ModuleN, NtApi));

	if (GetCursorFunc(&Cursor_One)) // Not sure if ONEPARAM_ROUTINE_GETCURSORPOS is static across all windows
	{
		ntSleep(5000);

		POINT Cursor_Two;
		if (GetCursorFunc(&Cursor_Two))
		{
			if (Cursor_One.x != Cursor_Two.x || Cursor_One.y != Cursor_Two.y)
				return false;
		}
	}
	return true;
}


bool chk_dbg()
{
#if _WIN64
	return *(BYTE*)(__readgsqword(0x60) + 2);

#elif _WIN32
	return *(BYTE*)(__readfsdword(0x30) + 2);
#endif
}

bool chk_ram()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);

	const char* NtApi = xorstr("GlobalMemoryStatusEx").crypt_get();	//{ 'G', 'l', 'o', 'b', 'a', 'l', 'M', 'e', 'm', 'o', 'r', 'y', 'S', 't', 'a', 't', 'u', 's', 'E', 'x', '\0' };

	char* ModuleN = xorstr("USER32.dll").crypt_get();

	auto mGMS = static_cast<pMemoryStatus>(GetImportB(ModuleN, NtApi));

	mGMS(&status);

	if(status.ullTotalPhys >= 1500000000)
		return false;

	return true;
}

bool chk_cores()
{
	SYSTEM_INFO   systeminfo;
	unsigned long bytesreturned;

	const char* NtApi = xorstr("GetSystemInfo").crypt_get();    //{ 'G', 'e', 't', 'S', 'y', 's', 't', 'e', 'm', 'I', 'n', 'f', 'o', '\0' };

	const char* ModuleN = xorstr("KERNELBASE.dll").crypt_get();   //{ 'K', 'E', 'R', 'N', 'E', 'L', 'B', 'A', 'S', 'E','.', 'd', 'l', 'l', '\0' };

	auto GSIfunc = static_cast<pGSI>(GetImportB(ModuleN, NtApi));

	GSIfunc(&systeminfo);

	if (systeminfo.dwNumberOfProcessors > 1)
		return false;

	return true;
}

//const wchar_t* szBlacklistedHypervisors[] = {
//	(xorstr(L"KVMKVMKVM\0\0\0").crypt_get()),	/* KVM */
//	(xorstr(L"Microsoft Hv").crypt_get()),		/* Microsoft Hyper-V or Windows Virtual PC */
//	(xorstr(L"VMwareVMware").crypt_get()),		/* VMware */
//	(xorstr(L"XenVMMXenVMM").crypt_get()),		/* Xen */
//	(xorstr(L"prl hyperv  ").crypt_get()),		/* Parallels */
//	(xorstr(L"VBoxVBoxVBox").crypt_get()),		/* VirtualBox */
//};
WCHAR* ascii_to_wide_str(CHAR* lpMultiByteStr)
{

	/* Get the required size */
	INT iNumChars = MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, -1, NULL, 0);

	/* Allocate new wide string */

	SIZE_T Size = (1 + iNumChars) * sizeof(WCHAR);

	WCHAR* lpWideCharStr = reinterpret_cast<WCHAR*>(malloc(Size));

	if (lpWideCharStr) {
		SecureZeroMemory(lpWideCharStr, Size);
		/* Do the conversion */
		iNumChars = MultiByteToWideChar(CP_ACP, 0, lpMultiByteStr, -1, lpWideCharStr, iNumChars);
	}
	return lpWideCharStr;
}

BOOL cpuid_hypervisor_vendor()
{
	INT CPUInfo[4] = { -1 };
	CHAR szHypervisorVendor[0x40];
	WCHAR* pwszConverted;

	BOOL bResult = FALSE;

	const TCHAR* szBlacklistedHypervisors[] = {
		_T("KVMKVMKVM\0\0\0"),	/* KVM */
		_T("Microsoft Hv"),		/* Microsoft Hyper-V or Windows Virtual PC */
		_T("VMwareVMware"),		/* VMware */
		_T("XenVMMXenVMM"),		/* Xen */
		_T("prl hyperv  "),		/* Parallels */
		_T("VBoxVBoxVBox"),		/* VirtualBox */
	};
	WORD dwlength = sizeof(szBlacklistedHypervisors) / sizeof(szBlacklistedHypervisors[0]);

	// __cpuid with an InfoType argument of 0 returns the number of
	// valid Ids in CPUInfo[0] and the CPU identification string in
	// the other three array elements. The CPU identification string is
	// not in linear order. The code below arranges the information 
	// in a human readable form.
	__cpuid(CPUInfo, 0x40000000);
	memset(szHypervisorVendor, 0, sizeof(szHypervisorVendor));
	memcpy(szHypervisorVendor, CPUInfo + 1, 12);

	for (int i = 0; i < dwlength; i++)
	{
		pwszConverted = ascii_to_wide_str(szHypervisorVendor);
		if (pwszConverted) {

			bResult = (_tcscmp(pwszConverted, szBlacklistedHypervisors[i]) == 0);



			free(pwszConverted);

			if (bResult)
				return TRUE;
		}
	}

	return FALSE;
}
NTSTATUS ChangePageProtection(IN HANDLE process, IN OUT PVOID baseAddress, IN OUT PULONG size, IN ULONG newProtection, OUT PULONG oldProtection) {
	typedef NTSTATUS(WINAPI * tNtPVM)(IN HANDLE ProcessHandle, IN OUT PVOID BaseAddress, IN OUT PULONG NumberOfBytesToProtect, IN ULONG NewAccessProtection, OUT PULONG OldAccessProtection);

	tNtPVM ntProtectVirtualMemory = nullptr;

	// Initialize address of ntdll.NtProtectVirtualMemory.
	if (ntProtectVirtualMemory == nullptr) {

		const char* NtApi = xorstr("NtProtectVirtualMemory").crypt_get(); //{ 'N', 't', 'P', 'r', 'o', 't', 'e', 'c', 't', 'V', 'i', 'r', 't', 'u', 'a', 'l', 'M', 'e', 'm', 'o', 'r', 'y', '\0' };

		char* ModuleN = xorstr("ntdll.dll").crypt_get(); //{ 'n', 't', 'd', 'l', 'l', '.', 'd', 'l', 'l', '\0' };

		ntProtectVirtualMemory = static_cast<tNtPVM>(GetImportB(ModuleN, NtApi));

		if (ntProtectVirtualMemory == nullptr) return STATUS_ENTRYPOINT_NOT_FOUND;
	}

	return ntProtectVirtualMemory(process, &baseAddress, (PULONG)&size, newProtection, oldProtection);
}