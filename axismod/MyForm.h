#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// MyForm 窗体视图

class MyForm : public CFormView
{
	DECLARE_DYNCREATE(MyForm)

protected:
	MyForm();           // 动态创建所使用的受保护的构造函数
	virtual ~MyForm();

public:
	enum { IDD = IDD_MYFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT timer0;
	afx_msg void OnBnClickedButton1();
	// 控制层数的slider
	CSliderCtrl layerslider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton3();
	CEdit layernedit;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton5();
//一次校正参数	
	float roadwid;
	float roadhight;
	float roaddis;
//二次校正参数
	float carhight;
	float carwidth;
	// 车厢起始位置
	int carst;
	// 车厢结束位置
	int caren;

	// 标准车型数据
	struct{int id,height,width;} trains[1000];

	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	virtual void OnInitialUpdate();
	int ruidalr;
	afx_msg void OnBnClickedButton8();
	int trainid;
	afx_msg void OnBnClickedButton10();
	CString e_filename;
	afx_msg void OnBnClickedButton9();
	// 刷新宽高数据
	void par_refresh(void);
	afx_msg LRESULT OnMessageOut(WPARAM,LPARAM);
	// log
	CString logtext;

	BOOL UpdateData(BOOL bSaveAndValidate = TRUE);

	CEdit mesedit;
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
};