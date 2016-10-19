#define DLL
#include "data.h"
#include "scan.h"
#include <cmath>
#include <stdio.h>
#define pi 3.141592653

point vertip(point stp, point enp, point detp)
{
	double x, y, lx, ly, nl, lxi, lyi, t, tpx, tpy;
	x = enp.x - detp.x;
	y = enp.y - detp.y;
	lx = enp.x - stp.x;
	ly = enp.y - stp.y;
	nl = sqrt(lx*lx+ly*ly);
	lxi = lx/nl;
	lyi = ly/nl;
	t = x*lxi+y*lyi;
	tpx = enp.x - t*lxi;
	tpy = enp.y - t*lyi;
	if(((tpx <= enp.x)&(tpx >= stp.x))|((tpx >= enp.x)&(tpx <= stp.x)))
	{
		if(((tpy <= enp.y)&(tpy >= stp.y))|((tpy >= enp.y)&(tpy <= stp.y)))
		{
			point res;
			res.x = tpx;
			res.y = tpy;
			return res;
		}
	}

	double stl = (detp.x - stp.x)*(detp.x - stp.x) + (detp.y-stp.y)*(detp.y-stp.y);
	double enl = (detp.x - enp.x)*(detp.x - enp.x)+(detp.y-enp.y)*(detp.y-enp.y);
	if(stl>enl)
		return enp;
	else
		return stp;
}

bool isinlim(point detp, point* trainlim, int len)
{
	int i = 0;
	while(true)
	{
		if(detp.y<trainlim[i].y)
			break;
		i++;
		if(i >= len)
			break;
	}
	i--;
	if(trainlim[i+1].y == trainlim[i].y)
	{
		if(abs(detp.x) < trainlim[i+1].x)
		{
			return true;
		}
	}
	else
	{
		if(abs(detp.x) < (trainlim[i].x + (trainlim[i+1].x- trainlim[i].x)*(detp.y - trainlim[i].y)/(trainlim[i+1].y - trainlim[i].y)))
		{
			return true;
		}
	}
	return false;
}

double nearest(point detp, point* trainlim, int len)
{
	point *curpoint, *nexpoint, p, np;
	int id = 0;
	double deltx, delty, d;
	double tmp = 100000;
	if(detp.x >= 0)
	{

		for(int i = 0; i < len - 1; i++)
		{
			curpoint = &trainlim[i];
			nexpoint = &trainlim[i+1];
			p = vertip(*curpoint, *nexpoint, detp);
			deltx = p.x - detp.x;
			delty = p.y - detp.y;
			d = sqrt(deltx*deltx+delty*delty);
			if(d < tmp)
			{
				tmp = sqrt(deltx*deltx+delty*delty);
				id = i;
			}
		}
		np = vertip(trainlim[id], trainlim[id + 1], detp);
		return sqrt((np.x - detp.x)*(np.x-detp.x) + (np.y-detp.y)*(np.y-detp.y));
	}
	else
	{
		detp.x = -detp.x;
		for(int i = 0; i < len - 1; i++)
		{
			curpoint = &trainlim[i];
			nexpoint = &trainlim[i+1];
			p = vertip(*curpoint, *nexpoint, detp);
			deltx = p.x - detp.x;
			delty = p.y - detp.y;
			d = sqrt(deltx*deltx+delty*delty);
			if(d < tmp)
			{
				tmp = sqrt(deltx*deltx+delty*delty);
				id = i;
			}
		}
		np = vertip(trainlim[id], trainlim[id + 1], detp);
		return sqrt((np.x - detp.x)*(np.x-detp.x) + (np.y-detp.y)*(np.y-detp.y));
	}
}

double framdet(double dx, double dy)
{	
	point detp = point(dy, dx);
	if(isinlim(detp, trainlim, sizeof(trainlim)/sizeof(point)))
		return 0.0;

	else if(isinlim(detp, firstlim, sizeof(firstlim)/sizeof(point)))
		return nearest(detp, trainlim, sizeof(trainlim)/sizeof(point));


	else if(isinlim(detp, secondlim, sizeof(secondlim)/sizeof(point)))
		return nearest(detp, firstlim, sizeof(firstlim)/sizeof(point));


	else if(isinlim(detp, buildinglim, sizeof(buildinglim)/sizeof(point)))
		return nearest(detp, secondlim, sizeof(secondlim)/sizeof(point));


	else
		return nearest(detp, buildinglim, sizeof(buildinglim)/sizeof(point));



}