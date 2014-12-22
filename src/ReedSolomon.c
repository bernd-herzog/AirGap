#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "agmath.h"


#define nsym ag_ERRORCORRECTIONSIZE
#define chunk_size ag_PACKETSIZE

unsigned char gf_exp[512] = { 1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38, 76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192, 157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35, 70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161, 95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240, 253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226, 217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206, 129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204, 133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84, 168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115, 230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255, 227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65, 130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166, 81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9, 18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22, 44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1, 2 };
unsigned char gf_log[256] = { 0, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75, 4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113, 5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69, 29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166, 6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136, 54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64, 30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61, 202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87, 7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24, 227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46, 55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97, 242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162, 31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246, 108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90, 203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215, 79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175 };

void gf_poly_eval(unsigned char msg[], int x, unsigned char synd[], int size, int pos);


int gf_mul(int x, int y)
{
	if (x == 0 || y == 0)
	{
		return 0;
	}
	else{
		return gf_exp[gf_log[x] + gf_log[y]];
	}
}

void gf_poly_mul(unsigned char p[], unsigned char q[], int size_p, int size_q)
{
	unsigned char r[nsym+1];
	int new_size = size_p + size_q - 1;
	for (int b = 0; b < size_p + size_q - 1; b++)
	{
		r[b] = 0;
	}

	for (int j = 0; j < size_q; j++)
	{
		for (int i = 0; i < size_p; i++)
		{
			r[i + j] ^= gf_mul(p[i], q[j]);
		}
	}

	for (int x = 0; x < new_size; x++)
	{
		p[x] = r[x];
	}
}

void rs_generator_poly(unsigned char g[]){
	g[0] = 1;
	unsigned char h[2];
	h[0] = 1;
	for (int z = 0; z < nsym; z++)
	{
		h[1] = gf_exp[z];
		gf_poly_mul(g, h, z + 1, 2);
	}
}

