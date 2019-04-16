#include <iostream>
#include "CMyVektor.h"


double f(const CMyVektor x) //n=2
{
	return sin(x[0] * x[1]) + sin(x[0]) + cos(x[1]);
}

double g(const CMyVektor x) //n=3
{
	return -(2 * x[0] * x[0] - 2 * x[0] * x[1] + x[1] * x[1] + x[2] * x[2] - 2 * x[0] - 4 * x[2]);
}

int main()
{
	//maximize({0.2,-2.1},f);
	maximize({ 0,0,0 }, g, 0.1);


	std::system("pause");
	return 0;
}