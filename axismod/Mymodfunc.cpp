#include "StdAfx.h"
#include "Mymodfunc.h"
#include <math.h>
#define pi 3.141592653f
#define SEND(str) pstr = new CString(#str);\
	::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);


Mymodfunc* Mymodfunc::m_pInstance = NULL;//单例对象指针

Mymodfunc::Mymodfunc(void)
	: totallayer(0)
	, cc(0)
	, version(_T(""))
{
	w1[0] = 0.0f;
	w1[1] = 0.0f;
	w2[0] = 0.0f;
	w2[1] = 0.0f;
	isfirmod[0] = false;
	isfirmod[1] = false;
	issecmod[0] = false;
	issecmod[1] = false;
	pnum = 180;
	lpdata = NULL;
	memset(stps,0,sizeof(stps));
	memset(lens,0,sizeof(lens));
	static Deleter deleter;
	version = "1.0.0.160801";
	source = "";
}
Mymodfunc::~Mymodfunc(void)
{
	delete lpdata;
	lpdata = NULL;
}
Mymodfunc *Mymodfunc::GetInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new Mymodfunc();
	}
	return m_pInstance;
}

// 读取点云文件
//输入：文件名
//输出：点云数据 长度 是否成功读取
bool Mymodfunc::readfile(CString filename,CString path)
{
	this->source = filename;
	FILE* pf;
	long length;
	inifilename = path+"\\par.ini";
	int *temdata;
	if(0 == fopen_s(&pf,filename,"rb"))
	{
		//2016-7-27修改
		//原因：读取文件格式发生变化
		//修改内容：读取文件方法
		////////////////////////////////////////////////
		int temint[4];
		fread(temint,sizeof(int),4,pf);
		fseek(pf,0,SEEK_END);
		length = ftell(pf);
		this->totallayer = length/(temint[2]*2+16);
		rewind (pf);
		if(lpdata != NULL)
		{
			delete lpdata;
			lpdata = NULL;
		}
		temdata = new int[length/sizeof(int)];
		fread_s(temdata,length,sizeof(int),length/sizeof(int),pf);
		if(temint[2] == 732)
		{
			pnum = 180;
			lpdata = new int[totallayer * 360];
		}
		else if(temint[2] == 1464)
		{
			pnum = 360;
			lpdata = new int[totallayer * 720];
		}
		else if(temint[2] == 2164)
		{
			pnum = 540;
			lpdata = new int[totallayer * 1080];
		}
		else
		{
			return false;
		}
		for(int i = 0;i<totallayer;i++)
		{
			memcpy(lpdata+pnum*2*i,temdata+(temint[2]/2+4)*i+4,pnum*sizeof(int));
			memcpy(lpdata+(2*i+1)*pnum,temdata+(temint[2]/2+4)*i+4+temint[2]/4,pnum*sizeof(int));
		}
		delete temdata;
		for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
		{
			float th = (i-pnum)*pi/2.0f/(float)pnum;
			sint[i] = sin(th);
			cost[i] = cos(th);
		}
		for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
		{
			float th = -(i+pnum)*pi/2.0f/(float)pnum;
			sint[i+pnum] = sin(th);
			cost[i+pnum] = cos(th);
		}

		isfirmod[0] = true;
		isfirmod[1] = true;
		issecmod[0] = false;
		issecmod[1] = false;

		pstr = new CString();
		pstr->Format("读取切片数：%d",totallayer);
		::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);

		return true;
	}
	return false;
}

// 获取第n个断片的数据
bool Mymodfunc::Getpiecen(int n, int** x, int** y,int &numofp)
{
	numofp = pnum;
	if(lpdata != NULL)
	{
		*x = new int[pnum*2];
		*y = new int[pnum*2];
		for(int i = 0; i<pnum*2; i++) //初始化极坐标转换序列
		{
			(*x)[i] = (int)(cost[i]*lpdata[pnum*2*n + i]);
			(*y)[i] = (int)(sint[i]*lpdata[pnum*2*n + i]);
		}
		return true;
	}
	return false;
}



