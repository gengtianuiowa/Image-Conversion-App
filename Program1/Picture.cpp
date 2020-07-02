#include "Picture.h"

#define MaxFloat 9999999999999.0
#define MinFloat 0.0000000001


//注意：静态变量不能在构造函数中初始化
int Eu5Tmp::offX[25]={-2,-1,0,1,2, -2,-1,0,1,2, -2,-1,0,1,2, -2,-1,0,1,2, -2,-1,0,1,2 };
int Eu5Tmp::offY[25]={-2,-2,-2,-2,-2, -1,-1,-1,-1,-1, 0,0,0,0,0, 1,1,1,1,1, 2,2,2,2,2 };
float Drct8Tmp::drct8Mtx[37]={3,3,3, 3,2,2,2,3, 3,2,2,1,2,2,3 ,3,2,1,0,1,2,3, 3,2,2,1,2,2,3, 3,2,2,2,3, 3,3,3};
int Drct8Tmp::offX[37]={-1,0,1, -2,-1,0,1,2, -3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -3,-2,-1,0,1,2,3, -2,-1,0,1,2, -1,0,1};
int Drct8Tmp::offY[37]={-3,-3,-3, -2,-2,-2,-2,-2, -1,-1,-1,-1,-1,-1,-1, 0,0,0,0,0,0,0, 1,1,1,1,1,1,1, 2,2,2,2,2, 3,3,3};
float VrnTmp::vrnMtx[5]={1, 1,0,1, 1};
int VrnTmp::offX[5]={0, -1,0,1, 0};
int VrnTmp::offY[5]={-1, 0,0,0, 1};
Picture::Picture()
{
	eu5Tmp=Eu5Tmp();
	dr8Tmp=Drct8Tmp();
	vrnTmp=VrnTmp();
}

bool Picture::Inverse()
{
	//开始之前，请务必认知认真阅读学习BMP格式存储数据的特点，和位图的基本知识
	//了解变量和其属性的含义，便于阅读以下代码
	//注意16色（只有16种颜色）位图与16位色位图的区别！

	//声明文件头
	//包含文件类型（bfType)；位图文件大小（bfSize）；偏移量（bfOffBits）
	BITMAPFILEHEADER srcBmpHead;

	//声明信息头
	//有biSize说明信息头需要的字节数，biWidth说明像素宽度，biHeight说明像素高度,biBitCount说明每个像素占多少比特等
	BITMAPINFOHEADER srcBmpInfo;


	unsigned char ColorTab[1024];

	FILE *srcBmp = fopen("TGtest1.bmp","rb");

	//读取文件头，存到srcBmpHeader中
	fread(&srcBmpHead, sizeof(BITMAPFILEHEADER),1,srcBmp);
	//读取信息头，存到srcBmpInfo中
	fread(&srcBmpInfo, sizeof(BITMAPINFOHEADER),1,srcBmp);
	//读取调色板。注意！这里默认长度为1024，即调色板中有1024/4即256种颜色，默认其为16位图了（若是24位图或32位图则没有调色板）。
	//其实更普适的做法应该在信息头中间读取调色板索引数，然后*4（因为有BRG和Alpha透明度四个值，每个占一个字节）
	fread(ColorTab, 1024,1,srcBmp);

	char m= srcBmpInfo.biBitCount;
	//开始读取位图数据
	//读取像素矩阵高度（单位为像素）
	unsigned MtxHeight = srcBmpInfo.biHeight;
	//读取像素矩阵宽度（单位为像素）
	unsigned MtxWidth = srcBmpInfo.biWidth;
	//BMP每行的数据长度必为4的倍数（原因是为了和windows默认的最小4字节的扫描方式一致，对于扫描速度有增益），其他位置是由0填充的
	unsigned BufWidth = (MtxWidth+3) /4 *4;

	//unsigned char 占一个字节
	unsigned char *srcLineBuf = new unsigned char[BufWidth];
	//动态生成二维数组destBmpMtx
	unsigned char **destBmpMtx = new unsigned char *[MtxHeight];

	unsigned i,j;
	for (i=0; i < MtxHeight; i++)
	{
		//每次读取增长宽度（BufWidth）的长度
		fread(srcLineBuf, BufWidth, 1, srcBmp);
		//生成动态数组的一行
		destBmpMtx[i] = new unsigned char[BufWidth];

		for (j=0; j < MtxWidth; j++)
		{
			//针对此行数据的每一个像素进行扫描，若不是黑色就进行反色
			//0x开头表示16进制的数据，FF两位16进制，刚好32位即为一个字节，F可看做1111（16），FF即（1111 1111）16*16=256
			//if (srcLineBuf[j] == 0xFF) 
			//	destBmpMtx[i][j] = 0xFF;
			//反色公式为255-此位置像素的值
			destBmpMtx[i][j] = 255 - srcLineBuf[j];
		}
	}

	//事先在画图中画好一个BMP，打开该文件
	FILE *destBmp = fopen("result1.bmp","wb");

	//把此文件的文件头和信息头写入新的文件
	fwrite(&srcBmpHead, sizeof(BITMAPFILEHEADER),1,destBmp);
	fwrite(&srcBmpInfo, sizeof(BITMAPINFOHEADER),1,destBmp);
	//调色板也存入后面
	fwrite(ColorTab, 1024,1,destBmp);

	//开始存位图数据
	for (i=0; i<MtxHeight; i++)
	{
		fwrite(destBmpMtx[i], BufWidth, 1, destBmp);
		delete []destBmpMtx[i];
		destBmpMtx[i] = NULL;
	}

	//析构和关闭文件
	delete []destBmpMtx;
	delete []srcLineBuf;
	srcLineBuf = NULL;
	return true;
}

