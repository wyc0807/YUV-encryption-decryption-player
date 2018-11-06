#include "stdafx.h"
#include "Logistic.h"
#include "Resource.h"
using namespace std;
unsigned char enY_init[Y_WIDTH_QCIF][Y_HEIGHT_QCIF];
unsigned char enU_init[U_WIDTH_QCIF][U_HEIGHT_QCIF];
unsigned char enV_init[V_WIDTH_QCIF][V_HEIGHT_QCIF];
unsigned char enY_init_CIF[Y_WIDTH_CIF][Y_HEIGHT_CIF];
unsigned char enU_init_CIF[U_WIDTH_CIF][U_HEIGHT_CIF];
unsigned char enV_init_CIF[V_WIDTH_CIF][V_HEIGHT_CIF];
Logistic::~Logistic()
{

}
int* Logistic::getBin(int data)
{
	int mask = 128;
	static int *bin = new int[8];
	for (int i = 0; i < 8; i++)
	{
		bin[i] = (data&mask) >> 7;
		data = data << 1;
	}
	return bin;
}
double* Logistic::generY0(double y, int K)          //generate Y0
{
	
	static double *_y0=new double[K];
	for (int i = 0; i < K; i++)
	{
		y = 3.9999*y*(1 - y);
		if (y > 0.1&&y < 0.9)
			_y0[i] = y;
		else 
		{
			do
			{
				y = 3.9999*y*(1 - y);
			} while (y <= 0.1 || y >= 0.90);
			_y0[i] = y;
		}
	}
	return _y0;
}
int* Logistic::getP(double x)
{
	double *f=new double[24];
	int *P=new int[24];
	for (int i = 0; i < 24; i++)
	{
		x = 3.9999*x*(1 - x);
		if (x >= 0.1&&x <= 0.9)
			f[i] = x;
		else
		{
			do
			{
				x = 3.9999*x*(1 - x);
			} while (x<0.1 || x>0.9);
			f[i] = x;
		}
	}
	for (int i = 0; i < 24; i++)
		P[i] = (int)(23 * (f[i] - 0.1) / 0.8) + 1;
	free(f);
	return P;
}
double Logistic::getX0(int K[])
{
	double X0, X01, X02;
	int *k3 = getBin(K[3]);
	int *k4 = getBin(K[4]);
	int *k5 = getBin(K[5]);
	X01 = k3[0] + k3[1] * 2 + k3[2] * 4 + k3[3] * 8 + k3[4] * 16 + k3[5] * 32 + k3[6] * 64 + k3[7] * 128;
	X01 += (k4[0] + k4[1] * 2 + k4[2] * 4 + k4[3] * 8 + k4[4] * 16 + k4[5] * 32 + k4[6] * 64 + k4[7] * 128) * 256;
	X01 += (k5[0] + k5[1] * 2 + k5[2] * 5 + k5[3] * 8 + k5[5] * 16 + k5[5] * 32 + k5[6] * 65 + k5[7] * 128) * 65536;
	X01 = X01 / 16777216;
	X02 = (1 + 0 + 15 + 15 + 14 + 8 + 0 + 5) / 96;
	//X0 = (X01 + X02) % 1;
	X0 = (X01 + X02);
	return X0;
}
double Logistic::getY0(int K[], int P[])
{
	int *B2 = new int[24];
	double Y0, Y01, Y02;
	int *k = getBin(K[0]);
	for (int i = 0; i < 8; i++)
		B2[i] = k[i];
	k = getBin(K[1]);
	for (int i = 0; i < 8; i++)
		B2[i + 8] = k[i];
	k = getBin(K[2]);
	for (int i = 0; i < 8; i++)
		B2[i + 16] = k[i];
	Y01 = (K[2] + K[1] * 256 + K[0] * 65536) / 16777216;
	Y02 = B2[P[0]] + B2[P[1]] * 2 + B2[P[2]] * 4 + B2[P[3]] * 8 + B2[P[4]] * 16 + B2[P[5]] * 32 + B2[P[6]] * 64 + B2[P[7]] * 128
		+ B2[P[8]] * 256 + B2[P[9]] * 512 + B2[P[10]] * 1024 + B2[P[11]] * 2048 + B2[P[12]] * 4096 + B2[P[13]] * 8192 + B2[P[14]] * 16384
		+ B2[P[15]] * 32768 + B2[P[16]] * 65536 + B2[P[17]] * 131072 + B2[P[18]] * 262144 + B2[P[19]] * 524288 + B2[P[20]] * 1048576
		+ B2[P[21]] * 2097152 + B2[P[22]] * 4194304 + B2[P[23]] * 8388608;
	Y02 = Y02 / 16777216;
	//Y0 = (Y01 + Y02) % 1;
	Y0 = (Y01 + Y02);
	return Y0;
}
void Logistic::Log_enc_dec_Y(unsigned char* R, double Y0, int ED,int *key, int Width, int Height)
{
	//unsigned char* enY_temp_space=(unsigned char*)malloc(sizeof(unsigned char) * Width * Height);
	int y;
	int bsize = 4;
	int height = Height / bsize;
	int width = Width / bsize;
	double Y, z;
	Y = Y0;
	int N = 256;
	for (int i = 0; i < Width; i++)
		for (int j = 0; j < Height; j++)
		{
			if(Width==176)
				enY_init[i][j] = R[i*Height + j];
			else
				enY_init_CIF[i][j] = R[i*Height + j];
		}
			
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i < height; i++)
		{
			for (int l = 0; l < 4; l++)
			{
				for (int k = 0; k < 4; k++)
				{
					//y = new1_Y_space[j*bsize + k][i*bsize + l];
					if (Width == 176)
						y = enY_init[j*bsize + k][i*bsize + l];
					else
						y = enY_init_CIF[j*bsize + k][i*bsize + l];
					double *y0 = generY0(Y, key[9]);
					Y = y0[key[9] - 1];
					for (int n = 0; n < key[9]; n++)
					{
						if (ED == 1)
							z = y0[n];
						else
							z = y0[key[9] - n - 1];
						//group 1
						if ((z > 0.10&&z<0.13) || (z>0.34&&z<0.37) || z == 0.34 || (z>0.58&&z < 0.62) || z == 0.58)
						{
							y = ~y + N;
						}
						//group 2
						else if ((z > 0.13&&z<0.16) || z == 0.13 || (z>0.37&&z<0.40) || z == 0.37 || (z>0.62&&z < 0.66) || z == 0.62)
						{
							y = (y^key[3] + N) % N;
						}
						//group 3
						else if ((z > 0.13&&z>0.16) || z == 0.16 || (z>0.40&&z<0.43) || z == 0.40 || (z>0.66&&z < 0.70) || z == 0.66)
						{
							if (ED == 1)
							{
								y = (y + key[3] + key[4]) % N;
							}
							else
							{
								y = (y - key[3] - key[4] + 512) % N;								
							}
						}
						//group 4
						else if ((z > 0.19&&z<0.22) || z == 0.19 || (z>0.43&&z<0.46) || z == 0.43 || (z>0.70&&z < 0.74) || z == 0.70)
						{
							if (ED == 1)
							{
								y = (~(y^key[3]) + N) % N;								
							}
							else
							{
								y = (((~y + 256) ^ key[3]) + N) % N;				
							}
						}
						//group 5
						else if ((z > 0.22&&z<0.25) || z == 0.22 || (z>0.46&&z<0.49) || z == 0.46 || (z>0.74&&z < 0.78) || z == 0.74)
						{
							y = (y^key[6] + N) % N;
						}
						//group 6
						else if ((z > 0.25&&z<0.28) || z == 0.25 || (z>0.49&&z<0.52) || z == 0.49 || (z>0.78&&z < 0.82) || z == 0.78)
						{
							if (ED == 1)
							{
								y = (y + key[6] + key[7]) % N;								
							}
							else
							{
								y = (y - key[6] - key[7] + 512) % N;								

							}
						}
						//group 7
						else if ((z > 0.28&&z<0.31) || z == 0.28 || (z>0.52&&z<0.55) || z == 0.52 || (z>0.82&&z < 0.86))
						{
							if (ED == 1)
							{
								y = (~(y^key[6] + N)) % N;								
							}
							else
							{
								y = (((~y + 256) ^ key[6]) + N) % N;		
							}
						}
						//no operations on group 8
					}
					if(Width==176)
						Y_encrypted[j*bsize + k][i*bsize + l] = y;
					else
						Y_encrypted_CIF[j*bsize + k][i*bsize + l] = y;
					//R[(j*bsize + k)*Height + i*bsize + l] = y;
				}
			}
			for (int k = 0; k < 9; k++)
				key[k] = (key[k] + key[9]) % N;
		}
	}
	for (int i = 0; i < Width; i++)
		for (int k = 0; k < Height; k++)
		{
			if(Width==176)
				R[i*Height + k] = Y_encrypted[i][k];
			else
				R[i*Height + k] = Y_encrypted_CIF[i][k];
		}
	
}
void Logistic::Log_enc_dec_U(unsigned char* R, double Y0, int ED,int *key, int Width, int Height)
{
	int N = 256;
	int u;
	int bsize = 4;
	int height = Height / bsize;
	int width = Width / bsize;
	double y, z;
	y = Y0;
	for (int i = 0; i < Width; i++)
		for (int j = 0; j < Height; j++)
		{
			if (Width == 88)
				enU_init[i][j] = R[i*Height + j];
			else
				enU_init_CIF[i][j] = R[i*Height + j];
		}
			
	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i <height ; i++)
		{
			for (int l = 0; l < 4; l++)
			{
				for (int k = 0; k < 4; k++)
				{
					//u = new1_U_space[j*bsize + k][i*bsize + l];
					if (Width == 88)
						u = enU_init[j*bsize + k][i*bsize + l];
					else
						u = enU_init_CIF[j*bsize + k][i*bsize + l];
					//u = R[(j*bsize + k)*Height + i*bsize + l];
					double *y0 = generY0(y, key[9]);
					y = y0[key[9] - 1];
					for (int n = 0; n < key[9]; n++)
					{
						if (ED == 1)
							z = y0[n];
						else
							z = y0[key[9] - n - 1];
						//group 1
						if ((z > 0.10&&z<0.13) || (z>0.34&&z<0.37) || z == 0.34 || (z>0.58&&z < 0.62) || z == 0.58)
						{
							u = ~u + N;
						}
						//group 2
						else if ((z > 0.13&&z<0.16) || z == 0.13 || (z>0.37&&z<0.40) || z == 0.37 || (z>0.62&&z < 0.66) || z == 0.62)
						{
							u = (u^key[4] + N) % N;
						}
						//group 3
						else if ((z > 0.13&&z>0.16) || z == 0.16 || (z>0.40&&z<0.43) || z == 0.40 || (z>0.66&&z < 0.70) || z == 0.66)
						{
							if (ED == 1)
								u = (u + key[4] + key[5]) % N;
							else
								u = (u - key[4] - key[5] + 512) % N;
						}
						//group 4
						else if ((z > 0.19&&z<0.22) || z == 0.19 || (z>0.43&&z<0.46) || z == 0.43 || (z>0.70&&z < 0.74) || z == 0.70)
						{
							if (ED == 1)
								u = (~(u^key[4]) + N) % N;
							else
								u = (((~u + 256) ^ key[4]) + N) % N;
						}
						//group 5
						else if ((z > 0.22&&z<0.25) || z == 0.22 || (z>0.46&&z<0.49) || z == 0.46 || (z>0.74&&z < 0.78) || z == 0.74)
						{
							u = (u^key[7] + N) % N;
						}
						//group 6
						else if ((z > 0.25&&z<0.28) || z == 0.25 || (z>0.49&&z<0.52) || z == 0.49 || (z>0.78&&z < 0.82) || z == 0.78)
						{
							if (ED == 1)
							{
								u = (u + key[7] + key[8]) % N;
							}
							else
							{
								u = (u - key[7] - key[8] + 512) % N;
							}
						}
						//group 7
						else if ((z > 0.28&&z<0.31) || z == 0.28 || (z>0.52&&z<0.55) || z == 0.52 || (z>0.82&&z < 0.86))
						{
							if (ED == 1)
							{
								u = (~(u^key[7] + N)) % N;
							}
							else
							{
								u = (((~u + 256) ^ key[7]) + N) % N;
							}
						}
						//no operations on group 8
					}
					if (Width == 88)
						U_encrypted[j*bsize + k][i*bsize + l] = u;
					else
						U_encrypted_CIF[j*bsize + k][i*bsize + l] = u;
				}
			}
			for (int k = 0; k < 9; k++)
				key[k] = (key[k] + key[9]) % N;
		}
	}
	for (int i = 0; i < Width; i++)
		for (int k = 0; k < Height; k++)
		{
			if (Width == 88)
				R[i*Height + k] = U_encrypted[i][k];
			else
				R[i*Height + k] = U_encrypted_CIF[i][k];
		}			
}	 
void Logistic::Log_enc_dec_V(unsigned char* R, double Y0, int ED, int *key, int Width, int Height)
{
	int N = 256;
	int u;
	int bsize = 4;
	int height = Height / bsize;
	int width = Width / bsize;
	double y, z;
	y = Y0;
	for (int i = 0; i < Width; i++)
		for (int j = 0; j < Height; j++)
		{
			if (Width == 88)
				enV_init[i][j] = R[i*Height + j];
			else
				enV_init_CIF[i][j] = R[i*Height + j];
		}

	for (int j = 0; j < width; j++)
	{
		for (int i = 0; i <height; i++)
		{
			for (int l = 0; l < 4; l++)
			{
				for (int k = 0; k < 4; k++)
				{
					//u = new1_U_space[j*bsize + k][i*bsize + l];
					if (Width == 88)
						u = enV_init[j*bsize + k][i*bsize + l];
					else
						u = enV_init_CIF[j*bsize + k][i*bsize + l];
					//u = R[(j*bsize + k)*Height + i*bsize + l];
					double *y0 = generY0(y, key[9]);
					y = y0[key[9] - 1];
					for (int n = 0; n < key[9]; n++)
					{
						if (ED == 1)
							z = y0[n];
						else
							z = y0[key[9] - n - 1];
						//group 1
						if ((z > 0.10&&z<0.13) || (z>0.34&&z<0.37) || z == 0.34 || (z>0.58&&z < 0.62) || z == 0.58)
						{
							u = ~u + N;
						}
						//group 2
						else if ((z > 0.13&&z<0.16) || z == 0.13 || (z>0.37&&z<0.40) || z == 0.37 || (z>0.62&&z < 0.66) || z == 0.62)
						{
							u = (u^key[4] + N) % N;
						}
						//group 3
						else if ((z > 0.13&&z>0.16) || z == 0.16 || (z>0.40&&z<0.43) || z == 0.40 || (z>0.66&&z < 0.70) || z == 0.66)
						{
							if (ED == 1)
								u = (u + key[4] + key[5]) % N;
							else
								u = (u - key[4] - key[5] + 512) % N;
						}
						//group 4
						else if ((z > 0.19&&z<0.22) || z == 0.19 || (z>0.43&&z<0.46) || z == 0.43 || (z>0.70&&z < 0.74) || z == 0.70)
						{
							if (ED == 1)
								u = (~(u^key[4]) + N) % N;
							else
								u = (((~u + 256) ^ key[4]) + N) % N;
						}
						//group 5
						else if ((z > 0.22&&z<0.25) || z == 0.22 || (z>0.46&&z<0.49) || z == 0.46 || (z>0.74&&z < 0.78) || z == 0.74)
						{
							u = (u^key[7] + N) % N;
						}
						//group 6
						else if ((z > 0.25&&z<0.28) || z == 0.25 || (z>0.49&&z<0.52) || z == 0.49 || (z>0.78&&z < 0.82) || z == 0.78)
						{
							if (ED == 1)
							{
								u = (u + key[7] + key[8]) % N;
							}
							else
							{
								u = (u - key[7] - key[8] + 512) % N;
							}
						}
						//group 7
						else if ((z > 0.28&&z<0.31) || z == 0.28 || (z>0.52&&z<0.55) || z == 0.52 || (z>0.82&&z < 0.86))
						{
							if (ED == 1)
							{
								u = (~(u^key[7] + N)) % N;
							}
							else
							{
								u = (((~u + 256) ^ key[7]) + N) % N;
							}
						}
						//no operations on group 8
					}
					if (Width == 88)
						V_encrypted[j*bsize + k][i*bsize + l] = u;
					else
						V_encrypted_CIF[j*bsize + k][i*bsize + l] = u;
				}
			}
			for (int k = 0; k < 9; k++)
				key[k] = (key[k] + key[9]) % N;
		}
	}
	for (int i = 0; i < Width; i++)
		for (int k = 0; k < Height; k++)
		{
			if (Width == 88)
				R[i*Height + k] = V_encrypted[i][k];
			else
				R[i*Height + k] = V_encrypted_CIF[i][k];
		}
}

