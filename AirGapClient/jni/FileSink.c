#include "FileSink.h"
#include <stdio.h>

extern void FileSink_OnData(UCharPackage);

void FileSink_OnData(UCharPackage packet)
{
	char *filename = "sink.txt";
	FILE *o_file;
	//o_file = fopen(filename, "ab");


	for (int i = 0; i < packet.count; i++)
	{
		unsigned char byte = packet.data[i];

		printf("%c", byte);

		//fwrite(&byte, sizeof(char), 1, o_file);
	}

	printf("\n");

	//fclose(o_file);
}
