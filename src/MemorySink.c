#include "MemorySink.h"
#include "agmath.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern void MemorySink_OnData(UCharPackage);
extern char *MemorySink_GetMemory();
extern void MemorySink_Clear();

int MemorySink_memsize = 0;
int MemorySink_len = 0;
#define mem_SIZE (256 * (ag_PACKETSIZE-1))
char MemorySink_data[mem_SIZE + 1];
int MemorySink_pos = 0;

void MemorySink_OnData(UCharPackage packet)
{
	unsigned char block = packet.data[0];

	for (int i = 1; i < packet.count; i++)
	{
		unsigned char byte = packet.data[i];

		if (isalnum(byte) || byte == ' ' || byte == '!' || byte == '\r' || byte == '\n' || byte == '\t' || byte > 0x20 && byte < 0x7e)
			MemorySink_data[block * (ag_PACKETSIZE-1) + i] = byte;
		else
			MemorySink_data[block * (ag_PACKETSIZE-1) + i] = ' ';
	}
}

void MemorySink_Init()
{
	memset(MemorySink_data, ' ', mem_SIZE);
	MemorySink_data[mem_SIZE] = '\0';
}

char *MemorySink_GetMemory()
{
	return MemorySink_data;
}
