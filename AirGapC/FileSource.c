#include "FileSource.h"

extern void(*FileSource_ReportData)(short *data);
extern void FileSource_Work();

void FileSource_Work()
{
	short data[512];

	FileSource_ReportData(data);
}
