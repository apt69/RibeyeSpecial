#include <Windows.h>
#include "Helper.h"


uintptr_t FindPattern(char* base, unsigned int size, char* pattern, char *mask)
{
	size_t patternLength = strlen(mask);

	for (uintptr_t i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (uintptr_t j = 0; j < patternLength; j++)
		{
			if (pattern[j] != *(char*)(base + i + j))
			{
				found = false;
				break; // yeah that's right, stop iterating when pattern is bad.  Looking at you fleep...
			}
		}

		if (found)
		{
			return (uintptr_t)base + i;
		}
	}
	return 0;
}