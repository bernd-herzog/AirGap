#pragma once

#include "DataTypes.h"

void(*Depacketizer_ReportData)(UCharPackage);
void Depacketizer_OnData(BoolPackage);
void Depacketizer_Init();
int Depacketizer_Stable_Connection();