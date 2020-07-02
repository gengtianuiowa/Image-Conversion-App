#include "Picture.h"

#define MaxFloat 9999999999999.0
#define MinFloat 0.0000000001


//ע�⣺��̬���������ڹ��캯���г�ʼ��
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
	//��ʼ֮ǰ���������֪�����Ķ�ѧϰBMP��ʽ�洢���ݵ��ص㣬��λͼ�Ļ���֪ʶ
	//�˽�����������Եĺ��壬�����Ķ����´���
	//ע��16ɫ��ֻ��16����ɫ��λͼ��16λɫλͼ������

	//�����ļ�ͷ
	//�����ļ����ͣ�bfType)��λͼ�ļ���С��bfSize����ƫ������bfOffBits��
	BITMAPFILEHEADER srcBmpHead;

	//������Ϣͷ
	//��biSize˵����Ϣͷ��Ҫ���ֽ�����biWidth˵�����ؿ�ȣ�biHeight˵�����ظ߶�,biBitCount˵��ÿ������ռ���ٱ��ص�
	BITMAPINFOHEADER srcBmpInfo;


	unsigned char ColorTab[1024];

	FILE *srcBmp = fopen("TGtest1.bmp","rb");

	//��ȡ�ļ�ͷ���浽srcBmpHeader��
	fread(&srcBmpHead, sizeof(BITMAPFILEHEADER),1,srcBmp);
	//��ȡ��Ϣͷ���浽srcBmpInfo��
	fread(&srcBmpInfo, sizeof(BITMAPINFOHEADER),1,srcBmp);
	//��ȡ��ɫ�塣ע�⣡����Ĭ�ϳ���Ϊ1024������ɫ������1024/4��256����ɫ��Ĭ����Ϊ16λͼ�ˣ�����24λͼ��32λͼ��û�е�ɫ�壩��
	//��ʵ�����ʵ�����Ӧ������Ϣͷ�м��ȡ��ɫ����������Ȼ��*4����Ϊ��BRG��Alpha͸�����ĸ�ֵ��ÿ��ռһ���ֽڣ�
	fread(ColorTab, 1024,1,srcBmp);

	char m= srcBmpInfo.biBitCount;
	//��ʼ��ȡλͼ����
	//��ȡ���ؾ���߶ȣ���λΪ���أ�
	unsigned MtxHeight = srcBmpInfo.biHeight;
	//��ȡ���ؾ����ȣ���λΪ���أ�
	unsigned MtxWidth = srcBmpInfo.biWidth;
	//BMPÿ�е����ݳ��ȱ�Ϊ4�ı�����ԭ����Ϊ�˺�windowsĬ�ϵ���С4�ֽڵ�ɨ�跽ʽһ�£�����ɨ���ٶ������棩������λ������0����
	unsigned BufWidth = (MtxWidth+3) /4 *4;

	//unsigned char ռһ���ֽ�
	unsigned char *srcLineBuf = new unsigned char[BufWidth];
	//��̬���ɶ�ά����destBmpMtx
	unsigned char **destBmpMtx = new unsigned char *[MtxHeight];

	unsigned i,j;
	for (i=0; i < MtxHeight; i++)
	{
		//ÿ�ζ�ȡ������ȣ�BufWidth���ĳ���
		fread(srcLineBuf, BufWidth, 1, srcBmp);
		//���ɶ�̬�����һ��
		destBmpMtx[i] = new unsigned char[BufWidth];

		for (j=0; j < MtxWidth; j++)
		{
			//��Դ������ݵ�ÿһ�����ؽ���ɨ�裬�����Ǻ�ɫ�ͽ��з�ɫ
			//0x��ͷ��ʾ16���Ƶ����ݣ�FF��λ16���ƣ��պ�32λ��Ϊһ���ֽڣ�F�ɿ���1111��16����FF����1111 1111��16*16=256
			//if (srcLineBuf[j] == 0xFF) 
			//	destBmpMtx[i][j] = 0xFF;
			//��ɫ��ʽΪ255-��λ�����ص�ֵ
			destBmpMtx[i][j] = 255 - srcLineBuf[j];
		}
	}

	//�����ڻ�ͼ�л���һ��BMP���򿪸��ļ�
	FILE *destBmp = fopen("result1.bmp","wb");

	//�Ѵ��ļ����ļ�ͷ����Ϣͷд���µ��ļ�
	fwrite(&srcBmpHead, sizeof(BITMAPFILEHEADER),1,destBmp);
	fwrite(&srcBmpInfo, sizeof(BITMAPINFOHEADER),1,destBmp);
	//��ɫ��Ҳ�������
	fwrite(ColorTab, 1024,1,destBmp);

	//��ʼ��λͼ����
	for (i=0; i<MtxHeight; i++)
	{
		fwrite(destBmpMtx[i], BufWidth, 1, destBmp);
		delete []destBmpMtx[i];
		destBmpMtx[i] = NULL;
	}

	//�����͹ر��ļ�
	delete []destBmpMtx;
	delete []srcLineBuf;
	srcLineBuf = NULL;
	return true;
}

