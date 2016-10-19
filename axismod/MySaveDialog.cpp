// MySaveDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "axismod.h"
#include "MySaveDialog.h"


// MySaveDialog 对话框

IMPLEMENT_DYNAMIC(MySaveDialog, CDialog)

MySaveDialog::MySaveDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MySaveDialog::IDD, pParent)
	, linetext(_T(""))
{

}

MySaveDialog::~MySaveDialog()
{
}

void MySaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, linetext);
}


BEGIN_MESSAGE_MAP(MySaveDialog, CDialog)
END_MESSAGE_MAP()


// MySaveDialog 消息处理程序

void MySaveDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	this->UpdateData(true);
	if(linetext.IsEmpty())
	{
		MessageBox("线路名称不允许留白，请认真填写！");
		return;
	}
	CDialog::OnOK();
}
