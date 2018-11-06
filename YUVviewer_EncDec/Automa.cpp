#include "stdafx.h"
#include "Automa.h"
#include "Resource.h"
using namespace std;

int attr[8] = { 53,106,212,169,83,166,77,154 };	//A status circle of cellular automata
void Automa::ECAENC_Y(unsigned char R[], int seed, int Width, int Height)
{
	int rt, rn, r;
	srand(seed);
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			//key[i*Y_HEIGHT + j]=rand() % 7;
			//rt = 1 + key[i*Y_HEIGHT + j];
			rt = 1 + rand() % 7;
			r = R[i*Height + j];
			for (int n = 0; n < rt; n++)
				r = r^attr[((j + i) % 8 + n) % 8];
			if(Width==176)
				enY_space[i*Height + j] = r;
			else
				enY_space_CIF[i*Height + j] = r;
		}
	}
}
void Automa::ECAENC_U(unsigned char R[], int seed, int Width, int Height)
{
	int rt, rn, r;
	srand(seed);
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			rt = 1 + rand() % 7;
			r = R[i*Height + j];
			for (int n = 0; n < rt; n++)
				r = r^attr[((j + i) % 8 + n) % 8];
			if (Width == 88)
				enU_space[i*Height + j] = r;
			else
				enU_space_CIF[i*Height + j] = r;
		}
	}
}
void Automa::ECAENC_V(unsigned char R[], int seed, int Width, int Height)
{
	int rt, rn, r;
	srand(seed);
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			rt = 1 + rand() % 7;
			r = R[i*Height + j];
			for (int n = 0; n < rt; n++)
				r = r^attr[((j + i) % 8 + n) % 8];
			if (Width == 88)
				enV_space[i*Height + j] = r;
			else
				enV_space_CIF[i*Height + j] = r;
		}
	}
}

unsigned char* Automa::ECADEC_Y(unsigned char *R, int key, int Width, int Height)
{
	int rt, rn, r;
	srand(key);
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			rt = 1 + rand() % 7;
			rn = 8 - rt;
			r = R[i*Height + j];
			for (int n = 0; n < rn; n++)
				r = r^attr[(((j + i) % 8 + rt) % 8 + n) % 8];
			R[i*Height + j] = r;
		}
	}
	return R;
}
unsigned char* Automa::ECADEC_U(unsigned char *R, int key, int Width, int Height)
{
	int rt, rn, r;
	srand(key);
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			rt = 1 + rand() % 7;
			rn = 8 - rt;
			r = R[i*Height + j];
			for (int n = 0; n < rn; n++)
				r = r^attr[(((j + i) % 8 + rt) % 8 + n) % 8];
			R[i*Height + j] = r;
		}
	}
	return R;
}
unsigned char* Automa::ECADEC_V(unsigned char *R, int key, int Width, int Height)
{
	int rt, rn, r;
	srand(key);
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			rt = 1 + rand() % 7;
			rn = 8 - rt;
			r = R[i*Height + j];
			for (int n = 0; n < rn; n++)
				r = r^attr[(((j + i) % 8 + rt) % 8 + n) % 8];
			R[i*Height + j] = r;
		}
	}
	return R;
}

Automa::~Automa()
{

}