bool Picture::Smooth(const char * OutputBmpFileName)
{
	
	if(this->readStatus==false)
	{
		cout<<"还未读入文件"<<endl;
		return false;
	}

	this->BmpHeader;
	this->BmpInfo;
	this->ColorTab;
	
	//2.数据处理
	int tmp=0;
	//先赋值一个和datamtx一模一样的矩阵dataoutmtx
	unsigned char** DataOutMtx= new unsigned char*[BmpHeight];
	for(int i=0;i<BmpHeight;i++)
	{
		DataOutMtx[i]=new unsigned char[MtxWidth];
		for(int j=0;j<MtxWidth;j++)
		{
			DataOutMtx[i][j]=this->DataMtx[i][j];
		}
	}

	//对这个矩阵进行平滑
	for(int i=1;i<BmpHeight-1;i++)
		for(int j=1;j<BmpWidth-1;j++)
		{
			//可以用循环代替模板
				for(int x=-1;x<2;x++)
					for(int y=-1;y<2;y++)
					{
						if((i+x>=0&&i+x<BmpHeight)&&(j+y>=0&&j+y<BmpWidth)) 
						tmp+=DataMtx[i+x][j+y];
					}
					DataOutMtx[i][j]=tmp/9;//Out的作用是不改变原数组的情况下，更新进入新的元素
					tmp=0;
		}



		//3.把结果存储到一个bmp中
		if(this->Write(OutputBmpFileName,DataOutMtx))
			return true;
		else
			return false;
}

