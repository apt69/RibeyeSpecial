#pragma once

// Loop x amount of time
int prime(int loopCount = 550000);

// Sleep for x amount of seconds in short bursts
int ntSleep(int milisecond);

// make sure sleep is not being hooked
bool chk_acceleratedsleep();

// make sure there are mouse movement
bool chk_mouse();

// check debugger
bool chk_dbg();

// check RAM
bool chk_ram();

// check CPU cores
bool chk_cores();

// check hypervisor
BOOL cpuid_hypervisor_vendor();

NTSTATUS ChangePageProtection(IN HANDLE process, IN OUT PVOID baseAddress, IN OUT PULONG size, IN ULONG newProtection, OUT PULONG oldProtection);