//¼ÓÃÜ
int Automa::Encrypt(char *file_in, int *key, int flag, int Width, int Height)
{
	int nFrame, size, EncDec;
	int i = 0, j = 0;
	Y_WIDTH = Width;
	Y_HEIGHT = Height;
	U_WIDTH = Width / 2;
	U_HEIGHT = Height / 2;
	V_WIDTH = Width / 2;
	V_HEIGHT = Height / 2;

	FILE * in, *out;
	char *file_out;
	file_out = (char*)malloc(sizeof(char) * 128);
	
	while (file_in[i] != '\0')
	{
		if (file_in[i] == '.')
		{
			file_out[j++] = file_in[i++];
			file_out[j++] = 'e';
			file_out[j++] = '.';
		}
		else
			file_out[j++] = file_in[i++];
	}
	file_out[j] = '\0';

	in = fopen(file_in, "rb+"); 
	out = fopen(file_out, "wb+"); 
	if (in == NULL || out == NULL) {
		return  -1;
	}

	fseek(out, 0, 0);
	fseek(in, 0, SEEK_END);   //move file pointer to file end
	size = ftell(in);     //get the file's size 
	rewind(in);       //pointer point to file head
	nFrame = size / (Width*Height*3/2);  //get the #frames of the video
	//encrypt all 3 channels
	if (flag == 0) 
	{
		for (int j = 1; j <= nFrame; j++)
		{
			if (Y_WIDTH == 176)
			{
				for (i = 0; i < Y_WIDTH*Y_HEIGHT; i++)
					fread(&Y_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < U_WIDTH*U_HEIGHT; i++)
					fread(&U_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < V_WIDTH*V_HEIGHT; i++)
					fread(&V_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				ECAENC_Y(Y_space, key[0], Y_WIDTH, Y_HEIGHT);
				ECAENC_U(U_space, key[1], U_WIDTH, U_HEIGHT);
				ECAENC_V(V_space, key[2], V_WIDTH, V_HEIGHT);
				fwrite(enY_space, sizeof(unsigned char), sizeof(enY_space), out);
				fwrite(enU_space, sizeof(unsigned char), sizeof(enU_space), out);
				fwrite(enV_space, sizeof(unsigned char), sizeof(enV_space), out);
			}
			else
			{
				for (i = 0; i < Y_WIDTH*Y_HEIGHT; i++)
					fread(&Y_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < U_WIDTH*U_HEIGHT; i++)
					fread(&U_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < V_WIDTH*V_HEIGHT; i++)
					fread(&V_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				ECAENC_Y(Y_space_CIF, key[0], Y_WIDTH, Y_HEIGHT);
				ECAENC_U(U_space_CIF, key[1], U_WIDTH, U_HEIGHT);
				ECAENC_V(V_space_CIF, key[2], V_WIDTH, V_HEIGHT);
				fwrite(enY_space_CIF, sizeof(unsigned char), sizeof(enY_space_CIF), out);
				fwrite(enU_space_CIF, sizeof(unsigned char), sizeof(enU_space_CIF), out);
				fwrite(enV_space_CIF, sizeof(unsigned char), sizeof(enV_space_CIF), out);
			}
			
		}
		fclose(in);
		fclose(out);
	}
	//encrypt only Y channel
	else if (flag == 1)
	{
		for (int j = 1; j <= nFrame; j++)
		{
			if (Y_WIDTH == 176)
			{
				for (i = 0; i < Y_WIDTH*Y_HEIGHT; i++)
					fread(&Y_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < U_WIDTH*U_HEIGHT; i++)
					fread(&U_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < V_WIDTH*V_HEIGHT; i++)
					fread(&V_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				ECAENC_Y(Y_space, key[0], Y_WIDTH, Y_HEIGHT);
				fwrite(enY_space, sizeof(unsigned char), sizeof(enY_space), out);
				fwrite(U_space, sizeof(unsigned char), sizeof(enU_space), out);
				fwrite(V_space, sizeof(unsigned char), sizeof(enV_space), out);
			}
			else
			{
				for (i = 0; i < Y_WIDTH*Y_HEIGHT; i++)
					fread(&Y_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < U_WIDTH*U_HEIGHT; i++)
					fread(&U_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < V_WIDTH*V_HEIGHT; i++)
					fread(&V_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				ECAENC_Y(Y_space_CIF, key[0], Y_WIDTH, Y_HEIGHT);
				fwrite(enY_space_CIF, sizeof(unsigned char), sizeof(enY_space_CIF), out);
				fwrite(U_space_CIF, sizeof(unsigned char), sizeof(enU_space_CIF), out);
				fwrite(V_space_CIF, sizeof(unsigned char), sizeof(enV_space_CIF), out);
			}
		}
		fclose(in);
		fclose(out);
	}
	//encrypt only U channel
	else if (flag == 2)
	{
		for (int j = 1; j <= nFrame; j++)
		{
			if (Y_WIDTH == 176) {
				for (i = 0; i < Y_WIDTH*Y_HEIGHT; i++)
					fread(&Y_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < U_WIDTH*U_HEIGHT; i++)
					fread(&U_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < V_WIDTH*V_HEIGHT; i++)
					fread(&V_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				ECAENC_U(U_space, key[1], U_WIDTH, U_HEIGHT);
				fwrite(Y_space, sizeof(unsigned char), sizeof(enY_space), out);
				fwrite(enU_space, sizeof(unsigned char), sizeof(enU_space), out);
				fwrite(V_space, sizeof(unsigned char), sizeof(enV_space), out);
			}
			else
			{
				for (i = 0; i < Y_WIDTH*Y_HEIGHT; i++)
					fread(&Y_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < U_WIDTH*U_HEIGHT; i++)
					fread(&U_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < V_WIDTH*V_HEIGHT; i++)
					fread(&V_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				ECAENC_U(U_space_CIF, key[1], U_WIDTH, U_HEIGHT);
				fwrite(Y_space_CIF, sizeof(unsigned char), sizeof(enY_space_CIF), out);
				fwrite(enU_space_CIF, sizeof(unsigned char), sizeof(enU_space_CIF), out);
				fwrite(V_space_CIF, sizeof(unsigned char), sizeof(enV_space_CIF), out);
			}
		}
		fclose(in);
		fclose(out);
	}
	//encrypt only V channel
	else if (flag == 3)
	{
		for (int j = 1; j <= nFrame; j++)
		{
			if (Y_WIDTH == 176) 
			{
				for (i = 0; i < Y_WIDTH*Y_HEIGHT; i++)
					fread(&Y_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < U_WIDTH*U_HEIGHT; i++)
					fread(&U_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < V_WIDTH*V_HEIGHT; i++)
					fread(&V_space[i], sizeof(unsigned char), sizeof(unsigned char), in);
				ECAENC_V(V_space, key[2], V_WIDTH, V_HEIGHT);
				fwrite(Y_space, sizeof(unsigned char), sizeof(enY_space), out);
				fwrite(U_space, sizeof(unsigned char), sizeof(enU_space), out);
				fwrite(enV_space, sizeof(unsigned char), sizeof(enV_space), out);
			}
			else
			{
				for (i = 0; i < Y_WIDTH*Y_HEIGHT; i++)
					fread(&Y_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < U_WIDTH*U_HEIGHT; i++)
					fread(&U_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				for (i = 0; i < V_WIDTH*V_HEIGHT; i++)
					fread(&V_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
				ECAENC_V(V_space_CIF, key[2], V_WIDTH, V_HEIGHT);
				fwrite(Y_space_CIF, sizeof(unsigned char), sizeof(enY_space_CIF), out);
				fwrite(U_space_CIF, sizeof(unsigned char), sizeof(enU_space_CIF), out);
				fwrite(enV_space_CIF, sizeof(unsigned char), sizeof(enV_space_CIF), out);
			}
		}
		fclose(in);
		fclose(out);
	}
	return 1;
}	