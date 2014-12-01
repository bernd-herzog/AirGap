#include "FileSink.h"

extern void FileSink_OnData(BoolPackage);

void FileSink_OnData(BoolPackage packet)
{
	char *filename = "testdaten.txt";

	//TODO: datei öffnen

	for (int i = 0; i < packet.count; i+=8)
	{

		for (int j = 0; j < 8; j++)
		{
			unsigned char byte = 0;
			byte |= packet.data[i + j] < j;

			//TODO: byte in datei schreiben
		}
	}

	//TODO: datei schließen
}
