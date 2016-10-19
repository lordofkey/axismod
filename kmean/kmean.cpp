#define DLL

#include "kmean.h"


bool kmean(int *x, int *y, int num, int lastnum)
{
	int *index = new int[num];
	memset(index, 0, num*sizeof(int));
	int currentindex = 1;

	int *nsp = new int[num];
	int *osp = new int[num];
	int numnsp;
	int numosp;
	while(true)
	{
		int i = 0;
		while(true)//查找index为0的点
		{
			if(index[i] == 0)
				break;
			i++;
			if(i == num)
				goto label_1;
		}
		index[i] = currentindex;
		nsp[0] = i;
		numnsp = 1;
		while(true)
		{
			int* tmp = osp;
			osp = nsp;
			numosp = numnsp;

			nsp = tmp;
			numnsp = 0;
			for(int j = 0; j < numosp; j++)//遍历当前层点
			{
				
				for(int k = 0; k<num;k++)
				{
					if(index[k] == 0)
					{
						float d = sqrt((float)(x[k] - x[osp[j]])*(x[k] - x[osp[j]]) + (y[k] - y[osp[j]])*(y[k] - y[osp[j]]));
						if(d<100)
						{
							nsp[numnsp] = k;
							index[k] = currentindex;
							numnsp++;
						}
					}
				}
			}
			if(numnsp == 0)
				break;
		}
		currentindex++;
	}
	delete nsp;
	delete osp;


label_1:

	for(int i = 1; i<currentindex;i++)
	{
		float sumx = 0,sumy = 0,sumx2 = 0,sumy2 = 0, ncur = 0, nxj = 0, nyj=0;
		for(int j = 0; j < num; j++)
		{
			if(index[j] == i)
			{
				if(abs(x[i])>400&&abs(x[i])<600)
					nxj++;
				if(y[i]>3200&&y[i]<3600)
					nyj++;
				sumy += y[j];
				sumy2 += y[j]*y[j];
				sumx2 += x[j] * x[j];
				ncur++;
			}
		}

		if ((abs(sumx/ncur)<100)&&(abs(sumy/ncur-900)<200)&&((sumx2/ncur)<50000)&&((sumy2/ncur-(sumy/ncur)*(sumy/ncur))>2000))
			return true;
		if(lastnum>200)
			if(nxj>25&&nyj>10)
				return true;
	}
	return false;
}