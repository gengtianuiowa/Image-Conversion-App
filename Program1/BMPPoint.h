#pragma once
#include "math.h"
class BMPPoint
{
public:
	BMPPoint(void);
	~BMPPoint(void);
	int X;//�õ�����ͼ��ĺ�����
	int Y;//�õ�����ͼ���������
	unsigned char color;//�õ���ɫֵ
};
bool BhmDrawLine(BMPPoint st,BMPPoint ed,unsigned char** DataMtx);

