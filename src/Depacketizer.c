#include "Depacketizer.h"
#include "DataTypes.h"
#include <stdlib.h>
#include <math.h>
#include "agmath.h"
#include <string.h>
#include "ReedSolomon.h"

extern void(*Depacketizer_ReportData)(UCharPackage);
extern void Depacketizer_OnData(BoolPackage);
extern void Depacketizer_Init();
bool IsLastBufferPreamble();

bool *Depacketizer_lastBuffer = 0;
int Depacketizer_BitsToRead = 0;
UCharPackage Depacketizer_ret;
int Depacketizer_inPacket = 0;

void Depacketizer_OnData(BoolPackage data)
{
	int positionInPacket = 0;

	do{
		for (int i = 0; i < 8 * ag_PREAMBLESIZE - 1; i++)
		{
			Depacketizer_lastBuffer[i] = Depacketizer_lastBuffer[i + 1];
		}

		Depacketizer_lastBuffer[15] = data.data[positionInPacket];

		if (Depacketizer_BitsToRead == 0 && IsLastBufferPreamble() == true)
		{
			Depacketizer_BitsToRead = (ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE) * 8;
			memset(Depacketizer_ret.data, 0, ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE);
			Depacketizer_inPacket = 0;
			positionInPacket++;
		}
		else if (Depacketizer_BitsToRead == 0)
		{
			positionInPacket++;
			continue;
		}

		for (; Depacketizer_BitsToRead > 0 && positionInPacket < data.count; Depacketizer_BitsToRead--, positionInPacket++, Depacketizer_inPacket++)
		{
			int j = Depacketizer_inPacket % 8;
			unsigned char bit = data.data[positionInPacket] == true ? 1 : 0;
			Depacketizer_ret.data[Depacketizer_inPacket / 8] |= bit << (7 - j);
		}

		if (Depacketizer_BitsToRead == 0)
		{
			rs_correct_msg(Depacketizer_ret.data);
			Depacketizer_ReportData(Depacketizer_ret);
		}

	} while (positionInPacket < data.count);
}

bool IsLastBufferPreamble()
{
	if (   Depacketizer_lastBuffer[0] != true
		|| Depacketizer_lastBuffer[1] != true
		|| Depacketizer_lastBuffer[2] != false
		|| Depacketizer_lastBuffer[3] != false
		|| Depacketizer_lastBuffer[4] != false
		|| Depacketizer_lastBuffer[5] != true
		|| Depacketizer_lastBuffer[6] != false
		|| Depacketizer_lastBuffer[7] != false
		|| Depacketizer_lastBuffer[8] != false
		|| Depacketizer_lastBuffer[9] != false
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

	Depacketizer_ret.count = (ag_PACKETSIZE);
	Depacketizer_ret.data = (unsigned char *)malloc(ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE * sizeof(unsigned char));
}
