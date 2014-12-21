#include "FileSink.h"
#include <stdio.h>

extern void FileSink_OnData(BoolPackage);

void FileSink_OnData(BoolPackage packet)
{
	char *filename = "sink.txt";
	FILE *o_file;
	o_file = fopen(filename, "ab");

	for (int k = 0; k < 8; k++)
	{
		for (int i = 0; i < packet.count - 8; i += 8)
		{
			unsigned char byte = 0;

			for (int j = 0; j < 8; j++)
			{
				char bit = packet.data[i + j + k] == true ? 1 : 0;

				byte |= bit << 7-j;
			}

			printf("%c", byte);

			fwrite(&byte, sizeof(char), 1, o_file);
		}

		printf("\t");

	}
	printf("\n");

	fclose(o_file);
}