bool Picture::Smooth(const char * OutputBmpFileName)
{
	
	if(this->readStatus==false)
	{
		cout<<"��δ�����ļ�"<<endl;
		return false;
	}

	this->BmpHeader;
	this->BmpInfo;
	this->ColorTab;
	
	//2.���ݴ���
	int tmp=0;
	//�ȸ�ֵһ����datamtxһģһ���ľ���dataoutmtx
	unsigned char** DataOutMtx= new unsigned char*[BmpHeight];
	for(int i=0;i<BmpHeight;i++)
	{
		DataOutMtx[i]=new unsigned char[MtxWidth];
		for(int j=0;j<MtxWidth;j++)
		{
			DataOutMtx[i][j]=this->DataMtx[i][j];
		}
	}

	//������������ƽ��
	for(int i=1;i<BmpHeight-1;i++)
		for(int j=1;j<BmpWidth-1;j++)
		{
			//������ѭ������ģ��
				for(int x=-1;x<2;x++)
					for(int y=-1;y<2;y++)
					{
						if((i+x>=0&&i+x<BmpHeight)&&(j+y>=0&&j+y<BmpWidth)) 
						tmp+=DataMtx[i+x][j+y];
					}
					DataOutMtx[i][j]=tmp/9;//Out�������ǲ��ı�ԭ���������£����½����µ�Ԫ��
					tmp=0;
		}



		//3.�ѽ���洢��һ��bmp��
		if(this->Write(OutputBmpFileName,DataOutMtx))
			return true;
		else
			return false;
}

bool Picture::Eu5Transform(const char * LocFileName,const char* DisFileName)
{
	if(this->readStatus==false)
	{
		cout<<"�ļ���δ����"<<endl;
		return false;
	}

	//����һ������LocMtx
	unsigned char ** LocMtx=new unsigned char* [this->BmpHeight];
	for(int i=0;i<this->BmpHeight;i++)
	{
		LocMtx[i]=new unsigned char [this->MtxWidth];
		for(int j=0;j<this->MtxWidth;j++)
			LocMtx[i][j]=this->DataMtx[i][j];
	}

	//��ʼ�����볡
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


	// ģ�����任
	// 1.�����µ����Ϸ���
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

				if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//ģ�����ȫ����ͼ����

				//if (fabs(DisMtx[y][x]-MaxFloat)<MinFloat) continue; // ��λ�����������󲻿ɼ���

				//1��MixDis���������y,x��������ɫ�ģ��ұ߼�ΪTmpDis��������Ϊ�����+TmpDis�����������
				//2��MixDis����ֵ�������Ƿ����������������ص㣬MixDis0ʼ��Ϊ������С��ֵ
				if (MinDis > DisMtx[y][x] + TmpDis)
				{
					MinDis =DisMtx[y][x] + TmpDis;//��ΪTmpDis������Сֵ�����Ҿ����෬�Ƚ�֮��һ���õ�������С�ľ���
					DisMtx[i][j] = MinDis;//�������볡
					LocMtx[i][j] = LocMtx[y][x];//�޸�ͼ�񣬣�y,x����Ϊ���������ɫ�ĵ�����꣬��ɫֵ���������
				}
			}
		}

		// ģ�����任
		// 2.�����ϵ����·���
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

					if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//ģ���ʱ����ĵ������ͼ����

					//if (fabs(DisMtx[y][x]-MaxFloat)<MinFloat) continue; // ��λ�����������󲻿ɼ���

					//1��MixDis���������y,x��������ɫ�ģ��ұ߼�ΪTmpDis��������Ϊ�����+TmpDis�����������
					//2��MixDis����ֵ�������Ƿ����������������ص㣬MixDis0ʼ��Ϊ������С��ֵ
					if (MinDis > DisMtx[y][x] + TmpDis)
					{
						MinDis =DisMtx[y][x] + TmpDis;//��ΪTmpDis������Сֵ�����Ҿ����෬�Ƚ�֮��һ���õ�������С�ľ���
						DisMtx[i][j] = MinDis;//�������볡
						LocMtx[i][j] = LocMtx[y][x];//�޸�ͼ�񣬣�y,x����Ϊ���������ɫ�ĵ�����꣬��ɫֵ������䳡
					}
				}
			}
			this->eu5Tmp.LocMtx=LocMtx;
			this->eu5Tmp.DisMtx=DisMtx;

			this->Write(DisFileName,DisMtx);//ע�⣡���볡����float��ʽ����ģ�����Ҫת����32λͼ�������Ϊһ��floatռ4���ֽڣ���32λͼһ�����ض�Ӧ
			this->Write(LocFileName,LocMtx);//���䳡��Ȼ����256ɫͼ�����
}

