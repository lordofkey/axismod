// MyViewresult.cpp : 实现文件
//

#include "stdafx.h"
#include "axismod.h"
#include "MyViewresult.h"
#include "atlimage.h"
extern "C"
{
	_declspec(dllimport) double framdet(double dx, double dy);
}
// MyViewresult

IMPLEMENT_DYNCREATE(MyViewresult, CView)

MyViewresult::MyViewresult()
: issave(false)
{
}

MyViewresult::~MyViewresult()
{
}

BEGIN_MESSAGE_MAP(MyViewresult, CView)
	ON_MESSAGE(WM_REFRESH,&MyViewresult::OnRefresh)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// MyViewresult 绘图

void MyViewresult::OnDraw(CDC* pDC)
{

	int *x = NULL;   //需要显示的x坐标指针
	int *y = NULL;   //需要显示的y坐标指针
	CRect drect;
	this->GetClientRect(&drect);
	transformaxis trans(drect,-4000,4000,-1000,6000);			    //设定显示范围

#pragma region 双缓冲initial
	CDC dcMem;														//用于缓冲作图的内存DC
	CBitmap bmp;													//内存中承载临时图象的位图
	dcMem.CreateCompatibleDC(pDC);									//依附窗口DC创建兼容内存DC
	bmp.CreateCompatibleBitmap(pDC,drect.Width(),drect.Height());	//创建兼容位图
	dcMem.SelectObject(&bmp);                          //将位图选择进内存DC
	dcMem.FillSolidRect(drect,RGB(125,125,125));//按原来背景填充客户区，不然会是黑色
#pragma endregion 双缓冲initial

	int nump = 0;
	int curlayn = 0;
	int tx[100000];
	int ty[100000];
	int tnum = 0;
#pragma region 雷达点绘图
	for(int z = -100; z<100; z++)
	{
		curlayn = layern + z;
		if(curlayn >0&&curlayn<Mymodfunc::GetInstance()->totallayer)
		{
			if(Mymodfunc::GetInstance()->Getpiecenmodl(curlayn,&x ,&y,nump))
			{
				for(int i = 0;i<nump;i++)
				{
					if((x[i] > -2100)&&(x[i] < 2100)&&(y[i] > 1250)&&(y[i] < 5550))
					{
						if(framdet(x[i], y[i])==0.0)
						{
							dcMem.SetPixel(trans.gettx(x[i], y[i], z),trans.getty(x[i], y[i], z),RGB(255,255,255));
						}
						else
						{
							tx[tnum] = trans.gettx(x[i], y[i], z);
							ty[tnum] = trans.getty(x[i], y[i], z);
							tnum++;
						}
					}
				}
				delete x;
				delete y;
			}
			if(Mymodfunc::GetInstance()->Getpiecenmodr(curlayn,&x ,&y,nump))
			{
				for(int i = 0;i<nump;i++)
				{
					if((x[i] > -2100)&&(x[i] < 2100)&&(y[i] > 1250)&&(y[i] < 5550))
					{
						if(framdet(x[i], y[i])==0.0)
						{
							dcMem.SetPixel(trans.gettx(x[i], y[i], z),trans.getty(x[i], y[i], z),  RGB(255,255,255));
						}
						else
						{
							tx[tnum] = trans.gettx(x[i], y[i], z);
							ty[tnum] = trans.getty(x[i], y[i], z);
							tnum ++;

						}
					}
				}

				delete x;
				delete y;
			}
		}
	}
	for(int i = 0; i < tnum; i++)
	{
		dcMem.SetPixel(tx[i],ty[i],RGB(255,0,0));
		dcMem.SetPixel(tx[i]+1,ty[i],RGB(255,0,0));
		dcMem.SetPixel(tx[i]-1,ty[i],RGB(255,0,0));
		dcMem.SetPixel(tx[i],ty[i]+1,RGB(255,0,0));
		dcMem.SetPixel(tx[i],ty[i]-1,RGB(255,0,0));
	}
#pragma endregion 雷达点绘图

	if(issave)
	{
		CImage imgTemp;     
        imgTemp.Attach(bmp.operator HBITMAP());
		CString tem;
		tem.Format("%d",this->layern);
		tem += ".BMP";
        imgTemp.Save(tem);
		issave = false;
	}
	pDC->BitBlt(0,0,drect.Width(),drect.Height(),&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台
	dcMem.DeleteDC();                                       //删除DC
	bmp.DeleteObject();
}


// MyViewresult 诊断

#ifdef _DEBUG
void MyViewresult::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void MyViewresult::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


LRESULT MyViewresult::OnRefresh(WPARAM wParam, LPARAM lParam)  
{
	layern = wParam;
	if(lParam!=NULL)
	{
		this->OnDraw(this->GetDC());
		issave = true;
		return 0;
	}
	this->Invalidate();
	return 0;
}
// MyViewresult 消息处理程序

void MyViewresult::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	Mymodfunc::GetInstance()->h_viewm = this->m_hWnd;
}

BOOL MyViewresult::OnEraseBkgnd(CDC* pDC)
{
	return true;
//	return CView::OnEraseBkgnd(pDC);
}