void rs_encode_msg(unsigned char *msg_out)
{
	unsigned char gen[ag_ERRORCORRECTIONSIZE + 1];
	rs_generator_poly(gen);

	for (int y = ag_PACKETSIZE; y < ag_PACKETSIZE + nsym; y++)
	{
		msg_out[y] = 0;
	}

	for (int p = 0; p < ag_PACKETSIZE; p++)
	{
		unsigned char coef;
		coef = msg_out[p];
		if (coef != 0)
		{
			for (int h = 0; h < nsym + 1; h++)
			{
				msg_out[p + h] ^= gf_mul(gen[h], coef);
			}
		}
	}
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!decoder!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



void gf_poly_add(unsigned char p[], unsigned char q[], int p_size, int q_size, unsigned char out[], int *out_size)
{
	for (int g = 0; g < p_size; g++)
	{
		out[g + *out_size - p_size] = p[g];
	}
	for (int h = 0; h < q_size; h++)
	{
		out[h + *out_size - q_size] ^= q[h];
	}
}


int gf_div(int x, int y)
{
	if (y == 0)
	{
		//Division durch Null muss noch abgefangen werden!!!
		return 0;
	}
	else{
		if (x == 0){
			return 0;
		}
		else{
			return gf_exp[gf_log[x] + 255 - gf_log[y]];
		}
	}
}

void gf_poly_scale(unsigned char p[], int x, int p_size, unsigned char out[])
{
	for (int a = 0; a < p_size; a++)
	{
		out[a] = gf_mul(p[a], x);
	}
}

void rs_find_errors(unsigned char err_pos[], unsigned char synd[], int *err_pos_size) //Berlekamp-Massey algorithmus
{
	unsigned char err_poly[256]; //!!!!!!!!!!!!!!!!!!Länge muss noch bestimmt werden!!!!!!!!!!!!!!!!!!!!!
	unsigned char old_poly[256]; //!!!!!!!!!!!!!!!!!!Länge muss noch bestimmt werden!!!!!!!!!!!!!!!!!!!!!
	unsigned char new_poly[256]; //!!!!!!!!!!!!!!!!!!Länge muss noch bestimmt werden!!!!!!!!!!!!!!!!!!!!!
	err_poly[0] = 1;
	int err_poly_size = 1;
	old_poly[0] = 1;
	int old_poly_size = 1;
	int new_poly_size = 0;
	for (int x = 0; x < nsym; x++)
	{
		old_poly[x + 1] = 0;
		old_poly_size++;
		unsigned char delta = synd[x];
		for (int y = 1; y<err_poly_size; y++)
		{
			delta ^= gf_mul(err_poly[err_poly_size - 1 - y], synd[x - y]);
		}
		if (delta != 0)
		{
			if (old_poly_size > err_poly_size)
			{
				gf_poly_scale(old_poly, delta, old_poly_size, new_poly);
				new_poly_size = old_poly_size;

				gf_poly_scale(err_poly, gf_div(1, delta), err_poly_size, old_poly);
				old_poly_size = err_poly_size;

				for (int z = 0; z<new_poly_size; z++)
				{
					err_poly[z] = new_poly[z];
				}
				err_poly_size = new_poly_size;
			}
			unsigned char *tmp_poly = (unsigned char *)calloc(old_poly_size, sizeof(unsigned char));
			gf_poly_scale(old_poly, delta, old_poly_size, tmp_poly);

			int tmp_error_poly_size;
			if (err_poly_size>old_poly_size){
				tmp_error_poly_size = err_poly_size;
			}
			else{
				tmp_error_poly_size = old_poly_size;
			}

			unsigned char *tmp_error_poly = (unsigned char *)calloc(tmp_error_poly_size, sizeof(unsigned char));
			for (int r = 0; r < tmp_error_poly_size; r++)
			{
				tmp_error_poly[r] = 0;
			}

			gf_poly_add(err_poly, tmp_poly, err_poly_size, old_poly_size, tmp_error_poly, &tmp_error_poly_size);
			free(tmp_poly);

			for (int f = 0; f < tmp_error_poly_size; f++) //geht das auch einfach mit pointer ändern?
			{
				err_poly[f] = tmp_error_poly[f];
			}

			free(tmp_error_poly);
			err_poly_size = tmp_error_poly_size;
		}
	}
	int errs = err_poly_size - 1;
	*err_pos_size = errs;
	int count = 0;
	if (errs * 2 > nsym)
	{
		//Es sind zu viele Fehler zum Korrigieren!!! Hier ein abbruch rein!!!!!!!!!!!!!!!!!!!!!!!!
	}
	unsigned char gf_poly_eval_out[ag_PACKETSIZE];
	for (int k = 0; k < ag_PACKETSIZE; k++)
	{
		gf_poly_eval(err_poly, gf_exp[255 - k], gf_poly_eval_out, err_poly_size, k);
		if (gf_poly_eval_out[k] == 0)
		{
			err_pos[count] = (ag_PACKETSIZE - 1 - k);
			count++;
		}
	}
}


void gf_poly_eval(unsigned char msg[], int x, unsigned char synd[], int size, int pos)
{
	unsigned char *local = calloc(size, sizeof(unsigned char));
	local[0] = msg[0];
	for (int i = 1; i < size; i++)
	{
		local[i] = gf_mul(local[i - 1], x) ^ msg[i];
	}
	synd[pos] = local[size - 1];

	free(local);
}

void rs_calc_syndromes(unsigned char msg[], unsigned char synd[], int size)
{
	for (int i = 0; i < nsym; i++)
	{
		gf_poly_eval(msg, gf_exp[i], synd, size, i);
	}

}


void rs_correct_errata(unsigned char msg[], unsigned char synd[], unsigned char err_pos[], int size, int err_pos_size)
{
	unsigned char q[256]; //Länge wohl unnötig groß!!!!!!!!!!!!!!!!!!!!!!!!!!!
	q[0] = 1;
	int q_size = 1;
	for (int i = 0; i < err_pos_size; i++)
	{
		unsigned char h[2];
		h[0] = gf_exp[size - 1 - err_pos[i]];
		h[1] = 1;
		gf_poly_mul(q, h, q_size, 2);
		q_size++;
	}
	unsigned char p[256];
	for (int k = 0; k < err_pos_size; k++){
		p[k] = synd[err_pos_size - 1 - k];
	}
	gf_poly_mul(p, q, err_pos_size, q_size);
	int p_size = err_pos_size + q_size - 1;
	unsigned char *r = (unsigned char *)calloc(err_pos_size, sizeof(unsigned char));
	for (int i = 0; i<err_pos_size; i++)
	{
		r[i] = p[p_size - err_pos_size + i];
	}

	unsigned char *s = (unsigned char *)calloc(q_size / 2, sizeof(unsigned char));
	int s_size = 0;
	if (q_size>1){
		if (q_size % 2)
		{
			for (int j = 1; j < q_size - 1; j += 2)
			{
				s[s_size] = q[j];
				s_size++;
			}
		}
		else{
			for (int o = 0; o < q_size - 1; o += 2)
			{
				s[s_size] = q[o];
				s_size++;
			}
		}
	}
	for (int t = 0; t < err_pos_size; t++)
	{
		int x = gf_exp[err_pos[t] + 256 - size];
		unsigned char y[1];
		gf_poly_eval(r, x, y, err_pos_size, 0);
		unsigned char z[1];
		gf_poly_eval(s, gf_mul(x, x), z, s_size, 0);
		msg[err_pos[t]] ^= gf_div(y[0], gf_mul(x, z[0]));

	}

	free(r);
	free(s);
}


void rs_correct_msg(unsigned char *msg_out)
{
	//unsigned char msg_out[size];
	//unsigned char erase_pos[nsym]; //man könnte noch gelöschte symbole erkennung hinzufügen (wenn man weiß, das gelöscht wurde dann kann man bis zu nsym gelöschte chars rekonstruieren), ist aber glaube ich etwas tricky
	unsigned char synd[nsym];
	//for(int x=0; x<size;x++)
	//{
	//	msg_out[x]=msg[x];
	//}
	rs_calc_syndromes(msg_out, synd, ag_PACKETSIZE);

	for (int w = 0; w < nsym; w++)
	{
		if (synd[w] != 0)
		{
			break;
		}
		if (w = ag_PACKETSIZE - 1)
		{
			return;// keine fehler in der Message enthalten :-) hier kann man also gleich zurückgeben!
		}
	}
	//fsynd = rs_forney_syndromes(synd, erase_pos, size); für die erkennung von löschungen zu gebrauchen; wobei synd dann durch fsynd ersetzt werden muss
	//ich weiß aber nicht wie wir löschungen gescheid darstellen, da ja zB. keine minuszahlen möglich sind und alle 8 bit des char gebraucht werden
	unsigned char err_pos[nsym];
	int err_pos_size = 0;
	rs_find_errors(err_pos, synd, &err_pos_size);

	if (err_pos_size == 0)
	{
		return;//keine Fehler gefunden
	}
	rs_correct_errata(msg_out, synd, err_pos, ag_PACKETSIZE, err_pos_size);
	rs_calc_syndromes(msg_out, synd, ag_PACKETSIZE);
	//for(int x=0; x<size;x++)
	//{
	//	msg[x]=msg_out[x];		
	//}

	//Achtung msg hat immer noch die parity bits in größe nsym hinten drann
}



//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!decoder!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//int main()
//{
//	unsigned char chunk[50]={'t','e','s','t','!','t','e','s','t','!','t','e','s','t','!','t','e','s','t','!','t','e','s','t','!','t','e','s','t','!','t','e','s','t','!','t','e','s','t','!','t','e','s','t','!','t','e','s','t','!'};
//	unsigned char parity[nsym];
//	unsigned char data[50+nsym];
//	rs_encode_msg(chunk, parity, 50);
//	printf("Die Payload:\n");
//	for(int d=0;d<50;d++)
//	{
//		printf("%x",chunk[d]);
//		data[d]=chunk[d];
//	}
//	printf("\nDie parity-chars in hex:\n");
//	for(int e=0;e<nsym;e++)
//	{
//		printf("%x",parity[e]);
//		data[e+50]=parity[e];
//	}
//	printf("\nBeides Zusammen:\n");
//	for(int d=0;d<50;d++)
//	{
//		printf("%x",chunk[d]);
//	}
//	for(int e=0;e<nsym;e++)
//	{
//		printf("%x",parity[e]);
//	}
//	for(int e=0;e<nsym;e++)
//	{
//		printf("\nparity[%i]: %x",e,parity[e]);
//	}
//	
//	//bei 32 korrektur chars können wir bis zu 16 fehlerhafte chars korrigieren!!! Beweis:
//	data[0]=111;
//	data[2]=111;
//	data[4]=111;
//	data[5]=111;
//	data[7]=111;
//	
//	data[10]=111;
//	data[12]=111;
//	data[14]=111;
//	data[15]=111;
//	data[17]=111;
//	
//	data[20]=111;
//	data[22]=111;
//	data[24]=111;
//	data[25]=111;
//	data[27]=111;	
//	
//	data[30]=111;
//	
//	
//	rs_correct_msg(data, 50+nsym);
//	
//	printf("\nHier wird decoded: \n");
//	for(int e=0;e<50;e++)
//	{
//		printf("Decoded[%i]: %c\n",e,data[e]);
//	}
//    return 0;
//    
//		
//    
//}


