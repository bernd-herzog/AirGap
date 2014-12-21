#pragma once

#include "DataTypes.h"

void(*Depacketizer_ReportData)(BoolPackage);
void Depacketizer_OnData(BoolPackage);
void Depacketizer_Init();
