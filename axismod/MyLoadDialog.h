#pragma once
#include "afxwin.h"

// MyLoadDialog �Ի���

class MyLoadDialog : public CDialog
{
	DECLARE_DYNAMIC(MyLoadDialog)

public:
	MyLoadDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyLoadDialog();

// �Ի�������
	enum { IDD = IDD_MYLOADDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();


	CListBox Blinelist;
	virtual void OnOK();
	afx_msg void OnLbnSelchangeList1();
	CListBox Bsetlist;
	std::vector<CString> str_lines;
};
