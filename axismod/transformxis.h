//Date:2016-06-25 
//Author:杨浩
//Description:此文件用于直角坐标系与屏幕坐标系相互转换，勿动
//            若修改此文件，请重新编译所有引用此文件的CPP文件
class transformaxis
{
private:
	float ax,bx,ay,by;
public:
//输入：clientrect 用于显示的hDC的rect    xs-x轴起始位置 xe-x轴截止位置  ys-y轴起始位置  ye - y轴截止位置 
	transformaxis(CRect clientrect,float xs,float xe,float ys,float ye)
	{ 
		ax = (float)clientrect.Width()/(xe-xs);
		bx = -xs * ax;
		ay = (float)clientrect.Height()/(ys-ye);
		by = -ay*ye;
	}
//屏幕坐标-》原始坐标
	float getox(int x)
	{
		return ((float)x-bx)/ax;
	}
	float getoy(int y)
	{
		return ((float)y-by)/ay;
	}
//原始坐标-》屏幕坐标
	int gettx(int x)
	{
		return (int)(ax*(float)x+bx);
	}
	int getty(int y)
	{
		return (int)(ay*(float)y+by);
	}
};