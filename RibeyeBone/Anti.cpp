#include <Windows.h>
#include <cstdio>
#include <time.h>
#include "ImportHandler.h"
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
	
	constexpr const char NtApi[] = { 'N', 't', 'D', 'e', 'l', 'a', 'y', 'E', 'x', 'e', 'c', 'u', 't', 'i', 'o', 'n', '\0'};

	constexpr const char ModuleN[] = { 'n', 't', 'd', 'l', 'l', '.', 'd', 'l', 'l', '\0'};

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
	constexpr const char NtApi[] = { 'S', 'l', 'e', 'e','p', '\0' };

	constexpr const char ModuleN[] = { 'K', 'E', 'R', 'N', 'E', 'L', '3', '2', '.', 'D', 'L', 'L', '\0' };

	auto SleepFunc = static_cast<pSleep>(GetImportB(ModuleN, NtApi));

	auto start_s = clock();

	// use normal sleep to make sure sleep is being messed with
	SleepFunc(10000);

	auto end_s = clock();

	auto dif = difftime(end_s, start_s);

	printf("%fms elapsed sec\n", dif);

	if (dif < 10000.f)
	{
		return true;
	}

	return false;
}

bool chk_mouse()
{
	POINT Cursor_One;

	constexpr const char NtApi[] = { 'G', 'e', 't', 'C','u', 'r', 's', 'o', 'r', 'P', 'o', 's', '\0' };

	constexpr const char ModuleN[] = { 'U', 'S', 'E', 'R', '3', '2', '.', 'd', 'l', 'l', '\0' };

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

	constexpr const char NtApi[] = { 'G', 'l', 'o', 'b', 'a', 'l', 'M', 'e', 'm', 'o', 'r', 'y', 'S', 't', 'a', 't', 'u', 's', 'E', 'x', '\0' };

	constexpr const char ModuleN[] = { 'K', 'E', 'R', 'N', 'E', 'L', '3', '2', '.', 'D', 'L', 'L', '\0' };

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

	constexpr const char NtApi[] = { 'G', 'e', 't', 'S', 'y', 's', 't', 'e', 'm', 'I', 'n', 'f', 'o', '\0' };

	constexpr const char ModuleN[] = { 'K', 'E', 'R', 'N', 'E', 'L', 'B', 'A', 'S', 'E','.', 'd', 'l', 'l', '\0' };

	auto GSIfunc = static_cast<pGSI>(GetImportB(ModuleN, NtApi));

	GSIfunc(&systeminfo);

	printf("\n[i] Processor Count: %d\n", systeminfo.dwNumberOfProcessors);

	if (systeminfo.dwNumberOfProcessors > 1)
		return false;

	return true;
}