bool Picture::Drct8Transform(const char * LocFileName,const char* DisFileName)
{
	if(this->readStatus==false)
	{
		cout<<"�ļ���δ����"<<endl;
		return false;
	}

	//����һ������LocMtx
	unsigned char ** LocMtx=new unsigned char* [this->BmpHeight];
	for(int i=0;i<this->BmpHeight;i++)
	{
		LocMtx[i]=new unsigned char [this->MtxWidth];
		for(int j=0;j<this->MtxWidth;j++)
			LocMtx[i][j]=this->DataMtx[i][j];
	}

	//��ʼ�����볡
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


	// ģ�����任
	// 1.�����µ����Ϸ���
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

				if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//ģ�����ȫ����ͼ����

				//if (fabs(DisMtx[y][x]-MaxFloat)<MinFloat) continue; // ��λ�����������󲻿ɼ���

				//1��MixDis���������y,x��������ɫ�ģ��ұ߼�ΪTmpDis��������Ϊ�����+TmpDis�����������
				//2��MixDis����ֵ�������Ƿ����������������ص㣬MixDis0ʼ��Ϊ������С��ֵ
				if (MinDis > DisMtx[y][x] + TmpDis)
				{
					MinDis =DisMtx[y][x] + TmpDis;//��ΪTmpDis������Сֵ�����Ҿ����෬�Ƚ�֮��һ���õ�������С�ľ���
					DisMtx[i][j] = MinDis;//�������볡
					LocMtx[i][j] = LocMtx[y][x];//�޸�ͼ�񣬣�y,x����Ϊ���������ɫ�ĵ�����꣬��ɫֵ���������
				}
			}
		}

		// ģ�����任
		// 2.�����ϵ����·���
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

					if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//ģ�����ȫ����ͼ����

					//if (fabs(DisMtx[y][x]-MaxFloat)<MinFloat) continue; // ��λ�����������󲻿ɼ���

					//1��MinDis���������y,x��������ɫ�ģ��ұ߼�ΪTmpDis��������Ϊ�����+TmpDis�����������
					//2��MinDis����ֵ�������Ƿ����������������ص㣬MinDis0ʼ��Ϊ������С��ֵ
					if (MinDis > DisMtx[y][x] + TmpDis)
					{
						MinDis =DisMtx[y][x] + TmpDis;//��ΪTmpDis������Сֵ�����Ҿ����෬�Ƚ�֮��һ���õ�������С�ľ���
						DisMtx[i][j] = MinDis;//�������볡
						LocMtx[i][j] = LocMtx[y][x];//�޸�ͼ�񣬣�y,x����Ϊ���������ɫ�ĵ�����꣬��ɫֵ������䳡
					}
				}
			}
			this->dr8Tmp.LocMtx=LocMtx;
			this->dr8Tmp.DisMtx=DisMtx;

			this->Write(DisFileName,DisMtx);//ע�⣡���볡����float��ʽ����ģ�����Ҫת����32λͼ�������Ϊһ��floatռ4���ֽڣ���32λͼһ�����ض�Ӧ
			this->Write(LocFileName,LocMtx);//���䳡��Ȼ����256ɫͼ�����
}