bool Picture::Eu5Transform(const char * LocFileName,const char* DisFileName)
{
	if(this->readStatus==false)
	{
		cout<<"文件还未读入"<<endl;
		return false;
	}

	//复制一个矩阵到LocMtx
	unsigned char ** LocMtx=new unsigned char* [this->BmpHeight];
	for(int i=0;i<this->BmpHeight;i++)
	{
		LocMtx[i]=new unsigned char [this->MtxWidth];
		for(int j=0;j<this->MtxWidth;j++)
			LocMtx[i][j]=this->DataMtx[i][j];
	}

	//初始化距离场
	float ** DisMtx = new float *[this->BmpHeight];
	for(int i=0;i<this->BmpHeight;i++)
	{
		DisMtx[i]=new float [this->BmpWidth];
		for(int j=0;j<this->BmpWidth;j++)
		{
		if(this->DataMtx[i][j]==255)
			DisMtx[i][j]=MaxFloat;
		else
			DisMtx[i][j]=0;
		}
	}


	// 模板距离变换
	// 1.从左下到右上方向
	for (int i=0; i<this->BmpHeight; i++)
		for (int j=0; j<this->BmpWidth; j++)
		{
			if (fabs(DisMtx[i][j]) < MinFloat) continue;
			float MinDis = DisMtx[i][j];

			for (int k=0; k<=eu5Tmp.TmpSize()/2+1; k++)
			{
				int OffX = eu5Tmp.GetOffX(k);
				int OffY = eu5Tmp.GetOffY(k);
				float TmpDis =eu5Tmp.GetDis(k);

				int x = j + OffX;
				int y = i + OffY;

				if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//模板必须全部在图像内

				//if (fabs(DisMtx[y][x]-MaxFloat)<MinFloat) continue; // 该位置如果是无穷大不可计算

				//1、MixDis无穷大：若（y,x）是有颜色的，右边即为TmpDis，否则则为无穷大+TmpDis，大于无穷大
				//2、MixDis被赋值过：看是否有其他更近的像素点，MixDis0始终为其中最小的值
				if (MinDis > DisMtx[y][x] + TmpDis)
				{
					MinDis =DisMtx[y][x] + TmpDis;//若为TmpDis则赋予最小值，而且经过多番比较之后一定得到的是最小的距离
					DisMtx[i][j] = MinDis;//赋给距离场
					LocMtx[i][j] = LocMtx[y][x];//修改图像，（y,x）必为最近的有颜色的点的坐标，颜色值赋予这个点
				}
			}
		}

		// 模板距离变换
		// 2.从右上到左下方向
		for (int i=this->BmpHeight-1; i>=0; i--)
			for (int j=this->BmpWidth-1; j>=0; j--)
			{
				if (fabs(DisMtx[i][j]) < MinFloat) continue;
				float MinDis = DisMtx[i][j];

				for (int k=eu5Tmp.TmpSize()/2+1; k<eu5Tmp.TmpSize(); k++)
				{
					int OffX = eu5Tmp.GetOffX(k);
					int OffY = eu5Tmp.GetOffY(k);
					float TmpDis =eu5Tmp.GetDis(k);

					int x = j + OffX;
					int y = i + OffY;

					if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//模板此时计算的点必须在图像内

					//if (fabs(DisMtx[y][x]-MaxFloat)<MinFloat) continue; // 该位置如果是无穷大不可计算

					//1、MixDis无穷大：若（y,x）是有颜色的，右边即为TmpDis，否则则为无穷大+TmpDis，大于无穷大
					//2、MixDis被赋值过：看是否有其他更近的像素点，MixDis0始终为其中最小的值
					if (MinDis > DisMtx[y][x] + TmpDis)
					{
						MinDis =DisMtx[y][x] + TmpDis;//若为TmpDis则赋予最小值，而且经过多番比较之后一定得到的是最小的距离
						DisMtx[i][j] = MinDis;//赋给距离场
						LocMtx[i][j] = LocMtx[y][x];//修改图像，（y,x）必为最近的有颜色的点的坐标，颜色值赋予分配场
					}
				}
			}
			this->eu5Tmp.LocMtx=LocMtx;
			this->eu5Tmp.DisMtx=DisMtx;

			this->Write(DisFileName,DisMtx);//注意！距离场是以float形式储存的，所以要转换成32位图输出，因为一个float占4个字节，和32位图一个像素对应
			this->Write(LocFileName,LocMtx);//分配场仍然是以256色图输出的
}

