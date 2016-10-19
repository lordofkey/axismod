#include "StdAfx.h"
#include "Mymodfunc.h"
#define pi 3.141592653f
#define SEND(str) pstr = new CString(#str);\
	::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);


Mymodfunc* Mymodfunc::m_pInstance = NULL;//单例对象指针

Mymodfunc::Mymodfunc(void)
	: totallayer(0)
{
	memset(lxbuffer, 0, sizeof(void*)*50000);
	memset(lybuffer, 0, sizeof(void*)*50000);
	memset(rxbuffer, 0, sizeof(void*)*50000);
	memset(rybuffer, 0, sizeof(void*)*50000);
	isfirmod[0] = false;
	isfirmod[1] = false;
	issecmod[0] = false;
	issecmod[1] = false;
	pnum = 0;
	lpdata = NULL;
}
void Mymodfunc::release()
{
	delete this;
}
Mymodfunc::~Mymodfunc(void)
{
	delete lpdata;
	cleanbuffer();
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
bool Mymodfunc::readfile(CString filename,int& layers)
{
	set.fSource = filename;
	if(checkext(filename) == 2)
	{
		FILE* pf;
		long length;
		short *temdata;
		if(0 == fopen_s(&pf,filename,"rb"))
		{
			//2016-7-27修改
			//原因：读取文件格式发生变化
			//修改内容：读取文件方法
			////////////////////////////////////////////////
			short temlen;
			fread(&temlen,sizeof(short),1,pf);
			fseek(pf,0,SEEK_END);
			length = ftell(pf);
			this->totallayer = length/((temlen)*4);
			rewind (pf);
			if(lpdata != NULL)
			{
				delete lpdata;
				lpdata = NULL;
			}
			temdata = new short[length/sizeof(short)];
			fread_s(temdata,length,sizeof(short),length/sizeof(short),pf);
			fclose(pf);
			pnum = temlen;
			pstr = new CString();
			pstr->Format("扫描点数：%d",pnum);
			::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);

			lpdata = new int[totallayer * 2*pnum];

			for(int i = 0;i<totallayer * 2*pnum;i++)
			{
				lpdata[i] = temdata[i];
			}
			delete temdata;

			for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
			{
				float th = (i-pnum)*pi/2.0f/(float)pnum;
				l_sint[i] = sin(th);
				l_cost[i] = cos(th);
			}
			for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
			{
				float th = -(i+pnum)*pi/2.0f/(float)pnum;
				r_sint[i] = sin(th);
				r_cost[i] = cos(th);
			}
			isfirmod[0] = false;
			isfirmod[1] = false;
			issecmod[0] = false;
			issecmod[1] = false;
			this->cleanbuffer();
			pstr = new CString();
			pstr->Format("读取切片数：%d",totallayer);
			::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
			layers = totallayer;
			return true;
		}
		return false;
	}
	if(checkext(filename) == 0)
	{
		return false;
	}
	FILE* pf;
	long length;
	USHORT *temdata;
	bool isold = false;
	if(0 == fopen_s(&pf,filename,"rb"))
	{
		//2016-7-27修改
		//原因：读取文件格式发生变化
		//修改内容：读取文件方法
		////////////////////////////////////////////////
		
		USHORT temlen;
		fseek(pf,8,SEEK_SET);
		fread(&temlen,sizeof(USHORT),1,pf);
		fseek(pf,0,SEEK_END);
		length = ftell(pf);
		this->totallayer = length/(temlen*2+12);
		rewind (pf);
		if(lpdata != NULL)
		{
			delete lpdata;
			lpdata = NULL;
		}
		temdata = new USHORT[length/sizeof(USHORT)];
		fread_s(temdata,length,sizeof(USHORT),length/sizeof(USHORT),pf);
		fclose(pf);
		if(temlen == 366)
		{
			pnum = 180;
			lpdata = new int[totallayer * 360];
			SEND(角分辨率为1/2)
		}
		else if(temlen == 1464)
		{
			pnum = 360;
			lpdata = new int[totallayer * 720];
			SEND(角分辨率为1/4)
		}
		else if(temlen == 2164)
		{
			pnum = 540;
			lpdata = new int[totallayer * 1080];
			SEND(角分辨率为1/6)
		}
		else
		{
			this->totallayer = this->totallayer = length/378/2;
			pnum = 180;
			lpdata = new int[totallayer * 360];
			isold = true;
			SEND(角分辨率为1/2)
		}
		for(int i = 0;i<totallayer;i++)
		{
			if(isold)
			{
				for(int j = 0; j<180; j++)
				{
					lpdata[j+pnum*2*i] = temdata[378*i + 190 + j];
					lpdata[j+pnum*(2*i+1)] = temdata[378*i + 4 + j];
				}
			}
			else
			{
				for(int j = 0; j<180; j++)
				{
					lpdata[j+pnum*2*i] = temdata[(temlen+6)*i + 6 + j];
					lpdata[j+pnum*(2*i+1)] = temdata[(temlen+6)*i + 6 + temlen/2 + j];
				}
			}
		}
		delete temdata;

		for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
		{
			float th = (i-pnum)*pi/2.0f/(float)pnum;
			l_sint[i] = sin(th);
			l_cost[i] = cos(th);
		}
		for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
		{
			float th = -(i+pnum)*pi/2.0f/(float)pnum;
			r_sint[i] = sin(th);
			r_cost[i] = cos(th);
		}
		isfirmod[0] = false;
		isfirmod[1] = false;
		issecmod[0] = false;
		issecmod[1] = false;

		this->cleanbuffer();


		pstr = new CString();
		pstr->Format("读取切片数：%d",totallayer);
		::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
		layers = totallayer;
		return true;
	}
	return false;
}

