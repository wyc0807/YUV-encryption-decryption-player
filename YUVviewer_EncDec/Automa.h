#include "stdafx.h"
#include "Resource.h"
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
class Automa
{
public:
	virtual ~Automa();
	unsigned char* Automa::ECADEC_Y(unsigned char *R, int key, int Width, int Height);
	unsigned char* Automa::ECADEC_U(unsigned char *R, int key, int Width, int Height);
	unsigned char* Automa::ECADEC_V(unsigned char *R, int key, int Width, int Height);
	int Encrypt(char *file_in, int *key, int flag, int Width, int Height);
private:

	unsigned char Y_space[Y_WIDTH_QCIF*Y_HEIGHT_QCIF];
	unsigned char U_space[U_WIDTH_QCIF*U_HEIGHT_QCIF];
	unsigned char V_space[V_WIDTH_QCIF*V_HEIGHT_QCIF];
	unsigned char enY_space[Y_WIDTH_QCIF*Y_HEIGHT_QCIF];
	unsigned char enU_space[U_WIDTH_QCIF*U_HEIGHT_QCIF];
	unsigned char enV_space[V_WIDTH_QCIF*V_HEIGHT_QCIF];

	unsigned char Y_space_CIF[Y_WIDTH_CIF*Y_HEIGHT_CIF];
	unsigned char U_space_CIF[U_WIDTH_CIF*U_HEIGHT_CIF];
	unsigned char V_space_CIF[V_WIDTH_CIF*V_HEIGHT_CIF];
	unsigned char enY_space_CIF[Y_WIDTH_CIF*Y_HEIGHT_CIF];
	unsigned char enU_space_CIF[U_WIDTH_CIF*U_HEIGHT_CIF];
	unsigned char enV_space_CIF[V_WIDTH_CIF*V_HEIGHT_CIF];
	int Y_WIDTH = 176;
	int Y_HEIGHT = 144;
	int U_WIDTH = 88;
	int U_HEIGHT = 72;
	int V_WIDTH = 88;
	int V_HEIGHT = 72;
	void ECAENC_Y(unsigned char R[], int key, int Width, int Height);
	void ECAENC_U(unsigned char R[], int key, int Width, int Height);
	void ECAENC_V(unsigned char R[], int key, int Width, int Height);

};