bool Picture::Drct8Transform(const char * LocFileName,const char* DisFileName)
{
	if(this->readStatus==false)
	{
		cout<<"文件还未读入"<<endl;
		return false;
	}

	//复制一个矩阵到LocMtx
	unsigned char ** LocMtx=new unsigned char* [this->BmpHeight];
	for(int i=0;i<this->BmpHeight;i++)
	{
		LocMtx[i]=new unsigned char [this->MtxWidth];
		for(int j=0;j<this->MtxWidth;j++)
			LocMtx[i][j]=this->DataMtx[i][j];
	}

	//初始化距离场
	float ** DisMtx = new float *[this->BmpHeight];
	for(int i=0;i<this->BmpHeight;i++)
	{
		DisMtx[i]=new float [this->BmpWidth];
		for(int j=0;j<this->BmpWidth;j++)
		{
			if(this->DataMtx[i][j]==255)
				DisMtx[i][j]=MaxFloat;
			else
				DisMtx[i][j]=0;
		}
	}


	// 模板距离变换
	// 1.从左下到右上方向
	for (int i=0; i<this->BmpHeight; i++)
		for (int j=0; j<this->BmpWidth; j++)
		{
			if (fabs(DisMtx[i][j]) < MinFloat) continue;
			float MinDis = DisMtx[i][j];

			for (int k=0; k<=dr8Tmp.TmpSize()/2+1; k++)
			{
				int OffX = dr8Tmp.GetOffX(k);
				int OffY =dr8Tmp.GetOffY(k);
				float TmpDis =dr8Tmp.GetDis(k);

				int x = j + OffX;
				int y = i + OffY;

				if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//模板必须全部在图像内

				//if (fabs(DisMtx[y][x]-MaxFloat)<MinFloat) continue; // 该位置如果是无穷大不可计算

				//1、MixDis无穷大：若（y,x）是有颜色的，右边即为TmpDis，否则则为无穷大+TmpDis，大于无穷大
				//2、MixDis被赋值过：看是否有其他更近的像素点，MixDis0始终为其中最小的值
				if (MinDis > DisMtx[y][x] + TmpDis)
				{
					MinDis =DisMtx[y][x] + TmpDis;//若为TmpDis则赋予最小值，而且经过多番比较之后一定得到的是最小的距离
					DisMtx[i][j] = MinDis;//赋给距离场
					LocMtx[i][j] = LocMtx[y][x];//修改图像，（y,x）必为最近的有颜色的点的坐标，颜色值赋予这个点
				}
			}
		}

		// 模板距离变换
		// 2.从右上到左下方向
		for (int i=this->BmpHeight-1; i>=0; i--)
			for (int j=this->BmpWidth-1; j>=0; j--)
			{
				if (fabs(DisMtx[i][j]) < MinFloat) continue;
				float MinDis = DisMtx[i][j];

				for (int k=dr8Tmp.TmpSize()/2+1; k<dr8Tmp.TmpSize(); k++)
				{
					int OffX = dr8Tmp.GetOffX(k);
					int OffY = dr8Tmp.GetOffY(k);
					float TmpDis =dr8Tmp.GetDis(k);

					int x = j + OffX;
					int y = i + OffY;

					if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//模板必须全部在图像内

					//if (fabs(DisMtx[y][x]-MaxFloat)<MinFloat) continue; // 该位置如果是无穷大不可计算

					//1、MinDis无穷大：若（y,x）是有颜色的，右边即为TmpDis，否则则为无穷大+TmpDis，大于无穷大
					//2、MinDis被赋值过：看是否有其他更近的像素点，MinDis0始终为其中最小的值
					if (MinDis > DisMtx[y][x] + TmpDis)
					{
						MinDis =DisMtx[y][x] + TmpDis;//若为TmpDis则赋予最小值，而且经过多番比较之后一定得到的是最小的距离
						DisMtx[i][j] = MinDis;//赋给距离场
						LocMtx[i][j] = LocMtx[y][x];//修改图像，（y,x）必为最近的有颜色的点的坐标，颜色值赋予分配场
					}
				}
			}
			this->dr8Tmp.LocMtx=LocMtx;
			this->dr8Tmp.DisMtx=DisMtx;

			this->Write(DisFileName,DisMtx);//注意！距离场是以float形式储存的，所以要转换成32位图输出，因为一个float占4个字节，和32位图一个像素对应
			this->Write(LocFileName,LocMtx);//分配场仍然是以256色图输出的
}