///////////////////////////////////////////////////////////////////////////
// 获取第n个断片的一次校正后的数据
#pragma region 数据输出

bool Mymodfunc::Getpiecenmod(int n, int** x, int** y,int &numofp)
{

	if((lpdata != NULL)&&isfirmod[0])
	{
		*x = new int[pnum];
		*y = new int[pnum];
		for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
		{
			(*x)[i] = (int)(cosm[i]*lpdata[pnum*2*n + i] - x_delta[0]);
			(*y)[i] = (int)(sinm[i]*lpdata[2*pnum*n + i] + y_delta[0]);
		}
		numofp = pnum;
		return true;
	}
	return false;
}

bool Mymodfunc::Getpiecenmodr(int n, int** x, int** y,int &numofp)
{

	if((lpdata != NULL)&&isfirmod[1])
	{
		*x = new int[pnum];
		*y = new int[pnum];
		for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
		{
			(*x)[i] = (int)(cosm[i+pnum]*lpdata[2*pnum*n + pnum + i] + x_delta[1]);
			(*y)[i] = (int)(sinm[i+pnum]*lpdata[2*pnum*n + pnum + i] + y_delta[1]);
		}
		numofp = pnum;
		return true;
	}
	return false;
}
bool Mymodfunc::Getpiecenmod2(int n, int** x, int** y)
{
	*x = new int[180];
	*y = new int[180];
	if((lpdata != NULL)&&isfirmod[0]&&issecmod[0])
	{
		float tanw1 = tan(w1[0]); //水平偏转角
		float tanw2 = tan(w2[0]); //垂直偏转角

		float cosw1 = cos(w1[0]);
		float cosw2 = cos(w2[0]);
		for(int i = 0; i<180; i++) //初始化极坐标转换序列
		{
			float temx = cosm[i]*lpdata[360*n + i] - x_delta[0];
			float temy = sinm[i]*lpdata[360*n + i] + y_delta[0];
			float temd = (temx*tanw1+temy*tanw2)/sqrt(1+tanw1*tanw1+tanw2*tanw2); //点到法向量投影
			(*x)[i] = (int)((temx - temd*tanw1)/cosw1);//二次校正之后的坐标值
			(*y)[i] = (int)((temy - temd*tanw2)/cosw2);
		}
		return true;
	}
	delete *x;
	delete *y;
	return false;
}
bool Mymodfunc::Getpiecenmodr2(int n, int** x, int** y)
{
	*x = new int[180];
	*y = new int[180];
	if((lpdata != NULL)&&isfirmod[1]&&issecmod[1])
	{
		float tanw1 = tan(w1[1]); //水平偏转角
		float tanw2 = tan(w2[1]); //垂直偏转角

		float cosw1 = cos(w1[1]);
		float cosw2 = cos(w2[1]);
		for(int i = 0; i<180; i++) //初始化极坐标转换序列
		{
			float temx = cosm[i+180]*lpdata[360*n + 180 + i] + x_delta[1];
			float temy = sinm[i+180]*lpdata[360*n + 180 + i] + y_delta[1];
			float temd = (temx*tanw1+temy*tanw2)/sqrt(1+tanw1*tanw1+tanw2*tanw2); //点到法向量投影
			(*x)[i] = (int)((temx - temd*tanw1)/cosw1);//二次校正之后的坐标值
			(*y)[i] = (int)((temy - temd*tanw2)/cosw2);
		}
		return true;
	}
	delete *x;
	delete *y;
	return false;
}


#pragma endregion 数据输出


