#pragma once
#include "transformxis.h"
// MyViewresult3d ��ͼ
#include "../gltr/gltr.h"
#pragma comment(lib,"..\\debug\\gltr.lib")

class MyViewresult3d : public CView
{
	DECLARE_DYNCREATE(MyViewresult3d)
protected:
	MyViewresult3d();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~MyViewresult3d();

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
	bool issave;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	HDC hDC;
	int m_GLPixelIndex;
	bool SetWindowPixelFormat(HDC hDC);
	bool CreateViewGLContext(HDC hDC);
	HGLRC m_hGLContext;
	GLuint programID;
	GLuint vertexbuffer;
	GLuint MatrixID;
	float data[201*360*4];
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	bool isdown;
	CPoint op;
	float rangle;
public:
	// opengl��ʼ��
	void initGL(void);
	// ����gl��Դ
	void destroyGL(void);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	void getData(int &itt, int &iptt);
private:
	// �������
	int camdis;
};


