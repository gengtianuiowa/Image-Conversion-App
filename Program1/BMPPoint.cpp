#include "StdAfx.h"
#include "BMPPoint.h"

BMPPoint::BMPPoint(void)
{
}


BMPPoint::~BMPPoint(void)
{
}

bool BhmDrawLine(BMPPoint st,BMPPoint ed,unsigned char** DataMtx)
{
	int dx=st.X-ed.X;
	int dy=st.Y-ed.Y;
	double dx_t=double(dx);
	double dy_t=double(dy);
	double k=dy_t/dx_t;
	double y_tmp;
	int t=0,u=0;
	if(ed.X-st.X>0)
		t=1;
	else if(ed.X-st.X<0)
		t=-1;
	if(ed.Y-st.Y>0)
		u=1;
	else if(ed.Y-st.Y<0)
		u=-1;
	int formerPointy=0;
	for(int i=0;i<=abs(ed.X-st.X);i++)
	{
		y_tmp=st.Y+i*t*k;
		if(y_tmp-floor(y_tmp)<=0.5)//离下最近
		{
			int yt=int(floor(y_tmp));
			if(i!=0)
				//防止跨度过大，两个生成点之前会被填充满像素
				for(int m=0;m<abs(yt-formerPointy);m++)
				{
					//由于是逐x（数组第二个元）移动，当y跨度太大时会出现空白，此时要以相同的x填充y，直到y=y'-1
					DataMtx[formerPointy+u*m][st.X+t*(i-1)]=0;
				}
			DataMtx[yt][st.X+t*i]=0;
			formerPointy=yt;
		}
		else//离上最近
		{
			int yt=int(ceil(y_tmp));
			if(i!=0)
				//防止跨度过大，两个生成点之前会被填充满像素
				for(int m=0;m<abs(yt-formerPointy);m++)
				{
					//由于是逐x（数组第二个元）移动，当y跨度太大时会出现空白，此时要以相同的x填充y，直到y=y'-1
					DataMtx[formerPointy+u*m][st.X+t*(i-1)]=0;
				}
				DataMtx[yt][st.X+t*i]=0;
				formerPointy=yt;
			formerPointy=yt;
		}
	}
	return true;
}
