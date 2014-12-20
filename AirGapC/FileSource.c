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

	char *text = (char *)malloc(fsize);
	fread(text, fsize, 1, i_file);
	fclose(i_file);

	ret.count = 8 * fsize;

	ret.data = (bool *)malloc(ret.count * sizeof(bool));


	static int k = 0;

	for (int i = 0; i < fsize ; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (k++ % 2 == 0){
			//if (((text[i] >> j) & 0x01) == 0x01){
				ret.data[i * 8 + (7 - j)] = true;
			}else{
				ret.data[i * 8 + (7-j)] = false;
			}
		}
	}

	free(text);

	while (true)
	{
		FileSource_ReportData(ret);
	}
	
	free(ret.data);
}
