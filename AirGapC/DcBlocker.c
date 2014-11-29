#include "DcBlocker.h"

extern void(*DcBlocker_ReportData)(ShortPackage);
extern void DcBlocker_OnData(ShortPackage);

void DcBlocker_OnData(ShortPackage data){
	//Nicht implementieren!
	DcBlocker_ReportData(data);
}
