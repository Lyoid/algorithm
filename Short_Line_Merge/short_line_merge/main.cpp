
#include<iostream>
#include<fstream>  
#include <math.h>
#include <stdio.h>
#include <string>
#include <ctype.h>


#include"slm.h"
#include"lsd.h"
#include"struct.h"
#include "stdafx.h"

using namespace std;


/*----------------------------------------------------------------------------*/
/** Read a txt file into an double array.
If the name is "-" the file is read from standard input.
*/
static double * read_txt_double(const char * path,int * length) 
{
	int i = 0;
	int j = 0;
	ifstream f;
	char * s = new char;
	double * result = new double[800000];
	if (NULL == s) {
		void();
	}

	char* num = new char[100];
	if (NULL == num) {
		void();
	}
	/*open txt file*/
	f.open(path,ios::in|ios::binary);
	if (!f.is_open()) {
		void();
	}
	/* read txt file and copy into param*/
	while (!f.eof())
	{
		*s = f.get();
		if (*s == '\n') {
			continue;
		}
		if (isspace(*s)) {
			result[i] = atof(num);
//			cout << arr[i]<<endl;
			*num = NULL;
			j = 0;
			i++;
			continue;
		}
		num[j] = *s;
		j = j + 1;
	//	cout.precision(10);
	//	cout << atof(num)<<endl;
	}
	f.close();
	*length = i;
	delete s;
	s = NULL;
	delete []num;
	num = NULL;
	return result;
}

/*----------------------------------------------------------------------------*/
/** 
	put an double array into the customized struct.
*/
static struct arguments * read_line_struct(double * line,int * length)
{
	int i = 0;
	int j = 0;
	double * xsize = new double;
	double * ysize = new double;
	struct point * pt1 = new point[*length / 7];
	struct point * pt2 = new point[*length / 7];
	struct argument * arg = new argument[*length / 7];
	struct arguments * args = new arguments;
	j = 0;
	for (i = 0; i < *length; i++) 
	{
		pt1[j].x = line[i];
		pt1[j].y = line[++i];
		pt2[j].x = line[++i];
		pt2[j].y = line[++i];
		if (pt1[j].x > * xsize) {
			* xsize = pt1[j].x;
		}
		if (pt1[j].y > * ysize) {
			* ysize = pt1[j].y;
		}
		arg[j].length = sqrt((pt1[j].x - pt2[j].x)*(pt1[j].x - pt2[j].x) + (pt1[j].y - pt2[j].y)*(pt1[j].y - pt2[j].y));
		arg[j].id = j + 1;
		arg[j].is_focus = false;
		arg[j].p1 = &pt1[j];
		arg[j].p2 = &pt2[j];
		arg[j].width = line[++i];
		arg[j].p = line[++i];
		arg[j].log_NFA = line[++i];
		j = j + 1;
	}
	args->xsize = *xsize;
	args->ysize = *ysize;
	args->count = *length / 7;
	args->arg = arg;
	delete xsize;
	xsize = NULL;
	delete ysize;
	ysize = NULL;
	pt1 = NULL;
	pt2 = NULL;
	return args;
}

/*----------------------------------------------------------------------------*/
/**
	output the result line into a .eps picture
*/
static void write_eps(const char * path,struct arguments * args)
{
//	ofstream eps("D:\\FILE\\test\\short_line_merge\\short_line_merge\\test.txt", ios::out);
	ofstream eps(path, ios::out|ios::binary|ios::trunc);
	int i;
	double xsize = args->xsize;
	double ysize = args->ysize;
//	const char * filename = "test.eps";
	/* check input */
	if (args == NULL) {
		void;
	}

	/* open file */
//	eps.open("D:\\FILE\\test\\short_line_merge\\short_line_merge\\test.txt", ios::out);
	if (!eps.good()) {
		cout << "error";
	}
	/* write EPS header */
	eps.precision(10);
	eps << "%%!PS-Adobe-3.0 EPSF-3.0\n";
	eps << "%%BoundingBox: 0 0 " << xsize << " " << ysize;
	eps << "%%Author: Lyoid An\n";
	eps << "%%Title: (" << "test.eps" << ")\n";
	eps << "%%EndComments\n";


	/* write line segments */
	for (i = 0; i< args->count; i++)
	{
		eps << "newpath " << args->arg[i].p1->x << " " << (double)ysize - args->arg[i].p1->y << " moveto " << args->arg[i].p2->x << " " << (double)ysize - args->arg[i].p2->y << " lineto " << 1.500000 << " setlinewidth stroke\n";
	}

	/* close EPS file */
	eps << "showpage\n";
	eps << "%%EOF\n";
	eps.close();

}

/*----------------------------------------------------------------------------*/
/**
			Main Function
*/
int main() 
{
	int i;
	int * arrlength = new int;
	*arrlength = 0;
	double * line;
	struct arguments * args;
	struct arguments * result;
	/* 读取txt文件 */
	line = read_txt_double("C:\\Users\\Lyoid\\Desktop\\experiment\\Mytest.txt",arrlength);
	/* 保存数据到自定义结构体中 */
	args = read_line_struct(line,arrlength);
	/* 短直线剔除算法实现 */
	result = ShortLineMerge(args);


	/* 结果以eps格式输出 */
	write_eps("C:\\Users\\Lyoid\\Desktop\\experiment\\Mytest3.eps", result);
	/* delete 无用指针 */
	delete arrlength;
	arrlength = NULL;
	delete[]line;
	line = NULL;
	delete args->arg->p1;
	args->arg->p1 = NULL;
	delete args->arg->p2;
	args->arg->p2 = NULL;
	delete []args->arg;
	args->arg = NULL;
	delete args;
	args = NULL;

	delete[]result->arg;
	result->arg = NULL;
	delete result;
	result = NULL;



	return 0;
}