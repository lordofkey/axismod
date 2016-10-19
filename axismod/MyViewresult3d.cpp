// MyViewresult3d.cpp : 实现文件
//
#include "stdafx.h"
#include "axismod.h"
#include "MyViewresult3d.h"


// MyViewresult3d

IMPLEMENT_DYNCREATE(MyViewresult3d, CView)

MyViewresult3d::MyViewresult3d()
: issave(false)
, isdown(false)
, op(0)
, rangle(0)
, camdis(3000)
{
	m_GLPixelIndex = 0;
	m_hGLContext = NULL;
}

MyViewresult3d::~MyViewresult3d()
{
}

BEGIN_MESSAGE_MAP(MyViewresult3d, CView)
	ON_MESSAGE(WM_REFRESH,&MyViewresult3d::OnRefresh)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()
// MyViewresult3d 绘图


void MyViewresult3d::OnDraw(CDC* pDC)
{
}

// MyViewresult3d 诊断

#ifdef _DEBUG
void MyViewresult3d::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void MyViewresult3d::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

LRESULT MyViewresult3d::OnRefresh(WPARAM wParam, LPARAM lParam)  
{
	layern = wParam;
	if(lParam!=NULL)
	{
		this->OnPaint();
		issave = true;
		return 0;
	}
	this->Invalidate();
	return 0;
}
// MyViewresult3d 消息处理程序

void MyViewresult3d::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	Mymodfunc::GetInstance()->h_viewm3d = this->m_hWnd;
}

BOOL MyViewresult3d::OnEraseBkgnd(CDC* pDC)
{
	return true;
//	return CView::OnEraseBkgnd(pDC);
}

BOOL MyViewresult3d::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CView::PreCreateWindow(cs);
}

bool MyViewresult3d::SetWindowPixelFormat(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pixelDesc=
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|
        PFD_DOUBLEBUFFER|PFD_SUPPORT_GDI,
        PFD_TYPE_RGBA,
        24,
        0,0,0,0,0,0,
        0,
        0,
        0,
        0,0,0,0,
        32,
        0,
        0,
        PFD_MAIN_PLANE,
        0,
        0,0,0
    };

    this->m_GLPixelIndex = ChoosePixelFormat(hDC,&pixelDesc);
    if(this->m_GLPixelIndex==0)
    {
        this->m_GLPixelIndex = 1;
        if(DescribePixelFormat(hDC,this->m_GLPixelIndex,sizeof(PIXELFORMATDESCRIPTOR),&pixelDesc)==0)
        {
            return FALSE;
        }
    }

    if(SetPixelFormat(hDC,this->m_GLPixelIndex,&pixelDesc)==FALSE)
    {
        return FALSE;
    }
    return TRUE;
}

bool MyViewresult3d::CreateViewGLContext(HDC hDC)
{
	this->m_hGLContext = wglCreateContext(hDC);
	if(this->m_hGLContext==NULL)
	{//创建失败
		return FALSE;
	}
	
	if(wglMakeCurrent(hDC,this->m_hGLContext)==FALSE)
	{//选为当前RC失败
		return FALSE;
	}
	return	TRUE;
}

int MyViewresult3d::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	initGL();
	return 0;
}

void MyViewresult3d::OnDestroy()
{
	CView::OnDestroy();
	destroyGL();
	return;
}

void MyViewresult3d::OnPaint()
{
	// Clear the screen
	CPaintDC dc(this);

	// Use our shader
	int itt;
	int iptt;
	getData(itt, iptt);
	glClear( GL_COLOR_BUFFER_BIT );
	//顶点传值到GPU
	glBufferSubData(GL_ARRAY_BUFFER, 0, (itt+iptt)*4*sizeof(float), data); 
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//顶点指向GPUbuffer
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                 // stride
		(void*)0            // array buffer offset
	);
	//translate矩阵生成
	float rot[16], vm[16], tmp[16], ts[16], mvp[16];
	creatrotmatrix(1,rangle,rot);
	getVMatrix2(4000*camdis/3000, 6000*camdis/3000, -20000*camdis/3000, 0, 0, 0, vm);
	getTS(60, 1, 1000, 100000, ts);
	multiplyMatrix(vm,rot,tmp);
	multiplyMatrix(ts,tmp,mvp);
	/////////////////////////////////
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, mvp); //translate矩阵赋值
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);  //开启可变定点大小
	glDrawArrays(GL_POINTS, 0, itt); // 3 indices starting at 0 -> 1 triangle
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS,itt,iptt);
	glDisableVertexAttribArray(0);
	SwapBuffers(hDC);
}