bool Picture::VoronoiTransform(const char * OutputBmpFileName,unsigned char** LocMtx)
{
	if(LocMtx==NULL)
		return false;

	//下面开始生成Voronoi，不用扫描两次
	unsigned char** tmpMtx=new unsigned char*[this->BmpHeight];
	for(int i=0;i<this->BmpHeight;i++)
	{
		tmpMtx[i]=new unsigned char[this->MtxWidth];
		for(int j=0;j<this->MtxWidth;j++)
			for (int k=0; k<=vrnTmp.TmpSize(); k++)
			{
				int OffX = vrnTmp.GetOffX(k);
				int OffY = vrnTmp.GetOffY(k);

				int x = j + OffX;
				int y = i + OffY;

				if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//模板必须全部在图像内

				//代入模板判断，是边界点的，设为100
				if(LocMtx[y][x]!=LocMtx[i][j])
				{
					tmpMtx[i][j]=0;
					//注意这个break！一旦发现是边界点，停止套用模板，否则有重设回255的可能
					break;
				}
				else tmpMtx[i][j]=255;
			}
	}
	this->Write(OutputBmpFileName,tmpMtx);//分配场仍然是以256色图输出的
	return true;
}

bool Picture::BufferPaint(const char * OutputBmpFileName,float** EuDisMtx,float bufferDis)
{
	if(EuDisMtx==NULL)
		return false;

	//下面开始生成Voronoi，不用扫描两次
	unsigned char** tmpMtx=new unsigned char* [this->BmpHeight];
	for(int i=0;i<this->BmpHeight;i++)
	{
		tmpMtx[i]=new unsigned char[this->MtxWidth];
		for(int j=0;j<this->MtxWidth;j++)
		{
			//判断该像素点的距离是否等于缓冲区距离，若是则画黑线，不是则设置成白色
			if(abs(EuDisMtx[i][j]-bufferDis)<0.5)
					tmpMtx[i][j]=0;
			else if(EuDisMtx[i][j]==0)
				tmpMtx[i][j]=this->DataMtx[i][j]; 
			else tmpMtx[i][j]=255;
		}
	}
	Write(OutputBmpFileName,tmpMtx);
	return true;
}

