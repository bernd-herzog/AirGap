#include "FileSource.h"
#include "DataTypes.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

extern void(*FileSource_ReportData)(UCharPackage);
extern void FileSource_Work();

void FileSource_Work()
{
	UCharPackage ret;

	const char *filename = "source.txt";
	FILE *i_file;
	i_file = fopen(filename, "rb");
	fseek(i_file, 0, SEEK_END);
	long fsize = ftell(i_file);
	fseek(i_file, 0, SEEK_SET);

//	unsigned char *text = (unsigned char *)malloc(fsize);
	ret.count = fsize;
	ret.data = (unsigned char *)malloc(ret.count * sizeof(unsigned char));

	fread(ret.data, fsize, 1, i_file);
	fclose(i_file);


	//static int k = 0;

	//for (int i = 0; i < fsize ; i++)
	//{

	//	for (int j = 0; j < 8; j++)
	//	{
	//		if (((text[i] >> j) & 0x01) == 0x01){
	//			ret.data[i * 8 + (7 - j)] = true;
	//		}else{
	//			ret.data[i * 8 + (7-j)] = false;
	//		}
	//	}
	//}

	//free(text);

	while (true)
	{
		FileSource_ReportData(ret);
	}
	
	free(ret.data);
}