// 第一次校正计算
//输入：雷达实际高度height  铁路宽width  雷达到铁路的水平距离distan 左右轨道标定范围lroix lroiy rroix rroiy
//输出：雷达偏转角度ang_delta  轨道与雷达的相对位置x_delta  y_delta
bool Mymodfunc::firstmod(int islr, float height, float width, float distan,int carst ,int caren)
{
	SEND(正在进行一次标定)
		if((height == 0)||(width == 0)||(distan == 0)||(lpdata == NULL))
		{
			SEND(参数不合法，或者没有输入雷达数据，请检查)
				return false;
		}
		float ang_real = atan((distan+width/2)/height);//实际轨道中心与雷达连线的方向角度
		int lmx = 0,lmy = -200000,rmx = 0,rmy = -200000;//两轨道所在点
		for(int n = carst;n < caren;n++)
		{
			for(int i = pnum*islr;i<pnum*(1+islr);i++)
			{
				int x = (int)(cost[i]*lpdata[2*pnum*n + i]);
				int y = (int)(sint[i]*lpdata[2*pnum*n + i]);

				if((x > lroix_min)&&(x < lroix_max)&&(y > lroiy_min)&&(y < lroiy_max))
				{
					if(y > lmy)
					{
						lmy = y;
						lmx = x;
					}
				}
				else if((x > rroix_min)&&(x < rroix_max)&&(y > rroiy_min)&&(y < rroiy_max))
				{
					if(y > rmy)
					{
						rmy = y;
						rmx = x;
					}
				}
			}
		}

		if((lmy<-199999)||(rmy<-199999))
			return false;
		float mx = (float)(lmx+rmx)/2;
		float my = (float)(lmy+rmy)/2;

		pstr = new CString();
		pstr->Format("检测到轨道中心点： (%.1f,%.1f)",mx,my);
		::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);


		ang_delta[islr] = atan(abs(mx/my)) - ang_real;//偏差角度
		x_delta[islr] = sqrt(mx*mx+my*my) * sin(ang_real);
		y_delta[islr] = sqrt(mx*mx+my*my) * cos(ang_real);

		if(islr == 0)
		{
			for(int i = 0;i<pnum;i++) //初始化极坐标转换序列
			{
				float th = ((i-pnum)*pi/2.0f/(float)pnum) - ang_delta[islr];
				sinm[i] = sin(th);
				cosm[i] = cos(th);
			}
		}
		else if(islr == 1)
		{
			for(int i = pnum;i<pnum*2;i++) //初始化极坐标转换序列
			{
				float th = -i*pi/2.0f/(float)pnum + ang_delta[islr];
				sinm[i] = sin(th);
				cosm[i] = cos(th);
			}
		}
		isfirmod[islr] = true;
		char buf[100];


		if(islr == 0)
		{
			sprintf_s(buf,100,"%f",90 - ang_delta[0]*180/pi);
			::WritePrivateProfileString(_T("left radar"),_T("st angle"),buf,inifilename);
			sprintf_s(buf,100,"%f",x_delta[0]+4400);
			::WritePrivateProfileString(_T("left radar"),_T("st x"),buf,inifilename);
			sprintf_s(buf,100,"%f",6000 - y_delta[0]);
			::WritePrivateProfileString(_T("left radar"),_T("st y"),buf,inifilename);
			sprintf_s(buf,100,"%f",((double)y_delta[0] - height)/height);
			::WritePrivateProfileString(_T("left radar"),_T("偏差"),buf,inifilename);

			pstr = new CString();
			pstr->Format("左侧雷达：角度偏差： %.2f \r\n\t\t\t    水平偏差：%.1f\r\n\t\t\t    垂直偏差：%.1f",ang_delta[0]*180/pi,x_delta[0],y_delta[0]);
			::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
		}
		else if(islr == 1)
		{
			sprintf_s(buf,100,"%f",90 + ang_delta[1]*180/pi);
			::WritePrivateProfileString(_T("right radar"),_T("st angle"),buf,inifilename);
			sprintf_s(buf,100,"%f",x_delta[1]+4400);
			::WritePrivateProfileString(_T("right radar"),_T("st x"),buf,inifilename);
			sprintf_s(buf,100,"%f",6000 - y_delta[1]);
			::WritePrivateProfileString(_T("right radar"),_T("st y"),buf,inifilename);
			sprintf_s(buf,100,"%f",((double)y_delta[1] - height)/height);
			::WritePrivateProfileString(_T("right radar"),_T("偏差"),buf,inifilename);

			pstr = new CString();
			pstr->Format("右侧雷达：角度偏差： %.2f \r\n\t\t\t    水平偏差：%.1f\r\n\t\t\t    垂直偏差：%.1f",ang_delta[1]*180/pi,x_delta[1],y_delta[1]);
			::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
		}
		traincut();
		return true;
}
// 二次校正
//输入：标准车型 宽 高 width,height  断层位置
//输出：雷达水平偏移角度w1,雷达垂直偏移角度w2
bool Mymodfunc::seconmod(int islr, float width, float height,int carst,int caren)
{
	if(isfirmod[islr])
	{
		int minx = 0,maxx = 0,maxy = 0;
		for(int n = carst;n < caren;n++)
		{
			for(int i = 180*islr;i<180*(1+islr);i++)
			{
				int rx = (int)(cost[i]*lpdata[360*n + i]);
				int ry = (int)(sint[i]*lpdata[360*n + i]);
				int x = (int)(cosm[i]*lpdata[360*n + i] - x_delta[0]);
				int y = (int)(sinm[i]*lpdata[360*n + i] + y_delta[0]);
				if((rx > -2440)&&(rx < 2440)&&(ry > 100)&&(ry < 5550))
				{
					if(y > maxy)
						maxy = y;
					if(x < minx)
						minx = x;
					if(x > maxx)
						maxx = x;
				}
			}
		}
		int widthl = - 2*minx;
		if(width<widthl)
			w1[islr] = acos(width/widthl);
		else
			w1[islr] = 0;
		if(height < maxy)
			w2[islr] = acos(height/maxy);
		else
			w2[islr] = 0;
		issecmod[islr] = true;
		char buf[100];
		CString tt[2] = {"left radar","left radar"};
		sprintf_s(buf,100,"%f",w1[islr]);
		::WritePrivateProfileString(tt[islr],_T("水平偏移角"),buf,inifilename);
		sprintf_s(buf,100,"%f",w2[islr]);
		::WritePrivateProfileString(tt[islr],_T("垂直偏移角"),buf,inifilename);

		if(islr == 0)
		{
			pstr = new CString();
			pstr->Format("左侧雷达：水平偏移角： %.2f \r\n\t\t\t    垂直偏移角：%.2f",w1[islr]*180/pi,w2[islr]*180/pi);
			::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
		}
		if(islr == 1)
		{
			pstr = new CString();
			pstr->Format("右侧雷达：水平偏移角： %.2f \r\n\t\t\t    垂直偏移角：%.2f",w1[islr]*180/pi,w2[islr]*180/pi);
			::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
		}
		return true;
	}
	SEND(请检查参数)
		return false;
}


