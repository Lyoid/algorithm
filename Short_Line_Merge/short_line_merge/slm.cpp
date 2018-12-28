#include<iostream>
#include<vector>
#include<math.h>
#include"struct.h"
#include"slm.h"
#ifndef _PARAM
#define RADIUS 1  //����뾶
#define SPACING 500//�Ƕȷ���,Ϊ����
#define SORTING 5 //ѡ��ǰ��������
#define DISTANCE 5 //��ֱ�߼�������
#define LENGTH 10  //���ڳ����򲻺ϲ�
#define PTOLERATE 0.00000005 //�Ƕ����̶ȣ�����Ϊƽ����
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
��֪һ���߶εĳ��ȡ��е�ͽǶȣ������˵�

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
��һ�����е����ֵ

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
ð������

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
�Խ��ߺϲ�
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
								/* ƽ���ı��ζԽ��߽��㼰���ȼ���*/
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
��ֱ�߲����м���
��ѡ�����߶α��Ϊѡ��״̬
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
		������һ��ֱ�ߵ�ĳһ���˵�ΪԲ�ģ������ڰ뾶���ڵ��߶�
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
		�Ƕ�ͳ����ֱ��ͼͶƱ

ע�⣺p���������ǵ��߶νǶȣ�p*180 = L ,����L Ϊ��ֵ
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
	/* ����ͳ��ֱ��ͼ */
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
	/* ����ṹ���в�����ǶȾ�ֵ*/
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

	/* �Ƚϳ��� */
	int * sort = maopao(length,SPACING);

	delete[]length;
	length = NULL;

	/* ѡ������ǰ SORTING �������*/
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

		/* �Ƕ�ͳ����ֱ��ͼͶƱ */
		hist = angel_histogram_vote(radius_Line, tempLength);
		
		/* ��ֱ�߲����м��� */
		select_long_line(hist);

		if (*tempLength == 1)
		{
			continue;
		}
		/* �Խ��ߺϲ� */
		diagonal_merge(hist, newLine);

		delete hist;
		delete []radius_Line;
		radius_Line = NULL;
		hist = NULL;
	}

	vector<struct argument *> temp1;
	/* ����ԭʼ�����б�ѡ�е��߶�*/
	for (i = 0; i < args->count; i++)
	{
		if (args->arg[i].is_selected)
		{
			temp1.push_back(&args->arg[i]);
		}
	}
	/* �ϲ����߶� */
	for (i = 0; i <(int)(*newLine).size(); i++)
	{
			temp1.push_back(newLine[0][i]);
	}
	/* ����newLine�е����ݣ�ʹ���ַ���� */
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
