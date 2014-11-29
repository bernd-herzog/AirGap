#include "FileSource.h"
#include "DataTypes.h"

extern void(*FileSource_ReportData)(BoolPackage);
extern void FileSource_Work();

void FileSource_Work()
{
	BoolPackage ret;
	
	FileSource_ReportData(ret);
}
