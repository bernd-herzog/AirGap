#include "Packetizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "agmath.h"
#include "ReedSolomon.h"

extern void(*Packetizer_ReportData)(BoolPackage);
extern void Packetizer_OnData(UCharPackage);
extern void Packetizer_Init();

unsigned char *Packetizer_Buffer;
unsigned char *Packetizer_ErrorBuffer;
int Packetizer_InBuffer = 0;
BoolPackage Packetizer_ret;

void Packetizer_OnData(UCharPackage data)
{
	// foreach byte
	for (int inputBufferPosition = 0; inputBufferPosition < data.count; inputBufferPosition++)
	{
		Packetizer_Buffer[Packetizer_InBuffer++] = data.data[inputBufferPosition];


		if (Packetizer_InBuffer == ag_PACKETSIZE)
		{
			Packetizer_InBuffer = 0;
			
			Packetizer_Buffer[ag_PACKETSIZE] = 0;
			printf("Sending Packet: '%s'\n", Packetizer_Buffer);

			memcpy(Packetizer_ErrorBuffer, Packetizer_Buffer, ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE);
		
			//add error infos
			rs_encode_msg(Packetizer_ErrorBuffer);

			memcpy(Packetizer_Buffer + ag_PACKETSIZE, Packetizer_ErrorBuffer + ag_PACKETSIZE, ag_ERRORCORRECTIONSIZE);

			//convert to bits
			for (int i = ag_PREAMBLESIZE; i < ag_PREAMBLESIZE + ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					if (((Packetizer_Buffer[i-2] >> j) & 0x01) == 0x01){
						Packetizer_ret.data[i * 8 + (7 - j)] = true;
					}
					else{
						Packetizer_ret.data[i * 8 + (7 - j)] = false;
					}
				}
			}

			//send buffer
			Packetizer_ReportData(Packetizer_ret);
		}
	}
}

void Packetizer_Init()
{
	Packetizer_Buffer = (unsigned char *)calloc(ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE, sizeof(unsigned char));
	Packetizer_ErrorBuffer = (unsigned char *)calloc(ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE, sizeof(unsigned char));

	Packetizer_ret.count = (ag_PREAMBLESIZE + ag_PACKETSIZE + ag_ERRORCORRECTIONSIZE) * 8;
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