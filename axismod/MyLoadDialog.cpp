// MyLoadDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "axismod.h"
#include "MyLoadDialog.h"
#include "Mymodfunc.h"
// MyLoadDialog �Ի���

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


// MyLoadDialog ��Ϣ�������


BOOL MyLoadDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	for(int i = 0;i<(int)str_lines.size();i++)
		Blinelist.AddString(str_lines[i]);
	return TRUE;  // return TRUE unless you set the focus to a control
}


void MyLoadDialog::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	CString sline = "";
	CString stime = "";
	std::vector<CString> spar;
	int selid = 0;
	selid = Blinelist.GetCurSel();
	if(selid<0)
	{
		MessageBox("��ѡ��궨��·");
		return;
	}
	Blinelist.GetText(selid,sline);
	selid = Bsetlist.GetCurSel();
	if(selid<0)
	{
		MessageBox("��ѡ��궨ʱ��");
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
		MessageBox("��ѡ��궨ʱ��");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

