#include "Depacketizer.h"
#include "DataTypes.h"
#include <stdlib.h>
#include <math.h>
#include "agmath.h"

extern void(*Depacketizer_ReportData)(BoolPackage);
extern void Depacketizer_OnData(BoolPackage);
extern void Depacketizer_Init();
bool IsLastBufferPreamble();

bool *Depacketizer_lastBuffer = 0;
int Depacketizer_BitsToRead = 0;
BoolPackage Depacketizer_ret;
int Depacketizer_inPacket = 0;


void Depacketizer_OnData(BoolPackage data)
{
	int positionInPacket = 0;
	
	do{
		for (int i = 0; i < 8 * ag_ERRORCORRECTIONSIZE - 1; i++)
		{
			Depacketizer_lastBuffer[i] = Depacketizer_lastBuffer[i+1];
		}

		Depacketizer_lastBuffer[15] = data.data[positionInPacket];

		if (Depacketizer_BitsToRead == 0 && IsLastBufferPreamble() == true)
		{
			Depacketizer_BitsToRead = (ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE) * 8;
			Depacketizer_inPacket = 0;
			positionInPacket++;
		}
		else if (Depacketizer_BitsToRead == 0)
		{
			positionInPacket++;
			continue;
		}


		//read from packet
		for (; Depacketizer_BitsToRead > 0 && positionInPacket < data.count; Depacketizer_BitsToRead--, positionInPacket++, Depacketizer_inPacket++)
		{
			Depacketizer_ret.data[Depacketizer_inPacket] = data.data[positionInPacket];
		}

		if (Depacketizer_BitsToRead == 0)
		{
			Depacketizer_ReportData(Depacketizer_ret);
		}

	} 
	while (positionInPacket < data.count);
}

bool IsLastBufferPreamble()
{
	if (
		   Depacketizer_lastBuffer[ 0] != true
		|| Depacketizer_lastBuffer[ 1] != true
		|| Depacketizer_lastBuffer[ 2] != false
		|| Depacketizer_lastBuffer[ 3] != false
		|| Depacketizer_lastBuffer[ 4] != false
		|| Depacketizer_lastBuffer[ 5] != true
		|| Depacketizer_lastBuffer[ 6] != false
		|| Depacketizer_lastBuffer[ 7] != false
		|| Depacketizer_lastBuffer[ 8] != false
		|| Depacketizer_lastBuffer[ 9] != false
		|| Depacketizer_lastBuffer[10] != false
		|| Depacketizer_lastBuffer[11] != true
		|| Depacketizer_lastBuffer[12] != false
		|| Depacketizer_lastBuffer[13] != false
		|| Depacketizer_lastBuffer[14] != false
		|| Depacketizer_lastBuffer[15] != true)

		return false;

	return true;

}

void Depacketizer_Init()
{
	Depacketizer_lastBuffer = (bool *)calloc(8 * ag_PREAMBLESIZE, sizeof(bool));

	Depacketizer_ret.count = (ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE) * 8;
	Depacketizer_ret.data = (bool *)malloc(Depacketizer_ret.count * sizeof(bool));
}