// 获取第n个断片的数据

bool Mymodfunc::Getpiecen(int n, int** x, int** y,int &numofp)
{
	numofp = pnum;

	if(lpdata != NULL&&n<totallayer)
	{
		*x = new int[pnum*2];
		*y = new int[pnum*2];
		for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
		{
			(*x)[i] = (int)(l_cost[i]*lpdata[pnum*2*n + i]);
			(*y)[i] = (int)(l_sint[i]*lpdata[pnum*2*n + i]);
			(*x)[pnum+i] = (int)(r_cost[i]*lpdata[pnum*(2*n+1) + i]);
			(*y)[pnum+i] = (int)(r_sint[i]*lpdata[pnum*(2*n+1) + i]);
		}
		return true;
	}
	return false;
}



///////////////////////////////////////////////////////////////////////////
// 获取第n个断片的一次校正后的数据



#pragma region 数据输出

bool Mymodfunc::Getpiecenmodl(int n, int** x, int** y,int &numofp)
{
	
	if((lpdata != NULL)&&isfirmod[0])
	{
		if(lxbuffer[n] == NULL||lybuffer[n] == NULL)
		{
			lxbuffer[n] = new int[pnum];
			lybuffer[n] = new int[pnum];
			for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
			{
				lxbuffer[n][i] = (int)(l_cosm[i]*lpdata[pnum*2*n + i] - set.pLeft.delta_x);
				lybuffer[n][i] = (int)(l_sinm[i]*lpdata[pnum*2*n + i] + set.pLeft.delta_y);
			}
		}
		*x = lxbuffer[n];
		*y = lybuffer[n];
		numofp = pnum;
		return true;
	}
	return false;
}

