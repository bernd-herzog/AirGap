#include "MemorySink.h"
#include <stdio.h>
#include <stdlib.h>

extern void MemorySink_OnData(UCharPackage);
extern char *MemorySink_GetMemory();
extern void MemorySink_Clear();

int MemorySink_memsize = 0;
int MemorySink_len = 0;
char *MemorySink_data = 0;

void MemorySink_OnData(UCharPackage packet)
{
	if (MemorySink_memsize < packet.count+1)
	{
		MemorySink_data = (char *) realloc(MemorySink_data, (packet.count+1) * sizeof(char));
		MemorySink_memsize = packet.count+1;
	}

	for (int i = 0; i < packet.count; i++)
	{
		unsigned char byte = packet.data[i];

		MemorySink_data[i] = byte;
	}
	MemorySink_len = packet.count;
	MemorySink_data[MemorySink_len] = 0;
}

void MemorySink_Clear()
{
	if (MemorySink_data != 0)
	{
		MemorySink_len = 0;
		MemorySink_data[MemorySink_len] = 0;
	}
}

char *MemorySink_GetMemory()
{
	return MemorySink_data;
}
