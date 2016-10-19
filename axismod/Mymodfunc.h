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

#pragma region single cls //ʵ�ֵ�������
private:
	static Mymodfunc* m_pInstance;
	Mymodfunc(void);
	virtual ~Mymodfunc();   
public:
	static Mymodfunc *Mymodfunc::GetInstance();
	void release();
#pragma endregion


private:
	int *lpdata;//�״�����
	int pnum;//ÿ���Ӧ�ĵ���
	MyRect l_range;
	MyRect r_range;
	//У��ǰ
	float l_sint[540]; //�Ƕȶ�Ӧsinֵ
	float l_cost[540]; //�Ƕȶ�Ӧcosֵ
	float r_sint[540]; //�Ƕȶ�Ӧsinֵ
	float r_cost[540]; //�Ƕȶ�Ӧcosֵ

	//У����
	float l_sinm[540]; //�Ƕȶ�Ӧsinֵ
	float l_cosm[540]; //�Ƕȶ�Ӧcosֵ
	float r_sinm[540]; //�Ƕȶ�Ӧsinֵ
	float r_cosm[540]; //�Ƕȶ�Ӧcosֵ
public:
	// ��ȡ�����ļ�
	bool readfile(CString filename,int& layers);
	// ��ȡ��n����Ƭ������
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
	bool firstmod(ModPar par);//// ��һ��У������
	bool seconmod(int islr, float width, float height,int carst ,int caren);// ����У��
	bool traincut();//�Զ��г�


	HWND h_view;//��ͼ���ھ��
	HWND h_viewm; //У�������ͼ���
	HWND h_form;//���������


	MySetting set;//�궨����
	int totallayer;// �ܲ���


	bool isfirmod[2];//һ��У���Ƿ����
	bool issecmod[2];//����У���Ƿ����


	CString *pstr;//��Ϣ����



	// ��������ļ���չ��

	int checkext(CString filename);
	bool checklim(void);
};
