#include<iostream>
#include<vector>
#include<math.h>
#include"struct.h"
#include"slm.h"
#ifndef _PARAM
#define RADIUS 1  //辐射半径
#define SPACING 500//角度分区,为整数
#define SORTING 5 //选择前几个分区
#define DISTANCE 5 //两直线间最大距离
#define LENGTH 10  //大于长度则不合并
#define PTOLERATE 0.00000005 //角度容忍度，此项为平方项
#endif 
#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif


using namespace std;

/*----------------------------------------------------------------------------*/
/**
已知一条线段的长度、中点和角度，求两端点

*/
struct argument * get_Endpoint(double length, double p, struct point * midPoint)
{
	double dx;
	double dy;
	struct point * pt1 = new point;
	struct point * pt2 = new point;
	struct argument * newLine = new argument;
	dx = sqrt((length / 2)*(cos(p * 180))*(length / 2)*(cos(p * 180)));
	dy = (length / 2)*(sin(p * 180));
	pt1->x = midPoint->x + dx;
	pt1->y = midPoint->y + dy;
	pt2->x = midPoint->x - dx;
	pt2->y = midPoint->y - dy;
	newLine->p = p;
	newLine->is_selected = true;
	newLine->is_focus = true;
	newLine->length = length;
	newLine->id = 9999;
	newLine->log_NFA = 0;
	newLine->width = 1.5;
	newLine->p1 = pt1;
	newLine->p2 = pt2;
	pt1 = NULL;
	pt2 = NULL;
	return newLine;
}


/*----------------------------------------------------------------------------*/
/**
求一组数中的最大值

*/

double max(double a[], int length)
{
	double temp;
	temp = a[0];
	for (int i = 0; i < length; i++)
	{
		if (a[i] > temp)
		{
			temp = a[i];
		}
	}
	return temp;
}



/*----------------------------------------------------------------------------*/
/**
冒泡排序法

*/

int * maopao(double length[], int space)
{
	int * resultId = new int[space];
	double temp;
	int idtp;
	for (int k = 0; k < space; k++)
	{
		resultId[k] = k + 1;
	}
	for (int j = 1; j < space; j++)
	{
		for (int i = 0; i < space - j; i++)
		{
			if (length[i] < length[i + 1])
			{
				temp = length[i];
				length[i] = length[i + 1];
				length[i + 1] = temp;
				idtp = resultId[i];
				resultId[i] = resultId[i + 1];
				resultId[i + 1] = idtp;
			}
		}
	}
	return resultId;
}


/*----------------------------------------------------------------------------*/
/**
对角线合并
*/
void diagonal_merge(struct histogram * hist, vector<struct argument * > * newLine)
{
	int i;
	int j;
	int k;
	double length1;
	double length2;
	double length3;
	double length4;
	double  p1x;
	double  p1y;
	double  p2x;
	double  p2y;
	double  p3x;
	double  p3y;
	double  p4x;
	double  p4y;


	for (i = 0; i < SORTING; i++)
	{
		for (j = 0; j < hist[i].count; j++)
		{
			if (!(hist[i].arg[j].is_selected))
			{
				for (k = 0; k < hist[i].count; k++)
				{
					if (!(hist[i].arg[k].is_selected))
					{
						if (j == k)
						{
							break;
						}
						p1x = hist[i].arg[j].p1->x;
						p1y = hist[i].arg[j].p1->y;
						p2x = hist[i].arg[j].p2->x;
						p2y = hist[i].arg[j].p2->y;
						p3x = hist[i].arg[k].p1->x;
						p3y = hist[i].arg[k].p1->y;
						p4x = hist[i].arg[k].p2->x;
						p4y = hist[i].arg[k].p2->y;
						length1 = sqrt((((p1x) - (p3x))*((p1x) - (p3x)) + ((p1y) - (p3y))*((p1y) - (p3y))));
						length2 = sqrt((((p1x) - (p4x))*((p1x) - (p4x)) + ((p1y) - (p4y))*((p1y) - (p4y))));
						length3 = sqrt((((p2x) - (p3x))*((p2x) - (p3x)) + ((p2y) - (p3y))*((p2y) - (p3y))));
						length4 = sqrt((((p2x) - (p4x))*((p2x) - (p4x)) + ((p2y) - (p4y))*((p2y) - (p4y))));
						if (length1 < DISTANCE || length2 < DISTANCE || length3 < DISTANCE || length4 < DISTANCE)
						{
							if ((hist[i].arg[j].p - hist[i].arg[k].p)*(hist[i].arg[j].p - hist[i].arg[k].p) < PTOLERATE)
							{
								/* 平行四边形对角线交点及长度计算*/
								struct argument * newarg;
								double a[4] = { length1, length2, length3, length4 };
								double maxL = max(a, 4);
								struct point * midPoint = new point;
								if (maxL == length1)
								{
									midPoint->x = ((p1x) + (p3x)) / 2;
									midPoint->y = ((p1y) + (p3y)) / 2;
								}
								if (maxL == length2)
								{
									midPoint->x = ((p1x) + (p4x)) / 2;
									midPoint->y = ((p1y) + (p4y)) / 2;
								}
								if (maxL == length3)
								{
									midPoint->x = ((p2x) + (p3x)) / 2;
									midPoint->y = ((p2y) + (p3y)) / 2;
								}
								if (maxL == length4)
								{
									midPoint->x = ((p2x) + (p4x)) / 2;
									midPoint->y = ((p2y) + (p4y)) / 2;
								}
								newarg = get_Endpoint(maxL, hist[i].paverage, midPoint);
								newLine->push_back(newarg);
								delete midPoint;
								midPoint = NULL;
								newarg = NULL;

							}
						}
					}
				}
			}
		}
	}

}


