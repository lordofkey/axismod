

class transformaxis
{
private:
	float ax,bx,ay,by;
public:
	transformaxis(CRect clientrect,float xs,float xe,float ys,float ye)
	{ 
		ax = (float)clientrect.Width()/(xe-xs);
		bx = -xs * ax;
		ay = (float)clientrect.Height()/(ys-ye);
		by = -ay*ye;
	}
	float getox(int x)
	{
		return ((float)x-bx)/ax;
	}
	float getoy(int y)
	{
		return ((float)y-by)/ay;
	}
	int gettx(int x)
	{
		return (int)(ax*(float)x+bx);
	}
	int getty(int y)
	{
		return (int)(ay*(float)y+by);
	}
};