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
//	ON_LBN_SETFOCUS(IDC_LIST1, &MyLoadDialog::OnLbnSetfocusList1)
	ON_LBN_SELCHANGE(IDC_LIST1, &MyLoadDialog::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// MyLoadDialog 消息处理程序


BOOL MyLoadDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	for(int i = 0;i<str_lines.size();i++)
		Blinelist.AddString(str_lines[i]);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void MyLoadDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CString sline = "";
	CString stime = "";
	std::vector<CString> spar;
	Blinelist.GetText(Blinelist.GetCurSel(),sline);
	Bsetlist.GetText(Bsetlist.GetCurSel(),stime);
	pset->Loadsetting(sline,stime,spar);
	Mymodfunc::GetInstance()->x_delta[0] = atof(spar[0]);
	Mymodfunc::GetInstance()->x_delta[1] = atof(spar[3]);
	Mymodfunc::GetInstance()->y_delta[0] = atof(spar[1]);
	Mymodfunc::GetInstance()->y_delta[1] = atof(spar[4]);
	Mymodfunc::GetInstance()->ang_delta[0] = atof(spar[2])/57.3;
	Mymodfunc::GetInstance()->ang_delta[1] = atof(spar[5])/57.3;
	Mymodfunc::GetInstance()->isfirmod[0] = TRUE;
	Mymodfunc::GetInstance()->isfirmod[1] = TRUE;
	Mymodfunc::GetInstance()->updatahash();
	CDialog::OnOK();
}


//void MyLoadDialog::OnLbnSetfocusList1()
//{
//	MessageBox("abc");
//	// TODO: 在此添加控件通知处理程序代码
//}


void MyLoadDialog::OnLbnSelchangeList1()
{
	int selid = Blinelist.GetCurSel();
	CString str = "";
	Blinelist.GetText(selid,str);
	std::vector<CString> sets;
	pset->getsettings(str,sets);
	Bsetlist.ResetContent();
	for(int i = 0;i<sets.size();i++)
		Bsetlist.AddString(sets[i]);
	// TODO: 在此添加控件通知处理程序代码
}