/*----------------------------------------------------------------------------*/
/**
长直线不进行计算
把选出的线段标记为选中状态
*/
void select_long_line(struct histogram *args)
{
	int i;

	for (i = 0; i < args->count; i++)
	{
		if (args->arg[i].length > LENGTH)
		{
			args->arg[i].is_selected = true;
		}

	}
}


/*----------------------------------------------------------------------------*/
/**
		遍历以一条直线的某一个端点为圆心，所有在半径以内的线段
*/
struct argument ** catch_Radius_Line(struct arguments * args,struct argument * arg,int * length)
{
	int i;
	int k = 0;
	struct argument ** arr = new argument*[args->count];
	for (i = 0; i < args->count; i++)
	{
		if ((args->arg[i].is_selected == false) && (args->arg[i].is_focus == false) && sqrt((((args->arg[i].p1->x - arg->p1->x)*(args->arg[i].p1->x - arg->p1->x) + (args->arg[i].p1->y - arg->p1->y)*(args->arg[i].p1->y - arg->p1->y))) < RADIUS) )
		{
			arr[k] = &args->arg[i];
			args->arg[i].is_focus = true;
			k = k + 1;
		}
	}
	 * length = k;
	return arr;
}


/*----------------------------------------------------------------------------*/
/**
		角度统计与直方图投票

注意：p不代表真是的线段角度，p*180 = L ,其中L 为真值
*/
struct histogram * angel_histogram_vote(struct argument ** arg,int * arrlength)
{
	int i;
	int j;
	double * p;
	double N;
	double * length = new double[SPACING]();
	double * ptotal = new double[SPACING]();
	int * count = new int[SPACING]();
	struct histogram * hist = new histogram[SPACING];
	vector<struct argument *> * vec = new vector<struct argument *>[SPACING];
	N = (double)1 / (double)SPACING;
	
	if (!vec->empty())
	{
		vec->clear();
	}
	/* 构建统计直方图 */
	for (i = 0; i < (* arrlength); i++)
	{
		p = &arg[i]->p;
		for (j = 1; j <= SPACING; j++)
		{
			if (*p < (N * j))
			{
				vec[j - 1].push_back(arg[i]);
				length[j - 1] += arg[i]->length;
				ptotal[j - 1] += * p;
				count[j - 1] += 1;
				break;
			}
		}
	}
	/* 载入结构体中并计算角度均值*/
	for (i = 0; i < SPACING; i++)
	{
		hist[i].id = i + 1;
		hist[i].count = count[i];
		hist[i].length = length[i];
		hist[i].ptotal = ptotal[i];
		hist[i].paverage = ptotal[i] / count[i];
		if (!vec[i].empty())
		{
			hist[i].arg = vec[i][0];
		}
		else {
			hist[i].arg = NULL;
		}
		
	}
	delete[]count;
	count = NULL;
	delete[]ptotal;
	ptotal = NULL;
	p = NULL;

	/* 比较长度 */
	int * sort = maopao(length,SPACING);

	delete[]length;
	length = NULL;

	/* 选出排名前 SORTING 的区间段*/
	struct histogram * temp = new histogram;
	for (i = 0; i < SORTING; i++)
	{
		for (j = 0; j < SPACING; j++)
		{
			if (sort[i] == hist[j].id)
			{
				if (i == j) continue;
				memcpy(temp, &hist[j], sizeof(hist[j]));
				memcpy(&hist[j], &hist[i], sizeof(hist[i]));
				memcpy(&hist[i], temp, sizeof(*temp));
			}
		}
	}
	delete temp;
	temp = NULL;
	delete[]sort;
	sort = NULL;
	delete[]vec;
	vec = NULL;
	return hist;

}





struct arguments * ShortLineMerge(struct arguments * args) 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int i;
	int j;
	int * tempLength = new int;
	struct arguments * temp = new arguments;
	struct arguments * result;
	struct histogram * hist;
	struct argument ** radius_Line;
	vector<struct argument * > * newLine = new vector<struct argument * >;
	if (!(*newLine).empty())
	{
		(*newLine).clear();
	}


	for (i = 0; i < args->count; i++)
	{
		if (args->arg[i].is_focus )
		{
			continue;
		}
		radius_Line = catch_Radius_Line(args,&args->arg[i], tempLength);

		/* 角度统计与直方图投票 */
		hist = angel_histogram_vote(radius_Line, tempLength);
		
		/* 长直线不进行计算 */
		select_long_line(hist);

		if (*tempLength == 1)
		{
			continue;
		}
		/* 对角线合并 */
		diagonal_merge(hist, newLine);

		delete hist;
		delete []radius_Line;
		radius_Line = NULL;
		hist = NULL;
	}

	vector<struct argument *> temp1;
	/* 遍历原始数据中被选中的线段*/
	for (i = 0; i < args->count; i++)
	{
		if (args->arg[i].is_selected)
		{
			temp1.push_back(&args->arg[i]);
		}
	}
	/* 合并新线段 */
	for (i = 0; i <(int)(*newLine).size(); i++)
	{
			temp1.push_back(newLine[0][i]);
	}
	/* 拷贝newLine中的数据，使其地址连续 */
	struct argument * temp2 = new argument[(int)temp1.size()];
	for (i = 0; i < (int)temp1.size(); i++)
	{
		memcpy(&temp2[i], temp1[i], sizeof(struct argument));
	}
	temp->arg = temp2;
	temp->count = (int)temp1.size();
	temp->xsize = args->xsize;
	temp->ysize = args->ysize;
	temp2 = NULL;
	radius_Line = NULL;
	j = (int)(*newLine).size();

	delete newLine;
	newLine = NULL;
	delete tempLength;
	tempLength = NULL;
	temp1.clear();
	vector<struct argument *>(temp1).swap(temp1);

	return temp;
}
