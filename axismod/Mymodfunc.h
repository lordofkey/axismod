#pragma once
#include "MySetting.h"
#include <math.h>
struct ModPar
{
	ModPar()
	{
		islr = 0;
		height = 0;
		width = 0;
		distan = 0;
		carst = 0;
		caren = 0;
	}
	ModPar(	int islr,
	float height,
	float width,
	float distan,
	int carst,
	int caren)
	{
		this->islr = islr;
		this->height = height;
		this->width = width;
		this->distan = distan;
		this->carst = carst;
		this->caren = caren;
	}
	int islr;
	float height;
	float width;
	float distan;
	int carst;
	int caren;
};
struct MyPoint
{
	MyPoint()
	{
		x = 0.0f;
		y = 0.0f;
	}
	MyPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	MyPoint operator +(const MyPoint &a)
	{
		MyPoint re;
		re.x = a.x + x;
		re.y = a.y + y;
		return re;
	}
	MyPoint operator -(const MyPoint &a)
	{
		MyPoint re;
		re.x = x - a.x;
		re.y = y - a.y;
		return re;
	}
	float getangle()
	{
		float th = atan(y/x);
		return th;
	}
	float getdistan()
	{
		float dis = sqrt(x*x+y*y);
		return dis;
	}
	float x;
	float y;
};
struct MyRect
{
	bool isinside(float x,float y)
	{
		if(x<st.x)
			return false;
		else if(x>en.x)
			return false;
		else if(y<st.y)
			return false;
		else if(y>en.y)
			return false;
		else
			return true;
	}
	MyPoint st;
	MyPoint en;
};

class Mymodfunc
{

#pragma region single cls //实现单例对象
private:
	static Mymodfunc* m_pInstance;
	Mymodfunc(void);
	virtual ~Mymodfunc();   
public:
	static Mymodfunc *Mymodfunc::GetInstance();
	void release();
#pragma endregion


private:
	int *lpdata;//雷达数据
	int pnum;//每层对应的点数
	MyRect l_range;
	MyRect r_range;
	//校正前
	float l_sint[540]; //角度对应sin值
	float l_cost[540]; //角度对应cos值
	float r_sint[540]; //角度对应sin值
	float r_cost[540]; //角度对应cos值

	//校正后
	float l_sinm[540]; //角度对应sin值
	float l_cosm[540]; //角度对应cos值
	float r_sinm[540]; //角度对应sin值
	float r_cosm[540]; //角度对应cos值
public:
	// 读取点云文件
	bool readfile(CString filename,int& layers);
	// 获取第n个断片的数据
	bool Getpiecen(int n, int** x, int** y,int &numofp);
	bool Getpiecenmodl(int n, int** x, int** y,int &numofp);
	bool Getpiecenmodr(int n, int** x, int** y,int &numofp);
	bool checkModPar(ModPar  par);
	MyPoint findrail(MyRect range, ModPar par);
	bool setleftrange(float x_s, float y_s, float x_e, float y_e);
	bool setrightrange(float x_s, float y_s, float x_e, float y_e);
	void updatahash(void);
	//bool Getpiecenmodl2(int n, int** x, int** y);
	//bool Getpiecenmodr2(int n, int** x, int** y);
	bool firstmod(ModPar par);//// 第一次校正计算
	bool seconmod(int islr, float width, float height,int carst ,int caren);// 二次校正
	bool traincut();//自动切车


	HWND h_view;//视图窗口句柄
	HWND h_viewm; //校正后的视图句柄
	HWND h_form;//控制面板句柄


	MySetting set;//标定参数
	int totallayer;// 总层数


	bool isfirmod[2];//一次校正是否完成
	bool issecmod[2];//二次校正是否完成


	CString *pstr;//消息传递



	// 检查输入文件扩展名

	int checkext(CString filename);
	bool checklim(void);
};