bool Mymodfunc::Getpiecenmodr(int n, int** x, int** y,int &numofp)
{
	static int yy = 0;

	if((lpdata != NULL)&&isfirmod[1])
	{
		if(rxbuffer[n] == NULL||rybuffer[n] == NULL)
		{
			rxbuffer[n] = new int[pnum];
			rybuffer[n] = new int[pnum];
			for(int i = 0; i<pnum; i++) //初始化极坐标转换序列
			{
				rxbuffer[n][i] = (int)(r_cosm[i]*lpdata[pnum*(2*n+1) + i] + set.pRight.delta_x);
				rybuffer[n][i] = (int)(r_sinm[i]*lpdata[pnum*(2*n+1)  + i] + set.pRight.delta_y);
			}
		}
		*x = rxbuffer[n];
		*y = rybuffer[n];


		numofp = pnum;
		return true;
	}
	return false;
}
//bool Mymodfunc::Getpiecenmodl2(int n, int** x, int** y)
//{
//	*x = new int[180];
//	*y = new int[180];
//	if((lpdata != NULL)&&isfirmod[0]&&issecmod[0])
//	{
//		float tanw1 = tan(w1[0]); //水平偏转角
//		float tanw2 = tan(w2[0]); //垂直偏转角
//
//		float cosw1 = cos(w1[0]);
//		float cosw2 = cos(w2[0]);
//		for(int i = 0; i<180; i++) //初始化极坐标转换序列
//		{
//			float temx = cosm[i]*lpdata[360*n + i] - x_delta[0];
//			float temy = sinm[i]*lpdata[360*n + i] + y_delta[0];
//			float temd = (temx*tanw1+temy*tanw2)/sqrt(1+tanw1*tanw1+tanw2*tanw2); //点到法向量投影
//			(*x)[i] = (int)((temx - temd*tanw1)/cosw1);//二次校正之后的坐标值
//			(*y)[i] = (int)((temy - temd*tanw2)/cosw2);
//		}
//		return true;
//	}
//	delete *x;
//	delete *y;
//	return false;
//}
//
//bool Mymodfunc::Getpiecenmodr2(int n, int** x, int** y)
//{
//	*x = new int[180];
//	*y = new int[180];
//	if((lpdata != NULL)&&isfirmod[1]&&issecmod[1])
//	{
//		float tanw1 = tan(w1[1]); //水平偏转角
//		float tanw2 = tan(w2[1]); //垂直偏转角
//
//		float cosw1 = cos(w1[1]);
//		float cosw2 = cos(w2[1]);
//		for(int i = 0; i<180; i++) //初始化极坐标转换序列
//		{
//			float temx = cosm[i+180]*lpdata[360*n + 180 + i] + x_delta[1];
//			float temy = sinm[i+180]*lpdata[360*n + 180 + i] + y_delta[1];
//			float temd = (temx*tanw1+temy*tanw2)/sqrt(1+tanw1*tanw1+tanw2*tanw2); //点到法向量投影
//			(*x)[i] = (int)((temx - temd*tanw1)/cosw1);//二次校正之后的坐标值
//			(*y)[i] = (int)((temy - temd*tanw2)/cosw2);
//		}
//		return true;
//	}
//	delete *x;
//	delete *y;
//	return false;
//}
#pragma endregion 数据输出
//第一次校正计算
//输入：雷达实际高度height  铁路宽width  雷达到铁路的水平距离distan 左右轨道标定范围lroix lroiy rroix rroiy
//输出：雷达偏转角度ang_delta  轨道与雷达的相对位置x_delta  y_delta
bool Mymodfunc::firstmod(ModPar par)
{
	if(!checkModPar(par))
	{
		SEND(参数不合法，或者没有输入雷达数据，请检查)
		return false;
	}
	float ang_real = atan((par.distan+par.width/2)/par.height);			//实际轨道中心与雷达连线的方向角度
	MyPoint pleft,pright;
	pleft = findrail(l_range,par); //确定轨道位置
	pright = findrail(r_range,par);
	if(pleft.y<-190000)
		return false;
	if(pright.y<-190000)
		return false;




	float mx = (float)(pleft.x+pright.x)/2;
	float my = (float)(pleft.y+pright.y)/2;


/////////////////////////////////////////////
	TrainPar *tpar;
	if(par.islr == 0)
		tpar = &(set.pLeft);
	else
		tpar = &(set.pRight);
	tpar->radarheight = par.height;
	tpar->road_radar_dis = par.distan;
	tpar->roadwidth = par.width;
///////////////////////////////////////////////////


//it`s a litter disturbing, but definitely right. you may change this when you want to change it all!!!!


	tpar->delta_th = -((pright - pleft).getangle());
	tpar->delta_x = abs(cos(tpar->delta_th)*mx - sin(tpar->delta_th)*my);
	tpar->delta_y = abs(cos(tpar->delta_th)*my + sin(tpar->delta_th)*mx);
	if(par.islr == 0)
		tpar->delta_th = -(tpar->delta_th);

#pragma region push message
	pstr = new CString();
	pstr->Format("水平偏差：%.4f，垂直偏差%.4f，角度偏差%.4f",tpar->delta_x, tpar->delta_y, (tpar->delta_th)*57.3);
	::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
#pragma endregion push message								 


	isfirmod[par.islr] = true;

	updatahash();
	
//	traincut();
	return true;
}
// 二次校正
//输入：标准车型 宽 高 width,height  断层位置
//输出：雷达水平偏移角度w1,雷达垂直偏移角度w2
bool Mymodfunc::seconmod(int islr, float width, float height,int carst,int caren)
{
	if(!isfirmod[islr])
	{
		SEND(请先进行一次标定)
			return false;
	}
	int minx = 0,maxx = 0,maxy = 0;
	int x,y;
	int minxtmp;
	int hid = 0;
	int wid = 0;
	if(islr == 0)
	{
		for(int n = carst;n < caren;n++)
		{
			for(int i = 0;i<pnum;i++)
			{
				x =  (int)(l_cosm[i]*lpdata[2*pnum*n + i] - set.pLeft.delta_x);
				y =  (int)(l_sinm[i]*lpdata[2*pnum*n + i] + set.pLeft.delta_y);
				if((x > -2100)&&(x < 2100)&&(y > 1250)&&(y < 5550))
				{
					if(y > maxy)
					{
						maxy = y;
						hid = n;
					}
					if(x < minx)
					{
						minx = x;
						wid = n;
					}
					if(x > maxx)
						maxx = x;
				}
			}
		}
		int widthl = - 2*minx;
		if(width<widthl)
			set.pLeft.w1 = acos(width/widthl);
		else
			set.pLeft.w1 = 0;
		if(height < maxy)
			set.pLeft.w2 = acos(height/maxy);
		else
			set.pLeft.w2 = 0;
		minxtmp = -minx;
	}
	else
	{
		for(int n = carst;n < caren;n++)
		{
			for(int i = 0;i<pnum;i++)
			{
				x =  (int)(r_cosm[i]*lpdata[pnum*(2*n+1) + i] + set.pRight.delta_x);
				y =  (int)(r_sinm[i]*lpdata[pnum*(2*n+1) + i] + set.pRight.delta_y);
				if((x > -2100)&&(x < 2100)&&(y > 1250)&&(y < 5550))
				{
					if(y > maxy)
					{
						maxy = y;
						hid = n;
					}
					if(x < minx)
						minx = x;
					if(x > maxx)
					{
						maxx = x;
						wid = n;
					}
				}
			}
			
		}
		int widthl = - 2*minx;
		if(width<widthl)
			set.pRight.w1 = acos(width/widthl);
		else
			set.pRight.w1 = 0;
		if(height < maxy)
			set.pRight.w2 = acos(height/maxy);
		else
			set.pRight.w2 = 0;

		minxtmp = -maxx;
	}
	int widtmp = width/2;
	
	if(height != 0)
	{
		pstr = new CString();
		pstr->Format("标准宽：%d, 测量宽：%d,标准高：%d, 测量高：%d, hid：%d， wid：%d", widtmp, minxtmp, (int)height, maxy, hid, wid);
		::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
	}
	issecmod[islr] = true;

	//if(islr == 0)
	//{
	//	pstr = new CString();
	//	pstr->Format("左侧雷达：水平偏移角： %.2f \r\n\t\t\t    垂直偏移角：%.2f",set.pLeft.w1*180/pi,set.pLeft.w2*180/pi);
	//	::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
	//}
	//if(islr == 1)
	//{
	//	pstr = new CString();
	//	pstr->Format("右侧雷达：水平偏移角： %.2f \r\n\t\t\t    垂直偏移角：%.2f",set.pRight.w1*180/pi,set.pRight.w2*180/pi);
	//	::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
	//}
	return true;
}
//自动切车
bool Mymodfunc::traincut()
{
	int *x = new int[1000];
	int	*y = new int[1000];
	int *tmpx;
	int *tmpy;
	if(isfirmod[0]&&isfirmod[1])
	{
		int cc = 0;
		int postn = 0;
		int num;
		for(int n = 0;n < totallayer;n++)
		{
			Getpiecenmodl(n,&tmpx,&tmpy,num);
			memcpy(x,tmpx,num*sizeof(int));
			memcpy(y,tmpy,num*sizeof(int));
			Getpiecenmodr(n,&tmpx,&tmpy,num);
			memcpy(x+num,tmpx,num*sizeof(int));
			memcpy(y+num,tmpy,num*sizeof(int));
			if(kmean(x,y,2*num, n - postn))
			{
				if((n - postn)>50)
				{
					set.cpar.stpoint[cc] = postn;
					set.cpar.length[cc] = n - postn;
					cc++;
					postn = n;
				}
				
			}
		}
		set.cpar.num = cc;
		pstr = new CString();
		pstr->Format("切车节数：%.2d",cc);
		::PostMessage(this->h_form,WM_MESSOUT,(WPARAM)pstr,NULL);
		return true;
	}
	return false;
}

