#pragma once


// MySaveDialog 对话框

class MySaveDialog : public CDialog
{
	DECLARE_DYNAMIC(MySaveDialog)

public:
	MySaveDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MySaveDialog();

// 对话框数据
	enum { IDD = IDD_MYSAVEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	CString linetext;
};
