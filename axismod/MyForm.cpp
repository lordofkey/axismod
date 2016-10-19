// MyForm.cpp : 实现文件
//

#include "stdafx.h"
#include "axismod.h"
#include "MyForm.h"
#include "Mmsystem.h"
#include "limdata.h"
#include "MySaveDialog.h"
#include "MySetting.h"
#include "MyLoadDialog.h"
#include <Exception>
#pragma comment(lib,"Winmm.lib")


IMPLEMENT_DYNCREATE(MyForm, CFormView)

MyForm::MyForm()
: CFormView(MyForm::IDD),
timer0(NULL)
, roadwid(1450)
, roadhight(6100)
, roaddis(0)
, carhight(0)
, carwidth(0)
, carst(0)
, caren(0)
, ruidalr(0)
, trainid(0)
, e_filename(_T(""))
, logtext(_T(""))
{
	memset(trains,0,sizeof(trains));
}

MyForm::~MyForm()
{
}

void MyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, layerslider);
	DDX_Control(pDX, IDC_EDIT1, layernedit);
	DDX_Text(pDX, IDC_EDIT2, roadwid);
	DDX_Text(pDX, IDC_EDIT3, roadhight);
	DDX_Text(pDX, IDC_EDIT4, roaddis);
	DDX_Text(pDX, IDC_EDIT5, carwidth);
	DDX_Text(pDX, IDC_EDIT6, carhight);
	DDX_Text(pDX, IDC_EDIT7, carst);
	DDX_Text(pDX, IDC_EDIT8, caren);
	DDX_Radio(pDX, IDC_RADIO1, ruidalr);
	DDX_Text(pDX, IDC_EDIT13,trainid);
	DDX_Text(pDX, IDC_FILE, e_filename);
	DDX_Text(pDX, IDC_EDIT9, logtext);
	DDX_Control(pDX, IDC_EDIT9, mesedit);
}

BEGIN_MESSAGE_MAP(MyForm, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &MyForm::OnBnClickedButton1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON2, &MyForm::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &MyForm::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &MyForm::OnBnClickedButton4)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON5, &MyForm::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &MyForm::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &MyForm::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_RADIO1, &MyForm::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &MyForm::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON8, &MyForm::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON10, &MyForm::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON9, &MyForm::OnBnClickedButton9)
	ON_MESSAGE(WM_MESSOUT, &MyForm::OnMessageOut)
	ON_BN_CLICKED(IDC_BUTTON11, &MyForm::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &MyForm::OnBnClickedButton12)
END_MESSAGE_MAP()


// MyForm 诊断

#ifdef _DEBUG
void MyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void MyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// MyForm 消息处理程序
//导入雷达数据
void MyForm::OnBnClickedButton1()//button 导入雷达数据
{
	CFileDialog diag(true,0,0,4|2,"雷达数据文件(*.lms)|*.lms");
	CString filename;
	CString path;
	if(IDOK == diag.DoModal())
	{
		filename = diag.GetPathName();
		path = diag.GetFolderPath();
		this->UpdateData(true);
		e_filename = filename;
		this->UpdateData(false);

		CString *pStr = new CString("正在加载雷达数据");
		::PostMessage(this->m_hWnd,WM_MESSOUT,(WPARAM)pStr,NULL);

		if(Mymodfunc::GetInstance()->readfile(filename,path))
		{
			CString *pStr = new CString("雷达数据加载成功");
			::PostMessage(this->m_hWnd,WM_MESSOUT,(WPARAM)pStr,NULL);
		}
		else
		{
			CString *pStr = new CString("雷达数据加载失败");
			::PostMessage(this->m_hWnd,WM_MESSOUT,(WPARAM)pStr,NULL);
		}

		::SendMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,0,NULL);
		::SendMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,0,NULL);
		layerslider.SetRange(0,Mymodfunc::GetInstance()->totallayer-1);
		layerslider.SetPageSize(1);
	}
}

void MyForm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	char tembuf[100];
	_itoa_s(layerslider.GetPos(),tembuf,100,10);
	layernedit.SetWindowTextA(tembuf);
	::SendMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),NULL);
	::SendMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CALLBACK onTimeFunc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
	MyForm *pform = (MyForm *)dwUser;
	pform->layerslider.SetPos(pform->layerslider.GetPos()+1);
	char tembuf[100];
	_itoa_s(pform->layerslider.GetPos(),tembuf,100,10);
	pform->layernedit.SetWindowTextA(tembuf);
	::PostMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,pform->layerslider.GetPos(),NULL);
	::PostMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,pform->layerslider.GetPos(),NULL);
}

