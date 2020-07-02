#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include "stdafx.h"
#include "Template.h"
#include "BMPPoint.h"
using namespace std;
class Picture
{

public:
	//�����������ļ����õĻ������ԣ�Read֮��ſɻ��
	const char* InputBmpFileName;
	FILE* InputBmp;
	BITMAPFILEHEADER BmpHeader;
	BITMAPINFOHEADER BmpInfo;
	unsigned char ColorTab[4*256];
	unsigned char ** DataMtx;
	unsigned BmpHeight;
	unsigned MtxWidth;
	unsigned BmpWidth;
	bool readStatus;

	//����ʹ�õ�ģ��
	Eu5Tmp eu5Tmp;
	Drct8Tmp dr8Tmp;
	VrnTmp vrnTmp;
	
	Picture();

	bool Read(const char* InputBmpFileName);//��ȡ�ļ�
	bool Write(const char* InputBmpFileName,unsigned char** DataMtx);//��ɲ�����д���ļ�
	bool Write(const char* OutputBmpFileName,float** DataMtx);
	bool Inverse();//��ɫ�任
	bool Smooth(const char * OutputBmpFileName);//ƽ���任
	bool Eu5Transform(const char * LocFileName,const char* DisFileName);//ŷʽ5*5ģ��任
	bool Drct8Transform(const char * LocFileName,const char* DisFileName);//8�������任
	bool VoronoiTransform(const char * OutputBmpFileName,unsigned char** LocMtx);//Voronoi����
	bool BufferPaint(const char * OutputBmpFileName,float** EuDisMtx,float bufferDis);
	bool GetDelaunay(const char* OutputBmpFileName,unsigned char** LocMtx);
	bool GetCenterLine(const char* OutputBmpFileName);
};

bool Overlap(Picture pic1,Picture pic2,const char * OutputBmpFileName);//���ñ任
