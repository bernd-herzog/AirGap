#include "MemorySink.h"
#include <stdio.h>
#include <stdlib.h>

extern void MemorySink_OnData(UCharPackage);

int MemorySink_memsize = 0;
extern int MemorySink_len;
extern char *MemorySink_data;

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
}
