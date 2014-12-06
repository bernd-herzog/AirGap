#include "FileSource.h"
#include "DataTypes.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

extern void(*FileSource_ReportData)(BoolPackage);
extern void FileSource_Work();

void FileSource_Work()
{
	BoolPackage ret;
	const char *filename = "testdata.out";
	FILE *i_file;
	i_file = fopen(filename, "rb");
	fseek(i_file, 0, SEEK_END);
	long fsize = ftell(i_file);
	fseek(i_file, 0, SEEK_SET);

	char *text = malloc(fsize +1);
	fread(text, fsize, 1, i_file);
	fclose(i_file);

	text[fsize] = 0;

	//TODO: datei öffnen
	//TODO: länge auf anzahl der bits in der datei setzen
	ret.count = 8 * fsize;

	ret.data = (bool *)malloc(ret.count * sizeof(bool));

	//TODO: inhalt der datei in <ret.data> bit für bit ablegen
	for (int i = 0; i < fsize ; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (((text[i] >> j) & 0x00000001) == true){
				ret.data[i * 8 + (7-j)] = true;
			}else{
				ret.data[i * 8 + (7-j)] = false;
			}
		}
	}

	/* To check if first char is correct:
	ret.data[0];
	ret.data[1];
	ret.data[2];
	ret.data[3];
	ret.data[4];
	ret.data[5];
	ret.data[6];
	ret.data[7];*/


	FileSource_ReportData(ret);
	free(ret.data);
}