//播放
void MyForm::OnBnClickedButton2()
{

	//	this->SetTimer(1,100,0);
	if(timer0 == NULL)
	{
		timer0 = timeSetEvent(40,1,onTimeFunc,(DWORD_PTR)this,TIME_PERIODIC);
	}
}

void MyForm::OnTimer(UINT_PTR nIDEvent)
{
	layerslider.SetPos(layerslider.GetPos()+1);
	char tembuf[100];
	_itoa_s(layerslider.GetPos(),tembuf,100,10);
	layernedit.SetWindowTextA(tembuf);
	::PostMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),NULL);
	::PostMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
	CFormView::OnTimer(nIDEvent);
}

//停止播放
void MyForm::OnBnClickedButton3()
{
	//	this->KillTimer(1);
	if(timer0 != NULL)
	{
		timeKillEvent(timer0);
		timer0 = NULL;
	}
	// TODO: 在此添加控件通知处理程序代码
}
//校正
void MyForm::OnBnClickedButton4()
{
	this->UpdateData();
	//Mymodfunc::GetInstance()->firstmod(6649,2000,3320.5);
	if(Mymodfunc::GetInstance()->firstmod(this->roadhight,this->roadwid,this->roaddis,carst,caren))
	{
		::SendMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),NULL);
		::SendMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
	}
	else
		MessageBox("参数不合法！请检查后重新校正！");
	//if(Mymodfunc::GetInstance()->islr == 0)
	//{
	//	if(Mymodfunc::GetInstance()->firstmod(5514,1502,1518,carst,caren))
	//	{
	//		::SendMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),NULL);
	//		::SendMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
	//	}
	//	else
	//		MessageBox("参数不合法！请检查后重新校正！");
	//}
	//else
	//{
	//	if(Mymodfunc::GetInstance()->firstmod(5630,1502,1472,carst,caren))
	//	{
	//		::SendMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),NULL);
	//		::SendMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
	//	}
	//	else
	//		MessageBox("参数不合法！请检查后重新校正！");
	//}

}
void MyForm::OnDestroy()
{
	CFormView::OnDestroy();
	if(timer0 != NULL)
	{
		timeKillEvent(timer0);
		timer0 = NULL;
	}
}
//二次校正
void MyForm::OnBnClickedButton5()
{
	UpdateData(true);
	Mymodfunc::GetInstance()->seconmod(carwidth,carhight,carst,caren);
	::SendMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),NULL);
	::SendMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
}

void MyForm::OnBnClickedButton6()
{
	this->UpdateData(true);
	carst = layerslider.GetPos();
	this->UpdateData(false);
}

void MyForm::OnBnClickedButton7()
{
	this->UpdateData(true);
	caren = layerslider.GetPos();
	this->UpdateData(false);
}

void MyForm::OnBnClickedRadio1()
{
	this->UpdateData(true);
	Mymodfunc::GetInstance()->islr = this->ruidalr;
	::SendMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),1);
	::SendMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
}

void MyForm::OnBnClickedRadio2()
{
	this->UpdateData(true);
	Mymodfunc::GetInstance()->islr = this->ruidalr;
	::SendMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),2);
	::SendMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
}



void MyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	Mymodfunc::GetInstance()->h_form = this->m_hWnd;
	// TODO: 在此添加专用代码和/或调用基类
}

//切车
void MyForm::OnBnClickedButton8()
{
	this->UpdateData(true);
	if(--trainid<1)
		trainid = 0;
	this->carst = Mymodfunc::GetInstance()->stps[trainid];
	this->caren = Mymodfunc::GetInstance()->stps[trainid] + Mymodfunc::GetInstance()->lens[trainid];
	this->UpdateData(false);
	layerslider.SetPos(Mymodfunc::GetInstance()->stps[trainid]);
	char tembuf[100];
	_itoa_s(layerslider.GetPos(),tembuf,100,10);
	layernedit.SetWindowTextA(tembuf);
	::PostMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),NULL);
	::PostMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
	this->par_refresh();
}

