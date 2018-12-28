
#ifndef STRUCT_H_
#define STRUCT_H_
#include<vector>
/*----------------------------------------------------------------------------*/
/** Structure to store one point definition and read value.
*/
struct point
{
	double x;
	double y;
};
/*----------------------------------------------------------------------------*/
/** Structure to store one argument definition and read value.
*/
struct argument
{
	int id;
	bool is_focus = false;
	bool is_selected = false;
	struct point * p1;
	struct point * p2;
	double width = 1.5;
	double length;
	double p;
	double log_NFA;
};


/*----------------------------------------------------------------------------*/
/** Structure to store the full set of argument definitions and its values.
*/
struct arguments
{
	int count;
	double xsize;
	double ysize;
	struct argument * arg;
};

/*----------------------------------------------------------------------------*/
/** Structure to store the Histogram value.
*/
struct histogram
{
	int id;
	int count;
	double length;
	double ptotal;
	double paverage;
	struct argument  * arg;
};
#endif // !STRUCT_H_