//自动切车
bool Mymodfunc::traincut()
{
	int np = 0;
	double sumd = 0;
	int postn = 0;
	cc = 0;//切车标号

	if(isfirmod[0])
	{
		for(int n = 0;n < totallayer;n++)
		{
			np = 0;
			sumd = 0;
			for(int i = 0;i<pnum;i++)
			{
				int x = (int)(cosm[i]*lpdata[pnum*2*n + i] - x_delta[0]);
				int y = (int)(sinm[i]*lpdata[pnum*2*n + i] + y_delta[0]);
				if((x>-2440)&&(x<2440)&&(y>500)&&(y<5550))
				{
					np++;
					sumd = sumd + sqrt((float)(y-800)*(y-800) + x*x);
				}
			}
			double ed = sumd/np;
			if((np > 5)&&(ed<350))
			{
				if((n - postn)>100)
				{
					stps[cc] = postn;
					lens[cc] = n - postn;
					cc++;
				}
				postn = n;
			}
		}
		pstr = new CString();
		pstr->Format("切车节数：%.2d",cc);
		::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);

		return true;
	}
	return false;
}



void Mymodfunc::updatahash(void)
{
	for(int i = 0;i<pnum;i++) //初始化极坐标转换序列
	{
		float th = ((i-pnum)*pi/2.0f/(float)pnum) - ang_delta[0];
		sinm[i] = sin(th);
		cosm[i] = cos(th);
	}
	for(int i = pnum;i<pnum*2;i++) //初始化极坐标转换序列
	{
		float th = -i*pi/2.0f/(float)pnum + ang_delta[1];
		sinm[i] = sin(th);
		cosm[i] = cos(th);
	}
}
