#pragma once
#include "math.h"
class BMPPoint
{
public:
	BMPPoint(void);
	~BMPPoint(void);
	int X;//该点所在图像的横坐标
	int Y;//该点所在图像的纵坐标
	unsigned char color;//该点颜色值
};
bool BhmDrawLine(BMPPoint st,BMPPoint ed,unsigned char** DataMtx);

