#pragma once
#include "transformxis.h"
#include "limdata.h"

// MyViewresult 视图


class MyViewresult : public CView
{
	DECLARE_DYNCREATE(MyViewresult)

protected:
	MyViewresult();           // 动态创建所使用的受保护的构造函数
	virtual ~MyViewresult();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	int layern;// 第n层的雷达点云数据
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void drawlim(CDC* pdc,transformaxis trans);


	bool isinlim(int* x, int* y);
};