bool Picture::GetDelaunay(const char* OutputBmpFileName,unsigned char** LocMtx)
{
	//请务必确认传入的是点数据，否则无法正确生成Delaunay
	BMPPoint colorPoint[DnyCpct];
	int pointCount=0;
	//统计生成元个数
	for(int i=0;i<this->BmpHeight;i++)
		for(int j=0;j<this->BmpWidth;j++)
		{
			if(this->DataMtx[i][j]!=255)
			{					
				colorPoint[pointCount].X=j;
				colorPoint[pointCount].Y=i;
				colorPoint[pointCount].color=DataMtx[i][j];
				pointCount++;
			}
		}

	if(LocMtx==NULL)
		return false;

	//pointPair为统计点对的数组
	int pointPairCount=0;

	//开始以十字模板扫描所有图像
	for(int i=0;i<this->BmpHeight;i++)
	{
		for(int j=0;j<this->BmpWidth;j++)
			for (int k=0; k<vrnTmp.TmpSize(); k++)
			{
				int OffX = vrnTmp.GetOffX(k);
				int OffY = vrnTmp.GetOffY(k);

				int x = j + OffX;
				int y = i + OffY;

				if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//模板必须全部在图像内

				if(LocMtx[y][x]!=LocMtx[i][j])//发现边界
				{
					bool same=true;
					for(int m=0;m<=pointPairCount;m++)
					{
						//检测这对颜色是否已经存过
						if(this->vrnTmp.color1[m].color==LocMtx[i][j])
						{
							if (this->vrnTmp.color2[m].color==LocMtx[y][x])
							{
								same=true;
								break;
							}
						}
						else if(this->vrnTmp.color2[m].color==LocMtx[i][j])
						{
							if (this->vrnTmp.color1[m].color==LocMtx[y][x])
							{
								same=true;
								break;
							}
						}
						else
							same=false;
					}
					if(same==false)//若没有被存过
					{
						//记录这些点对在color1，color2中（vrnTmp的属性）
						this->vrnTmp.color1[pointPairCount].color=LocMtx[i][j];
						this->vrnTmp.color2[pointPairCount].color=LocMtx[y][x];
						for(int p=0;p<pointCount;p++)
						{
							if(colorPoint[p].color==LocMtx[i][j])
							{
								this->vrnTmp.color1[pointPairCount].X=colorPoint[p].X;
								this->vrnTmp.color1[pointPairCount].Y=colorPoint[p].Y;
							}
							if(colorPoint[p].color==LocMtx[y][x])
							{
								this->vrnTmp.color2[pointPairCount].X=colorPoint[p].X;
								this->vrnTmp.color2[pointPairCount].Y=colorPoint[p].Y;
							}
						}
						//cout<<int(this->vrnTmp.color1[pointPairCount].color)<<" "<<int(this->vrnTmp.color2[pointPairCount].color)<<endl;
						//cout<<"x1:"<<int(this->vrnTmp.color1[pointPairCount].X)<<"y1:"<<int(this->vrnTmp.color1[pointPairCount].Y)<<endl;
						//cout<<"x2:"<<int(this->vrnTmp.color2[pointPairCount].X)<<"y2:"<<int(this->vrnTmp.color2[pointPairCount].Y)<<endl;
						pointPairCount++;
					}
					break;//跳出k循环
				}
			}
	}
	for(int i=0;i<pointPairCount;i++)
	{
		//根据记录的点对调用Bresenham算法
		BhmDrawLine(this->vrnTmp.color1[i],this->vrnTmp.color2[i],this->DataMtx);
	}	

	Write(OutputBmpFileName,DataMtx);

}

bool Picture::GetCenterLine(const char* OutputBmpFileName)
{
	if(this->VoronoiTransform(OutputBmpFileName,this->dr8Tmp.LocMtx))
	{
		Picture ptemp;
		ptemp.Read(OutputBmpFileName);
		int maxX=-1,maxY=-1,minX=999999,minY=999999;
		for(int i=0;i<this->BmpHeight;i++)
			for(int j=0;j<this->MtxWidth;j++)
			{
				if(this->DataMtx[i][j]!=255)
				{
					if(i>maxY)
						maxY=i;
					if(j>maxX)
						maxX=j;
					if(i<minY)
						minY=i;
					if(j<minX)
						minX=j;
				}
			}

			for(int i=0;i<this->BmpHeight;i++)
				for(int j=0;j<this->MtxWidth;j++)
				{
					if(!(minY<=i&&i<=maxY&&minX<=j&&j<=maxX))
						ptemp.DataMtx[i][j]=255;
				}
			Write(OutputBmpFileName,ptemp.DataMtx);
			return true;
	}else
		return false;
}

bool Picture::Read(const char* InputBmpFileName)
{
	this->InputBmpFileName=InputBmpFileName;
	InputBmp=fopen(InputBmpFileName,"rb");
	if(InputBmp ==NULL) return false;


	//原理相同，不赘述
	fread(&BmpHeader,sizeof(BITMAPFILEHEADER),1,InputBmp);
	fread(&BmpInfo,sizeof(BITMAPINFOHEADER),1,InputBmp);
	fread(ColorTab,1024,1,InputBmp);

	BmpHeight= BmpInfo.biHeight;
	MtxWidth=(BmpInfo.biWidth+3)/4*4;
	BmpWidth=BmpInfo.biWidth;

	//开始读数据
	DataMtx=new unsigned char *[BmpHeight];
	for(int i=0;i<BmpHeight;i++)
	{
		//还是动态创建二元数组
		DataMtx[i]= new unsigned char [MtxWidth];
		fread(DataMtx[i],MtxWidth,1,InputBmp);
	}
	fclose(InputBmp);//至此第一个文件读完了
	readStatus=true;
	return true;
}

