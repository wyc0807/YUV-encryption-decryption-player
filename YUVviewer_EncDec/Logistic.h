#if !defined(AFX_AES_H__6BDD3760_BDE8_4C42_85EE_6F7A434B81C4__INCLUDED_)
#define AFX_AES_H__6BDD3760_BDE8_4C42_85EE_6F7A434B81C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"
#define U_HEIGHT_QCIF                   72
#define V_HEIGHT_QCIF                   72
#define U_WIDTH_QCIF                    88
#define V_WIDTH_QCIF                    88
#define Y_HEIGHT_QCIF                   144
#define U_HEIGHT_CIF                    144
#define V_HEIGHT_CIF                    144
#define Y_WIDTH_QCIF                    176
#define U_WIDTH_CIF                     176
#define V_WIDTH_CIF                     176
#define Y_HEIGHT_CIF                    288
#define Y_WIDTH_CIF                     352

class Logistic
{
public:
	virtual ~Logistic();
	int* getP(double x);
	double getX0(int K[]);
	double getY0(int K[], int P[]);
	int Logistic_chaos_map(char *file_in, int *KEY, int flag, int Width, int Height,int EncDec);
	void Log_enc_dec_Y(unsigned char* R, double Y0, int ED, int *key, int Height, int Width);
	void Log_enc_dec_U(unsigned char* R, double Y0, int ED, int *key, int Height, int Width);
	void Log_enc_dec_V(unsigned char* R, double Y0, int ED, int *key, int Height, int Width);
private:
	unsigned char Y_space[Y_WIDTH_QCIF*Y_HEIGHT_QCIF];
	unsigned char U_space[U_WIDTH_QCIF*U_HEIGHT_QCIF];
	unsigned char V_space[V_WIDTH_QCIF*V_HEIGHT_QCIF];
	unsigned char Y_encrypted[Y_WIDTH_QCIF][Y_HEIGHT_QCIF];
	unsigned char U_encrypted[U_WIDTH_QCIF][U_HEIGHT_QCIF];
	unsigned char V_encrypted[V_WIDTH_QCIF][V_HEIGHT_QCIF];
	unsigned char enY_space[Y_WIDTH_QCIF*Y_HEIGHT_QCIF];
	unsigned char enU_space[U_WIDTH_QCIF*U_HEIGHT_QCIF];
	unsigned char enV_space[V_WIDTH_QCIF*V_HEIGHT_QCIF];
	
	unsigned char Y_space_CIF[Y_WIDTH_CIF*Y_HEIGHT_CIF];
	unsigned char U_space_CIF[U_WIDTH_CIF*U_HEIGHT_CIF];
	unsigned char V_space_CIF[V_WIDTH_CIF*V_HEIGHT_CIF];
	unsigned char Y_encrypted_CIF[Y_WIDTH_CIF][Y_HEIGHT_CIF];
	unsigned char U_encrypted_CIF[U_WIDTH_CIF][U_HEIGHT_CIF];
	unsigned char V_encrypted_CIF[V_WIDTH_CIF][V_HEIGHT_CIF];
	unsigned char enY_space_CIF[Y_WIDTH_CIF*Y_HEIGHT_CIF];
	unsigned char enU_space_CIF[U_WIDTH_CIF*U_HEIGHT_CIF];
	unsigned char enV_space_CIF[V_WIDTH_CIF*V_HEIGHT_CIF];
	
	int Y_WIDTH = 176;
	int Y_HEIGHT = 144;
	int U_WIDTH = 88;
	int U_HEIGHT = 72;
	int V_WIDTH = 88;
	int V_HEIGHT = 72;
	int *getBin(int data);
	double *generY0(double y, int K);	
};

#endif
