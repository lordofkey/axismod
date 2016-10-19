// MyLoadDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "axismod.h"
#include "MyLoadDialog.h"
#include "Mymodfunc.h"
// MyLoadDialog 对话框

IMPLEMENT_DYNAMIC(MyLoadDialog, CDialog)

MyLoadDialog::MyLoadDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MyLoadDialog::IDD, pParent)
{

}

MyLoadDialog::~MyLoadDialog()
{
}

void MyLoadDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, Blinelist);
	DDX_Control(pDX, IDC_LIST2, Bsetlist);
}


BEGIN_MESSAGE_MAP(MyLoadDialog, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, &MyLoadDialog::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// MyLoadDialog 消息处理程序


BOOL MyLoadDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	for(int i = 0;i<(int)str_lines.size();i++)
		Blinelist.AddString(str_lines[i]);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void MyLoadDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString sline = "";
	CString stime = "";
	std::vector<CString> spar;
	int selid = 0;
	selid = Blinelist.GetCurSel();
	if(selid<0)
	{
		MessageBox("请选择标定线路");
		return;
	}
	Blinelist.GetText(selid,sline);
	selid = Bsetlist.GetCurSel();
	if(selid<0)
	{
		MessageBox("请选择标定时间");
		return;
	}
	Bsetlist.GetText(selid,stime);
	if(Mymodfunc::GetInstance()->set.Loadsetting(sline,stime))
	{
		Mymodfunc::GetInstance()->isfirmod[0] = TRUE;
		Mymodfunc::GetInstance()->isfirmod[1] = TRUE;
		Mymodfunc::GetInstance()->updatahash();
	}
	else
	{
		MessageBox("请选择标定时间");
		return;
	}
	CDialog::OnOK();
}



void MyLoadDialog::OnLbnSelchangeList1()
{
	int selid = Blinelist.GetCurSel();
	CString str = "";
	Blinelist.GetText(selid,str);
	std::vector<CString> sets;
	Mymodfunc::GetInstance()->set.getsettings(str,sets);
	Bsetlist.ResetContent();
	for(int i = 0;i<(int)sets.size();i++)
		Bsetlist.AddString(sets[i]);
	// TODO: 在此添加控件通知处理程序代码
}

