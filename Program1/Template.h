#pragma once
#include <iostream>
#include "BMPPoint.h"
#define DnyCpct 100
class Template
{
public:
	Template(void);
	~Template(void);
	virtual int TmpSize() = 0;
	virtual float GetDis(int i) = 0;
	virtual int GetOffX(int i) = 0;
	virtual int GetOffY(int i) = 0;
	unsigned char** LocMtx;
	float** DisMtx;
};


class Eu5Tmp:public Template
{
public:
	float eu5Mtx[25];//距离场模板
	static int offX[25];
	static int offY[25];
	virtual int TmpSize() { return 25; };
	virtual float GetDis(int i) { return eu5Mtx[i]; };
	virtual int GetOffX(int i) { return offX[i]; };
	virtual int GetOffY(int i) { return offY[i];};

	Eu5Tmp()
	{
		for(int i=0;i<25;i++)
		{
			float x=GetOffX(i);
			float y=GetOffY(i);
			eu5Mtx[i]=sqrt(x*x+y*y);
		}
	}
};

class Drct8Tmp:public Template
{
public:
	static float drct8Mtx[37];//距离场模板
	static int offX[37];
	static int offY[37];

	virtual int TmpSize() { return 37; };
	virtual float GetDis(int i) { return drct8Mtx[i]; };
	virtual int GetOffX(int i) { return offX[i]; };
	virtual int GetOffY(int i) { return offY[i];};
};

class VrnTmp:public Template//也即十字模板
{
public:
	static float vrnMtx[5];//距离场模板
	static int offX[5];
	static int offY[5];
	virtual int TmpSize() { return 5; };
	virtual float GetDis(int i) { return vrnMtx[i]; };
	virtual int GetOffX(int i) { return offX[i]; };
	virtual int GetOffY(int i) { return offY[i];};
	BMPPoint color1[DnyCpct];
	BMPPoint color2[DnyCpct];
};