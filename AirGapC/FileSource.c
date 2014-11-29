#include "FileSource.h"
#include "DataTypes.h"

extern void(*FileSource_ReportData)(BoolPackage);
extern void FileSource_Work();

void FileSource_Work()
{
	BoolPackage ret;
	ret.count = 0;
	ret.data = 0;
	
	FileSource_ReportData(ret);
}