void Mymodfunc::updatahash(void)
{
	for(int i = 0;i<pnum;i++)       //初始化极坐标转换序列
	{
		float th = (i-pnum)*pi/2.0f/pnum - set.pLeft.delta_th;
		l_sinm[i] = sin(th);
		l_cosm[i] = cos(th);
	}
	for(int i = 0;i<pnum;i++)       //初始化极坐标转换序列
	{
		float th = (-i-pnum)*pi/2.0f/pnum + set.pRight.delta_th;
		r_sinm[i] = sin(th);
		r_cosm[i] = cos(th);
	}
	this->cleanbuffer();
}
// 检查一次校正参数
bool Mymodfunc::checkModPar(ModPar par)
{
	if((par.height <= 0)||(par.width <= 0)||(par.distan <= 0)||(lpdata == NULL))
		return false;
	else if(par.islr != 0&&par.islr != 1)
		return false;
	else if(par.caren <= par.carst)
		return false;
	else if(par.caren <= 0 || par.carst < 0)
		return false;
	else if(par.caren > totallayer || par.carst >= totallayer)
		return false;
	else
		return true;
}

MyPoint Mymodfunc::findrail(MyRect range, ModPar par)
{
//求均值
	double *meand = new double[pnum];
	memset(meand,0,pnum*sizeof(double));
	int initd = 0;
	int mnum = 0;
	for(int i = 0;i<pnum;i++)
	{
		initd = lpdata[pnum*(2*par.carst+par.islr)+i];
		mnum = 0;
		for(int j = par.carst;j<par.caren;j++)
		{
			if(abs(lpdata[pnum*(2*j+par.islr)+i] - initd)<100)
			{
				meand[i] += lpdata[pnum*(2*j+par.islr)+i];
				mnum++;
			}
		}
		meand[i] /= mnum;
	}
	float *coss;
	float *sins;
//寻找最高点
	if(par.islr == 0)
	{
		coss = l_cost;
		sins = l_sint;
	}
	else
	{
		coss = r_cost;
		sins = r_sint;
	}
	float limy = -200000;
	float limx = 0;
	float temx;
	float temy;
	for(int i = 0;i<pnum;i++)
	{
		temx = coss[i]*(float)meand[i];
		temy = sins[i]*(float)meand[i];
		if(range.isinside(temx,temy))
		{
			if(temy>limy)
			{
				limy = temy;
				limx = temx;
			}
		}
	}
	delete meand;
	MyPoint pt;
	pt.x = limx;
	pt.y = limy;
	return pt;
}

