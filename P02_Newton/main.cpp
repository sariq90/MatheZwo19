#include <iostream>
#include "CMyMatrix.h"
using namespace std;

CMyVektor f1(const CMyVektor x) //R4->R3
{
	CMyVektor result = { 0,0,0 };
	result[0] = x[0] * x[1] * exp(x[2]);
	result[1] = x[1] * x[2] * x[3];
	result[2] = x[3];
	return result;
}

CMyVektor f2(const CMyVektor x) //R2->R2
{
	CMyVektor result = { 0,0 };
	result[0] = x[0] * x[0] * x[0] * x[1] * x[1] * x[1] - 2 * x[1];
	result[1] = x[0] - 2;
	return result;
}

int main()
{
	//CMyVektor x = { 1,2,0,3 };
	//CMyMatrix m(4,3);
	//m = jacobi(x, f1);
	//m.print();

	CMyVektor y = { 1,1 };
	newton(y, f2);
	
		
	system("pause");
	return 0;
}