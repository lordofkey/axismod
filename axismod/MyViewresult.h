#pragma once
#include "transformxis.h"
#include "limdata.h"

// MyViewresult ��ͼ


class MyViewresult : public CView
{
	DECLARE_DYNCREATE(MyViewresult)

protected:
	MyViewresult();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~MyViewresult();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam=NULL);
	int layern;// ��n����״��������
	virtual void OnInitialUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void drawlim(CDC* pdc,transformaxis trans);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	bool isinlim(int* x, int* y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	CPoint Lstpoint;// KUANG
	CPoint Lenpoint;// KUANG
	bool Lisdown;// KUANG
	bool iswritedata;

	bool saveimg;
//	afx_msg void OnPaint();
};