void MyForm::OnBnClickedButton10()
{
	this->UpdateData(true);
	if(++trainid>Mymodfunc::GetInstance()->cc-1)
		trainid = Mymodfunc::GetInstance()->cc - 1;
	this->carst = Mymodfunc::GetInstance()->stps[trainid];
	this->caren = Mymodfunc::GetInstance()->stps[trainid] + Mymodfunc::GetInstance()->lens[trainid];
	this->UpdateData(false);
	layerslider.SetPos(Mymodfunc::GetInstance()->stps[trainid]);
	char tembuf[100];
	_itoa_s(layerslider.GetPos(),tembuf,100,10);
	layernedit.SetWindowTextA(tembuf);
	::PostMessageA(Mymodfunc::GetInstance()->h_view,WM_REFRESH,layerslider.GetPos(),NULL);
	::PostMessageA(Mymodfunc::GetInstance()->h_viewm,WM_REFRESH,layerslider.GetPos(),NULL);
	this->par_refresh();
}

///打开车辆信息
void MyForm::OnBnClickedButton9()
{
	CFileDialog diag(true,0,0,4|2,"过车数据(*.txt)|*.txt");
	CString filename;

	if(IDOK == diag.DoModal())
	{
		filename = diag.GetPathName();
		CStdioFile file;
		if(file.Open(filename,CFile::modeRead))
		{
			CString line;

			std::vector<CString> temstr;
			int num_train = 0;
			while(file.ReadString(line))
			{
				int pos = 0;
				temstr.clear();
				while(pos<line.GetLength())
				{
					temstr.push_back(line.Tokenize("\t",pos));
				}
				trains[num_train].id = atoi(temstr[7]);

				for(int i = 0;i<(310-146);i++)
				{
					int pos1 = 0;
					if(temstr[5].Tokenize(" ",pos1) == traintype[i].type)
					{
						trains[num_train].height = traintype[i].height;
						trains[num_train].width = traintype[i].width;
						break;
					}
					trains[num_train].height = 0;
					trains[num_train].width = 0;
				}


				num_train++;
			}

		}
		else
		{
			MessageBox("文件打开失败!");
		}

		layerslider.SetRange(0,Mymodfunc::GetInstance()->totallayer-1);
		layerslider.SetPageSize(1);
	}
	this->par_refresh();
}

// 刷新宽高数据
void MyForm::par_refresh(void)
{
	this->UpdateData(true);
	this->carhight = trains[trainid].height;
	this->carwidth = trains[trainid].width;
	this->UpdateData(false);
}

LRESULT MyForm::OnMessageOut(WPARAM wParam,LPARAM lParam)
{
	CTime now = CTime::GetCurrentTime();
	CString *pstr = (CString *)wParam;
	this->UpdateData(true);
	logtext += now.Format("%Y-%m-%d %H:%M:%S: ");
	logtext += *pstr;
	logtext += "\r\n";
	this->UpdateData(false);
	delete pstr;
	return 0;

}

BOOL MyForm::UpdateData(BOOL bSaveAndValidate)
{
	BOOL r;
	r = CWnd::UpdateData(bSaveAndValidate);

	mesedit.LineScroll(mesedit.GetLineCount()); 
	return r;
}

void MyForm::OnBnClickedButton11()
{
	MySaveDialog temDialog;
	if(IDOK == temDialog.DoModal())
	{
		CString linename = temDialog.linetext;
		MySetting set;
		this->UpdateData(true);
		set.calipar[0] = roadwid;
		set.calipar[1] = roadhight;
		set.calipar[2] = roaddis;
		set.lradar[0] = Mymodfunc::GetInstance()->x_delta[0];
		set.lradar[1] = Mymodfunc::GetInstance()->y_delta[0];
		set.lradar[2] = Mymodfunc::GetInstance()->ang_delta[0];
		set.lradar[3] = 0 ;
		set.lradar[4] = Mymodfunc::GetInstance()->w1[0];
		set.lradar[5] = Mymodfunc::GetInstance()->w2[0];

		set.rradar[0] = Mymodfunc::GetInstance()->x_delta[1];
		set.rradar[1] = Mymodfunc::GetInstance()->y_delta[1];
		set.rradar[2] = Mymodfunc::GetInstance()->ang_delta[1];
		set.rradar[3] = 0 ;
		set.rradar[4] = Mymodfunc::GetInstance()->w1[1];
		set.rradar[5] = Mymodfunc::GetInstance()->w2[1];

		set.writesetting(linename);
	};
}


void MyForm::OnBnClickedButton12()
{
	MySetting set;
	MyLoadDialog dig;
	dig.DoModal();
}
