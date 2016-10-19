#pragma once

class Mymodfunc
{

#pragma region single cls //实现单例对象
private:
	class Deleter
	{
	public:
		~Deleter()
		{
			if (Mymodfunc::m_pInstance != NULL)
			{
				delete Mymodfunc::m_pInstance;
				Mymodfunc::m_pInstance = NULL;
			}
		}
	};
	static Mymodfunc* m_pInstance;
	Mymodfunc(void);
	virtual ~Mymodfunc();   
public:
	HWND h_view;//视图窗口句柄
	HWND h_viewm; //校正后的视图句柄
	HWND h_form;//控制面板句柄
	static Mymodfunc *Mymodfunc::GetInstance();

#pragma endregion

public:
	// 读取点云文件
	bool readfile(CString filename,CString path);
	// 获取第n个断片的数据
	bool Getpiecen(int n, int** x, int** y);
	bool Getpiecenmod(int n, int** x, int** y);
	bool Getpiecenmodr(int n, int** x, int** y);
	bool Getpiecenmod2(int n, int** x, int** y);
	bool Getpiecenmodr2(int n, int** x, int** y);
	bool firstmod(float height, float width, float distan,int carst ,int caren);//// 第一次校正计算
	bool seconmod(float width, float height,int carst ,int caren);// 二次校正
	bool traincut();//自动切车


	// 总层数
	int totallayer;
	short *lpdata;//雷达数据
	float sint[360];//角度对应sin值
	float cost[360];//角度对应cos值


	float lroix_min,lroix_max,lroiy_min,lroiy_max,rroix_min,rroix_max,rroiy_min,rroiy_max;//轨道标定区域
	float mroix_min,mroix_max,mroiy_min,mroiy_max;//标准车标定区域


	CString inifilename;


	float sinm[360];//角度对应sin值
	float cosm[360];//角度对应cos值
	///一次校正输出
	float ang_delta[2]; 
	float x_delta[2];
	float y_delta[2];
	// 一次校正
	bool isfirmod[2];
	float w1[2];
	float w2[2];
	bool issecmod[2];//一次校正是否完成
	int islr;//正在校正的雷达

	int stps[200];//切车开始位置
	int lens[200];//车长
	int cc;//切车标号

	CString source;

	CString *pstr;//消息传递

	CString version;
};