bool Picture::Write(const char* OutputBmpFileName,unsigned char** DataMtx)
{
	FILE* OutBmp=fopen(OutputBmpFileName,"wb");
	if(OutBmp ==NULL) return false;
	fwrite(&BmpHeader,sizeof(BITMAPFILEHEADER),1,OutBmp);
	fwrite(&BmpInfo,sizeof(BITMAPINFOHEADER),1,OutBmp);
	fwrite(ColorTab,1024,1,OutBmp);

	for(int i=0;i<BmpHeight;i++)
		fwrite(DataMtx[i],sizeof(unsigned char),this->MtxWidth,OutBmp);

	fclose(OutBmp);
	return true;
}

bool Picture::Write(const char* OutputBmpFileName,float** DataMtx)
{
	BITMAPFILEHEADER IdxFileHead; // 32位位图头结构
	BITMAPINFOHEADER IdxFileInfo;

	IdxFileHead.bfType = this->BmpHeader.bfType; 
	IdxFileHead.bfSize = 54 + this->BmpInfo.biWidth * 4 * this->BmpInfo.biHeight;//
	IdxFileHead.bfReserved1 = 0;
	IdxFileHead.bfReserved2 = 0;
	IdxFileHead.bfOffBits = 54;//

	IdxFileInfo.biSize = 40;
	IdxFileInfo.biWidth = this->BmpInfo.biWidth;//
	IdxFileInfo.biHeight = this->BmpInfo.biHeight;//
	IdxFileInfo.biPlanes = 1;
	IdxFileInfo.biBitCount = 32;//
	IdxFileInfo.biCompression = 0;
	IdxFileInfo.biSizeImage = 0;
	IdxFileInfo.biXPelsPerMeter = 0;
	IdxFileInfo.biYPelsPerMeter = 0;
	IdxFileInfo.biClrUsed = 0;
	IdxFileInfo.biClrImportant = 0;

	FILE * bmpWrite = fopen(OutputBmpFileName,"wb");
	if(bmpWrite ==NULL) return false;

	fwrite(&IdxFileHead, sizeof(BITMAPFILEHEADER),1,bmpWrite);
	fwrite(&IdxFileInfo, sizeof(BITMAPINFOHEADER),1,bmpWrite);
	for (int y=0; y<this->BmpHeight; y++)
	{
		fwrite(DataMtx[y], sizeof(float), this->BmpWidth, bmpWrite);
	}
	fclose(bmpWrite);
	return true;
}


bool Overlap(Picture pic1,Picture pic2,const char * OutputBmpFileName)
{
	if(!pic1.readStatus||!pic2.readStatus)
	{
		cout<<"有文件还未读取"<<endl;
		return false;
	}

	int borderW=min(pic1.MtxWidth,pic2.MtxWidth);
	int borderH=min(pic1.BmpHeight,pic2.BmpHeight);
	Picture pic3= Picture();

	pic3.DataMtx=new unsigned char *[pic1.BmpHeight];
	for(int i=0; i<pic1.BmpHeight;i++)
	{
		pic3.DataMtx[i]=new unsigned char [pic1.MtxWidth];
		for(int j=0;j<pic1.MtxWidth;j++)
			pic3.DataMtx[i][j]=pic1.DataMtx[i][j];
	}

	//pic1.Write("OverlapResultTest1.bmp",pic3.DataMtx);

	for(int i=0; i< borderH;i++)
	for(int j=0;j<borderW;j++)
		pic3.DataMtx[i][j]=(pic1.DataMtx[i][j]+pic2.DataMtx[i][j])/2;

	pic1.Write(OutputBmpFileName,pic3.DataMtx);
	return true;
}