void MyViewresult3d::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	GLsizei width,height;
    width = cx;
    height = cy;
    glViewport(0,0,width,height);
}

// opengl初始化
void MyViewresult3d::initGL(void)
{
	hDC = this->GetDC()->m_hDC;
	if(this->SetWindowPixelFormat(hDC)==FALSE)
	{
		return;
	}
	if(this->CreateViewGLContext(hDC)==FALSE)
	{
		return;
	}
	glewExperimental = true; 
	glewInit();//初始化glew
	programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	glGenBuffers(1, &vertexbuffer);//申请GPU内存
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);//绑定内存号
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*(200*360+4*1000), NULL, GL_DYNAMIC_DRAW);//为当前内存区块初始化
	MatrixID = glGetUniformLocation(programID, "MVP");//获取MVPuniform变量指针
	glUseProgram(programID);
	glClearColor(0.3,0.3,0.3,1.0);
}

// 回收gl资源
void MyViewresult3d::destroyGL(void)
{
	if(wglGetCurrentContext()!=NULL)
	{
		wglMakeCurrent(NULL,NULL);
	}
	if(this->m_hGLContext!=NULL)
	{
		wglDeleteContext(this->m_hGLContext);
		this->m_hGLContext = NULL;
	}
}

BOOL MyViewresult3d::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	camdis += zDelta;
	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void MyViewresult3d::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	isdown = true;
	op = point;
	CView::OnLButtonDown(nFlags, point);
}

void MyViewresult3d::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	isdown = false;
	CView::OnLButtonUp(nFlags, point);
}

void MyViewresult3d::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(isdown)
	{
		rangle += (point-op).cx;
		this->Invalidate();
	}
	op = point;
	CView::OnMouseMove(nFlags, point);
}

void MyViewresult3d::getData(int &itt,int &iptt)
{
	int curlayn;
	int *x = NULL;   //需要显示的x坐标指针
	int *y = NULL;   //需要显示的y坐标指针
	int nump;
	itt = 0;
	for(int z = -150; z<150; z++)
	{
		curlayn = layern + z;
		if(curlayn >0&&curlayn<Mymodfunc::GetInstance()->totallayer)
		{
			if(Mymodfunc::GetInstance()->Getpiecenmodl(curlayn, &x , &y, nump))
			{
				for(int i = 0;i<nump;i++)
				{
					if((x[i] > -2100)&&(x[i] < 2100)&&(y[i] > 600)&&(y[i] < 5550))
					{
						
						data[4*itt] = x[i];
						data[4*itt+1] = y[i];
						data[4*itt+2] = z*148;
						data[4*itt+3] = framdet(x[i], y[i]);
						itt++;
					}
				}
			}
			if(Mymodfunc::GetInstance()->Getpiecenmodr(curlayn,&x ,&y,nump))
			{
				for(int i = 0;i<nump;i++)
				{
					if((x[i] > -2100)&&(x[i] < 2100)&&(y[i] > 600)&&(y[i] < 5550))
					{
						data[4*itt] = x[i];
						data[4*itt+1] = y[i];
						data[4*itt+2] = z*148;
						data[4*itt+3] = framdet(x[i], y[i]);
						itt++;
					}
				}
			}
		}
	}
	int numt = Mymodfunc::GetInstance()->set.cpar.num;
	int *stp = Mymodfunc::GetInstance()->set.cpar.stpoint;
	iptt = 0;
	for(int i = 0; i < numt; i++)
	{
		int relalayer = stp[i] - layern;
		if(relalayer>-150&&relalayer<150)
		{
			data[4*(itt+iptt)] = -2000;
			data[4*(itt+iptt)+1] = 0;
			data[4*(itt+iptt)+2] = relalayer*148;
			data[4*(itt+iptt)+3] = 99.231;
			iptt++;
			data[4*(itt+iptt)] = 2000;
			data[4*(itt+iptt)+1] = 0;
			data[4*(itt+iptt)+2] = relalayer*148;
			data[4*(itt+iptt)+3] = 99.231;
			iptt++;
			data[4*(itt+iptt)] = 2000;
			data[4*(itt+iptt)+1] = 5000;
			data[4*(itt+iptt)+2] = relalayer*148;
			data[4*(itt+iptt)+3] = 99.231;
			iptt++;
			data[4*(itt+iptt)] = -2000;
			data[4*(itt+iptt)+1] = 5000;
			data[4*(itt+iptt)+2] = relalayer*148;
			data[4*(itt+iptt)+3] = 99.231;
			iptt++;
		}
	}
}
