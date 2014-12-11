#include "FileSink.h"
#include <stdio.h>

extern void FileSink_OnData(BoolPackage);

void FileSink_OnData(BoolPackage packet)
{
	char *filename = "testdaten.txt";
	FILE *o_file;
	o_file = fopen(filename, "ab");

	for (int i = 0; i < packet.count; i+=8)
	{
		unsigned char byte = 0;

		for (int j = 0; j < 8; j++)
		{
			byte |= packet.data[i + j] < j;
		}

		fwrite(&byte, sizeof(char), 1, o_file);
	}

	fclose(o_file);
}
