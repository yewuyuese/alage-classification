/*
 * Function: float norm2distance
 * ����ԭ�� float norm2distance(uchar* point1, uchar* point2, int dimension)
 * �����꣺(x1,x2,x3,...,),(y1,y2,y3,...)
 * ����ά��Ϊdimension
 */
#include<math.h>

#ifndef CALCULATE_H
#define CALCULATE_H

float norm2distance(uchar* point1,uchar* point2,int dimension)
{
	double distance;
	double sum = 0;
	for(int i = 0;i < dimension;i++)
		sum = sum + pow((point1[i] - point2[i]),2);
	distance = sqrt(sum);
	return distance;
}

#endif