int Logistic::Logistic_chaos_map(char *file_in,int *KEY,int flag,int Width,int Height,int EncDec) 
{
	clock_t start_time = clock();
	int i = 0, j = 0, nFrame, size;
	FILE *in, *out;
	double X0 = getX0(KEY);
	int *P = getP(X0);
	double Y0 = getY0(KEY, P);
	char *file_out;
	file_out = (char*)malloc(sizeof(char) * 128);
	while (file_in[i] != '\0')
	{
		if (file_in[i] == '.')
		{
			file_out[j++] = file_in[i++];
			if (EncDec == 1)
				file_out[j++] = 'e';
			else
				file_out[j++] = 'd';
			file_out[j++] = '.';
		}
		else
			file_out[j++] = file_in[i++];
	}
	file_out[j] = '\0';
	Y_WIDTH = Width;
	Y_HEIGHT = Height;
	U_WIDTH = Width / 2;
	U_HEIGHT = Height / 2;
	V_WIDTH = Width / 2;
	V_HEIGHT = Height / 2;
	in = fopen(file_in, "rb+");  
	out = fopen(file_out, "wb+");  
	//if(EncDec==1)
		//out = fopen(file_out, "wb+"); 
	if (in == NULL||out==NULL)
		return  -1;

	fseek(out, 0, 0);
	fseek(in, 0, SEEK_END);   ///move file pointer to file end
	size = ftell(in);     //get the file's size 
	rewind(in);       //pointer re-point to file head
	nFrame = size / (Width*Height * 3 / 2);   //get the #frames of the video
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
		}
		else
		{
			for (i = 0; i < Y_WIDTH*Y_HEIGHT; i++)
				fread(&Y_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
			for (i = 0; i < U_WIDTH*U_HEIGHT; i++)
				fread(&U_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
			for (i = 0; i < V_WIDTH*V_HEIGHT; i++)
				fread(&V_space_CIF[i], sizeof(unsigned char), sizeof(unsigned char), in);
		}
		//encrypt/decrypt,1 means enc£¬-1 means dec
		if (EncDec == 1) 
		{
			if (flag == 0)		//all channels
			{
				if (Y_WIDTH == 176)
				{
					Log_enc_dec_Y(Y_space, Y0, EncDec, KEY, Y_WIDTH, Y_HEIGHT);
					Log_enc_dec_U(U_space, Y0, EncDec, KEY, U_WIDTH, U_HEIGHT);
					Log_enc_dec_V(V_space, Y0, EncDec, KEY, V_WIDTH, V_HEIGHT);
					for (i = 0; i < Y_WIDTH; i++)
						for (int k = 0; k < Y_HEIGHT; k++)
							enY_space[i*Y_HEIGHT + k] = Y_encrypted[i][k];
					for (i = 0; i < U_WIDTH; i++)
					{
						for (int k = 0; k < U_HEIGHT; k++)
						{
							enU_space[i*U_HEIGHT + k] = U_encrypted[i][k];
							enV_space[i*U_HEIGHT + k] = V_encrypted[i][k];
						}
					}
				}
				else
				{
					Log_enc_dec_Y(Y_space_CIF, Y0, EncDec, KEY, Y_WIDTH, Y_HEIGHT);
					Log_enc_dec_U(U_space_CIF, Y0, EncDec, KEY, U_WIDTH, U_HEIGHT);
					Log_enc_dec_V(V_space_CIF, Y0, EncDec, KEY, V_WIDTH, V_HEIGHT);
					for (i = 0; i < Y_WIDTH; i++)
						for (int k = 0; k < Y_HEIGHT; k++)
							enY_space_CIF[i*Y_HEIGHT + k] = Y_encrypted_CIF[i][k];
					for (i = 0; i < U_WIDTH; i++)
					{
						for (int k = 0; k < U_HEIGHT; k++)
						{
							enU_space_CIF[i*U_HEIGHT + k] = U_encrypted_CIF[i][k];
							enV_space_CIF[i*U_HEIGHT + k] = V_encrypted_CIF[i][k];
						}
					}
				}
				
			}
			else if (flag == 1)		//Y channel
			{
				if (Y_WIDTH == 176)
				{
					Log_enc_dec_Y(Y_space, Y0, EncDec, KEY, Y_WIDTH, Y_HEIGHT);
					for (i = 0; i < Y_WIDTH; i++)
						for (int k = 0; k < Y_HEIGHT; k++)
							enY_space[i*Y_HEIGHT + k] = Y_encrypted[i][k];
					for (i = 0; i < U_WIDTH; i++)
					{
						for (int k = 0; k < U_HEIGHT; k++)
						{
							enU_space[i*U_HEIGHT + k] = U_space[i*U_HEIGHT + k];
							enV_space[i*U_HEIGHT + k] = V_space[i*U_HEIGHT + k];
						}
					}
				}
				else
				{
					Log_enc_dec_Y(Y_space_CIF, Y0, EncDec, KEY, Y_WIDTH, Y_HEIGHT);
					for (i = 0; i < Y_WIDTH; i++)
						for (int k = 0; k < Y_HEIGHT; k++)
							enY_space_CIF[i*Y_HEIGHT + k] = Y_encrypted_CIF[i][k];
					for (i = 0; i < U_WIDTH; i++)
					{
						for (int k = 0; k < U_HEIGHT; k++)
						{
							enU_space_CIF[i*U_HEIGHT + k] = U_space_CIF[i*U_HEIGHT + k];
							enV_space_CIF[i*U_HEIGHT + k] = V_space_CIF[i*U_HEIGHT + k];
						}
					}
				}	
			}
			else if (flag == 2)		//U channel
			{
				if (Y_WIDTH == 176)
				{
					Log_enc_dec_U(U_space, Y0, EncDec, KEY, U_WIDTH, U_HEIGHT);
					for (i = 0; i < U_WIDTH; i++)
						for (int k = 0; k < U_HEIGHT; k++)
							enU_space[i*U_HEIGHT + k] = U_encrypted[i][k];
					for (i = 0; i < Y_WIDTH; i++)
						for (int k = 0; k < Y_HEIGHT; k++)
							enY_space[i*Y_HEIGHT + k] = Y_space[i*Y_HEIGHT + k];
					for (i = 0; i < U_WIDTH; i++)
						for (int k = 0; k < U_HEIGHT; k++)
							enV_space[i*U_HEIGHT + k] = V_space[i*U_HEIGHT + k];
				}
				else
				{
					Log_enc_dec_U(U_space_CIF, Y0, EncDec, KEY, U_WIDTH, U_HEIGHT);
					for (i = 0; i < U_WIDTH; i++)
						for (int k = 0; k < U_HEIGHT; k++)
							enU_space_CIF[i*U_HEIGHT + k] = U_encrypted_CIF[i][k];
					for (i = 0; i < Y_WIDTH; i++)
						for (int k = 0; k < Y_HEIGHT; k++)
							enY_space_CIF[i*Y_HEIGHT + k] = Y_space_CIF[i*Y_HEIGHT + k];
					for (i = 0; i < U_WIDTH; i++)
						for (int k = 0; k < U_HEIGHT; k++)
							enV_space_CIF[i*U_HEIGHT + k] = V_space_CIF[i*U_HEIGHT + k];
				}
			}
			else if (flag == 3)		//V channel
			{
				if (Y_WIDTH == 176)
				{
					Log_enc_dec_V(V_space, Y0, EncDec, KEY, U_WIDTH, U_HEIGHT);
					for (i = 0; i < V_WIDTH; i++)
						for (int k = 0; k < V_HEIGHT; k++)
							enV_space[i*U_HEIGHT + k] = V_encrypted[i][k];
					for (i = 0; i < Y_WIDTH; i++)
						for (int k = 0; k < Y_HEIGHT; k++)
							enY_space[i*Y_HEIGHT + k] = Y_space[i*Y_HEIGHT + k];
					for (i = 0; i < U_WIDTH; i++)
						for (int k = 0; k < U_HEIGHT; k++)
							enU_space[i*U_HEIGHT + k] = U_space[i*U_HEIGHT + k];
				}
				else
				{
					Log_enc_dec_V(V_space_CIF, Y0, EncDec, KEY, U_WIDTH, U_HEIGHT);
					for (i = 0; i < U_WIDTH; i++)
						for (int k = 0; k < U_HEIGHT; k++)
							enV_space_CIF[i*U_HEIGHT + k] = V_encrypted_CIF[i][k];
					for (i = 0; i < Y_WIDTH; i++)
						for (int k = 0; k < Y_HEIGHT; k++)
							enY_space_CIF[i*Y_HEIGHT + k] = Y_space_CIF[i*Y_HEIGHT + k];
					for (i = 0; i < U_WIDTH; i++)
						for (int k = 0; k < U_HEIGHT; k++)
							enU_space_CIF[i*U_HEIGHT + k] = U_space_CIF[i*U_HEIGHT + k];
				}
			}
		}
		if (Y_WIDTH == 176) {
			fwrite(enY_space, sizeof(unsigned char), sizeof(enY_space), out);
			fwrite(enU_space, sizeof(unsigned char), sizeof(enU_space), out);
			fwrite(enV_space, sizeof(unsigned char), sizeof(enV_space), out);
		}
		else
		{
			fwrite(enY_space_CIF, sizeof(unsigned char), sizeof(enY_space_CIF), out);
			fwrite(enU_space_CIF, sizeof(unsigned char), sizeof(enU_space_CIF), out);
			fwrite(enV_space_CIF, sizeof(unsigned char), sizeof(enV_space_CIF), out);
		}
	}
	fclose(in);
	fclose(out);
	return 1;
}