bool Picture::VoronoiTransform(const char * OutputBmpFileName,unsigned char** LocMtx)
{
	if(LocMtx==NULL)
		return false;

	//���濪ʼ����Voronoi������ɨ������
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

				if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//ģ�����ȫ����ͼ����

				//����ģ���жϣ��Ǳ߽��ģ���Ϊ100
				if(LocMtx[y][x]!=LocMtx[i][j])
				{
					tmpMtx[i][j]=0;
					//ע�����break��һ�������Ǳ߽�㣬ֹͣ����ģ�壬�����������255�Ŀ���
					break;
				}
				else tmpMtx[i][j]=255;
			}
	}
	this->Write(OutputBmpFileName,tmpMtx);//���䳡��Ȼ����256ɫͼ�����
	return true;
}

bool Picture::BufferPaint(const char * OutputBmpFileName,float** EuDisMtx,float bufferDis)
{
	if(EuDisMtx==NULL)
		return false;

	//���濪ʼ����Voronoi������ɨ������
	unsigned char** tmpMtx=new unsigned char* [this->BmpHeight];
	for(int i=0;i<this->BmpHeight;i++)
	{
		tmpMtx[i]=new unsigned char[this->MtxWidth];
		for(int j=0;j<this->MtxWidth;j++)
		{
			//�жϸ����ص�ľ����Ƿ���ڻ��������룬�����򻭺��ߣ����������óɰ�ɫ
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
	//�����ȷ�ϴ�����ǵ����ݣ������޷���ȷ����Delaunay
	BMPPoint colorPoint[DnyCpct];
	int pointCount=0;
	//ͳ������Ԫ����
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

	//pointPairΪͳ�Ƶ�Ե�����
	int pointPairCount=0;

	//��ʼ��ʮ��ģ��ɨ������ͼ��
	for(int i=0;i<this->BmpHeight;i++)
	{
		for(int j=0;j<this->BmpWidth;j++)
			for (int k=0; k<vrnTmp.TmpSize(); k++)
			{
				int OffX = vrnTmp.GetOffX(k);
				int OffY = vrnTmp.GetOffY(k);

				int x = j + OffX;
				int y = i + OffY;

				if (x<0||y<0||x>this->BmpWidth-1||y>this->BmpHeight-1) continue;//ģ�����ȫ����ͼ����

				if(LocMtx[y][x]!=LocMtx[i][j])//���ֱ߽�
				{
					bool same=true;
					for(int m=0;m<=pointPairCount;m++)
					{
						//��������ɫ�Ƿ��Ѿ����
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
					if(same==false)//��û�б����
					{
						//��¼��Щ�����color1��color2�У�vrnTmp�����ԣ�
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
					break;//����kѭ��
				}
			}
	}
	for(int i=0;i<pointPairCount;i++)
	{
		//���ݼ�¼�ĵ�Ե���Bresenham�㷨
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


	//ԭ����ͬ����׸��
	fread(&BmpHeader,sizeof(BITMAPFILEHEADER),1,InputBmp);
	fread(&BmpInfo,sizeof(BITMAPINFOHEADER),1,InputBmp);
	fread(ColorTab,1024,1,InputBmp);

	BmpHeight= BmpInfo.biHeight;
	MtxWidth=(BmpInfo.biWidth+3)/4*4;
	BmpWidth=BmpInfo.biWidth;

	//��ʼ������
	DataMtx=new unsigned char *[BmpHeight];
	for(int i=0;i<BmpHeight;i++)
	{
		//���Ƕ�̬������Ԫ����
		DataMtx[i]= new unsigned char [MtxWidth];
		fread(DataMtx[i],MtxWidth,1,InputBmp);
	}
	fclose(InputBmp);//���˵�һ���ļ�������
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
	BITMAPFILEHEADER IdxFileHead; // 32λλͼͷ�ṹ
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
		cout<<"���ļ���δ��ȡ"<<endl;
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
