// Program1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"Picture.h"
#include<conio.h>

int _tmain(int argc, _TCHAR* argv[])
{
	Picture pic1;
	Picture pic2;
	//pic1.Read("E:\\Documents\\TIAN\\personal materials\\2018 SPRING\\Map Algebra\\TGProgram1\\Program1\\TGTest2.bmp");
	//pic1.Read("TGTest1.bmp");
	////pic2.Read("TGTest2.bmp");
	////pic1.Inverse();
	////pic1.Smooth("SmoothResult.bmp");
	////Overlap(pic2,pic1,"OverlapResult.bmp");
	//pic1.Eu5Transform("Eu5LocResult.bmp","Eu5DisResult.bmp");
	//pic1.Drct8Transform("Drct8LocResult.bmp","Drct8DisResult.bmp");
	//pic1.VoronoiTransform("Drct8VrnResult.bmp",pic1.dr8Tmp.LocMtx);
	//pic1.BufferPaint("BufferResult.bmp",pic1.eu5Tmp.DisMtx,6);

	pic1.Read("TGTest1.bmp");
	//pic2.Read("TGtest2.bmp");
	//pic1.Inverse();
	//pic1.Smooth("Test1SmthRslt.bmp");
	//Overlap(pic2,pic1,"Test1OvlpRslt.bmp");
	pic1.Drct8Transform("Drct8LocFinalResult.bmp","Drct8DisResult.bmp");
	//pic1.Eu5Transform("Eu5LocResult.bmp","Eu5DisResult.bmp");
	//pic1.GetCenterLine("ctlFinalResult.bmp");
	pic1.BufferPaint("BufferFinalResult.bmp",pic1.dr8Tmp.DisMtx,6);
	pic1.VoronoiTransform("Drct8VrnFinalResult.bmp",pic1.dr8Tmp.LocMtx);
	//pic1.VoronoiTransform("Drct8VrnResult.bmp",pic1.dr8Tmp.LocMtx);
	pic1.GetDelaunay("DlnFinalResult.bmp",pic1.dr8Tmp.LocMtx);
	return 0;
}