bool Mymodfunc::setleftrange(float x_s, float y_s, float x_e, float y_e)
{
	l_range.st.x = x_s<x_e?x_s:x_e;
	l_range.st.y = y_s<y_e?y_s:y_e;
	l_range.en.x = x_s>x_e?x_s:x_e;
	l_range.en.y = y_s>y_e?y_s:y_e;
	return true;
}

bool Mymodfunc::setrightrange(float x_s, float y_s, float x_e, float y_e)
{
	r_range.st.x = x_s<x_e?x_s:x_e;
	r_range.st.y = y_s<y_e?y_s:y_e;
	r_range.en.x = x_s>x_e?x_s:x_e;
	r_range.en.y = y_s>y_e?y_s:y_e;
	return true;
}

// 检查输入文件扩展名
int Mymodfunc::checkext(CString FileName)
{
	int Where;  
	Where=FileName.ReverseFind('//');  
	if(Where==-1)  
	{  
		Where=FileName.ReverseFind('/');  
	}  
	CString FileTitle=FileName.Right(FileName.GetLength()-1-Where);  
	int Which=FileTitle.ReverseFind('.');  
	CString csCStExtendName=FileTitle.Right(FileTitle.GetLength()-Which-1);  
	if (Which!=-1)  
	{  
		FileTitle=FileTitle.Left(Which);  
	}
	if(csCStExtendName == "lms")
		return 1;
	else if(csCStExtendName == "dat")
		return 2;
	else
		return 0;
}

