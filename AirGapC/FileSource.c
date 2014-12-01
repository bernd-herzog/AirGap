#include "FileSource.h"
#include "DataTypes.h"
#include <stdbool.h>
#include <stdlib.h>

extern void(*FileSource_ReportData)(BoolPackage);
extern void FileSource_Work();

void FileSource_Work()
{
	BoolPackage ret;
	const char *filename = "testdata.out";

	//TODO: datei öffnen
	//TODO: länge auf anzahl der bits in der datei setzen
	ret.count = 0;

	ret.data = (bool *)malloc(ret.count * sizeof(bool));

	//TODO: inhalt der datei in <ret.data> bit für bit ablegen

	FileSource_ReportData(ret);
	free(ret.data);
}
