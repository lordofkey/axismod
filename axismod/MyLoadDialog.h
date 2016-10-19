#pragma once


// MyLoadDialog 对话框

class MyLoadDialog : public CDialog
{
	DECLARE_DYNAMIC(MyLoadDialog)

public:
	MyLoadDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyLoadDialog();

// 对话框数据
	enum { IDD = IDD_MYLOADDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString str_lines;
};
