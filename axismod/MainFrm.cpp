// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "axismod.h"
#include "MyForm.h"
#include "MyView.h"
#include "MainFrm.h"
#include "MyViewresult.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;  
	}
	HICON ico = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(ico,true);

	EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	CRect tem_ClientRect;
	this->GetClientRect(&tem_ClientRect);
	tem_ClientRect.bottom -= 30;
	CSize siz_Control;
	CSize siz_View;
	siz_Control.cx = tem_ClientRect.Width();
	siz_Control.cy = 220;
	siz_View.cx = tem_ClientRect.Width()/2;
	siz_View.cy = tem_ClientRect.Height()-siz_Control.cy;
	splittery.CreateStatic(this,2,1);
	splitterx.CreateStatic(&splittery,1,2,WS_CHILD|WS_VISIBLE,splittery.IdFromRowCol(0,0));
	splittery.CreateView(1,0,RUNTIME_CLASS(MyForm),SIZE()/*siz_Control*/,pContext);
	splitterx.CreateView(0,0,RUNTIME_CLASS(MyView),SIZE()/*siz_View*/,pContext);
	splitterx.CreateView(0,1,RUNTIME_CLASS(MyViewresult),SIZE()/*siz_View*/,pContext);

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CRect tem_ClientRect;
	this->GetClientRect(&tem_ClientRect);
	tem_ClientRect.bottom -= 30;
	CSize siz_Control;
	CSize siz_View;
	siz_Control.cx = tem_ClientRect.Width();
	siz_Control.cy = 250;
	siz_View.cx = tem_ClientRect.Width()/2;
	siz_View.cy = tem_ClientRect.Height()-siz_Control.cy;

	if(siz_View.cy<=0)
		siz_View.cy = 1;
	splittery.SetRowInfo(0,siz_View.cy ,0 );
	splittery.SetRowInfo(1,250,0);
	splitterx.SetColumnInfo(0,siz_View.cx,0);
	splitterx.SetColumnInfo(0,siz_View.cx,0);
	CFrameWnd::OnSize(nType, cx, cy);
}