bool Mymodfunc::checklim(void)
{
	if(!isfirmod[0])
	{
		SEND(请先进行一次标定)
			return false;
	}
	if(!isfirmod[1])
	{
		SEND(请先进行一次标定)
			return false;
	}
	int x,y;
	bool isout = false;
	int t = 0;
	for(int n = 0;n < this->totallayer;n++)
	{

		isout = false;
		if(t>0)
			t--;
		for(int i = 0;i<pnum;i++)
		{
			x =  (int)(l_cosm[i]*lpdata[2*pnum*n + i] - set.pLeft.delta_x);
			y =  (int)(l_sinm[i]*lpdata[2*pnum*n + i] + set.pLeft.delta_y);
			if((x > -2100)&&(x < 2100)&&(y > 1250)&&(y < 5550))
			{
				if(framdet(x, y)!=0.0)
					isout = true;
			}
		}
		for(int i = 0;i<pnum;i++)
		{
			x =  (int)(r_cosm[i]*lpdata[pnum*(2*n+1) + i] + set.pRight.delta_x);
			y =  (int)(r_sinm[i]*lpdata[pnum*(2*n+1) + i] + set.pRight.delta_y);
			if((x > -2100)&&(x < 2100)&&(y > 1250)&&(y < 5550))
			{
				if(framdet(x, y)!=0.0)
					isout = true;
			}
		}
		if(isout&t == 0)
		{
			::PostMessageA(this->h_viewm, WM_REFRESH, (WPARAM)n, 1);
			::PostMessageA(this->h_viewm3d, WM_REFRESH, (WPARAM)n, 1);
			t = 50;
		}
	}
	return true;
}

void Mymodfunc::cleanbuffer(void)
{
	for(int i = 0; i < 50000; i++)
	{
		if(lxbuffer[i] != NULL)
		{
			delete lxbuffer[i];
			lxbuffer[i] = NULL;
		}
		if(lybuffer[i] != NULL)
		{
			delete lybuffer[i];
			lybuffer[i] = NULL;
		}
		if(rxbuffer[i] != NULL)
		{
			delete rxbuffer[i];
			rxbuffer[i] = NULL;
		}
		if(rybuffer[i] != NULL)
		{
			delete rybuffer[i];
			rybuffer[i] = NULL;
		}
	}
}
