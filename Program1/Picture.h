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
	//下面是所有文件常用的基本属性，Read之后才可获得
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

	//可以使用的模板
	Eu5Tmp eu5Tmp;
	Drct8Tmp dr8Tmp;
	VrnTmp vrnTmp;
	
	Picture();

	bool Read(const char* InputBmpFileName);//读取文件
	bool Write(const char* InputBmpFileName,unsigned char** DataMtx);//完成操作后，写成文件
	bool Write(const char* OutputBmpFileName,float** DataMtx);
	bool Inverse();//反色变换
	bool Smooth(const char * OutputBmpFileName);//平滑变换
	bool Eu5Transform(const char * LocFileName,const char* DisFileName);//欧式5*5模板变换
	bool Drct8Transform(const char * LocFileName,const char* DisFileName);//8方向距离变换
	bool VoronoiTransform(const char * OutputBmpFileName,unsigned char** LocMtx);//Voronoi生成
	bool BufferPaint(const char * OutputBmpFileName,float** EuDisMtx,float bufferDis);
	bool GetDelaunay(const char* OutputBmpFileName,unsigned char** LocMtx);
	bool GetCenterLine(const char* OutputBmpFileName);
};

bool Overlap(Picture pic1,Picture pic2,const char * OutputBmpFileName);//叠置变换
