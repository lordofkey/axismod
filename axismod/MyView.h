#pragma once
#include "atltypes.h"





// MyView 视图

class MyView : public CView
{
	DECLARE_DYNCREATE(MyView)

protected:
	MyView();           // 动态创建所使用的受保护的构造函数
	virtual ~MyView();

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
//	afx_msg void OnOpen();
	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
//	afx_msg void On32773();
	virtual void OnInitialUpdate();
	
	
	int layern;// 第n层的雷达点云数据

	
	CPoint Lstpoint;// 左侧轨道起始位置	
	CPoint Lenpoint;// 左侧截止点
	bool Lisdown;// 左键是否按下

	CPoint Rstpoint;// 左侧轨道起始位置	
	CPoint Renpoint;// 左侧截止点
	bool Risdown;// 左键是否按下




	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int isrl;
};


