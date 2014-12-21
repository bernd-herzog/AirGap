#include "Packetizer.h"
#include <stdlib.h>
#include <math.h>
#include "agmath.h"

extern void(*Packetizer_ReportData)(BoolPackage);
extern void Packetizer_OnData(BoolPackage);
extern void Packetizer_Init();

bool *Packetizer_Buffer = 0;
int Packetizer_InBuffer = 0;
BoolPackage Packetizer_ret;

void Packetizer_OnData(BoolPackage data)
{
	int position = 0;

	while (Packetizer_InBuffer + data.count - position > ag_PACKETSIZE)
	{
		//send a packet
		
		int inPacket = 0;

		//rest of buffer
		for (; inPacket < Packetizer_InBuffer; inPacket++)
		{
			Packetizer_ret.data[inPacket + 16] = Packetizer_Buffer[inPacket];
		}
		Packetizer_InBuffer = 0;
		
		//from stream
		for (; inPacket < ag_PACKETSIZE; inPacket++, position++)
		{
			Packetizer_ret.data[inPacket + 16] = data.data[position];
		}

		//send it
		Packetizer_ReportData(Packetizer_ret);
	}

	if (data.count - position > 0)
	{
		//store rest
		for (int inPacket = 0; data.count - position > 0; inPacket++, position++, Packetizer_InBuffer++)
		{
			Packetizer_Buffer[inPacket] = data.data[position];
		}
	}
}

void Packetizer_Init()
{
	Packetizer_Buffer = (bool *)calloc(ag_PACKETSIZE, sizeof(bool));

	Packetizer_ret.count = ag_PACKETSIZE + 8 * 2;
	Packetizer_ret.data = (bool *)malloc(Packetizer_ret.count * sizeof(bool));

	Packetizer_ret.data[0] = true;
	Packetizer_ret.data[1] = true;
	Packetizer_ret.data[2] = false;
	Packetizer_ret.data[3] = false;
	Packetizer_ret.data[4] = false;
	Packetizer_ret.data[5] = true;
	Packetizer_ret.data[6] = false;
	Packetizer_ret.data[7] = false;
	Packetizer_ret.data[8] = false;
	Packetizer_ret.data[9] = false;
	Packetizer_ret.data[10] = false;
	Packetizer_ret.data[11] = true;
	Packetizer_ret.data[12] = false;
	Packetizer_ret.data[13] = false;
	Packetizer_ret.data[14] = false;
	Packetizer_ret.data[15] = true;
}