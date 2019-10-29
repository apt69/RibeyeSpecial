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
