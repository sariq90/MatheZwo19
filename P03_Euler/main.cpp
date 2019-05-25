#include <iostream>
#include "CMyVektor.h"
#include "C_DGLSolver.h"


CMyVektor f1(CMyVektor y, double x)
{
	CMyVektor DGL_Sys = CMyVektor(2);
	DGL_Sys[0] = 2 * y[1] - x * y[0];
	DGL_Sys[1] = y[0] * y[1] - 2 * x*x*x;
	return DGL_Sys;
}

double f2(CMyVektor y, double x)
{
	return 2 * x*y[1] * y[2] + 2 * y[0] * y[0] * y[1];
}

int main()
{
	CMyVektor startSys = { 0,1 };
	C_DGLSolver solveSys = C_DGLSolver(*f1);
	solveSys.heun(0, 2, 100, startSys, false);
	
	CMyVektor startDGL3 = { 1,-1,2 };
	C_DGLSolver solveDGL3 = C_DGLSolver(*f2);
	//solveDGL3.euler(1, 2, 100, startDGL3, false);

	CMyVektor deviation = CMyVektor(8);
	double solution = 0.5;
	for (int i = 0, j = 10; i < 4; ++i, j = 10 * j)
	{
		deviation[i] = solution - solveDGL3.euler(1, 2, j, startDGL3, true);
	}
	for (int i = 4, j = 10; i < 8; ++i, j = 10 * j)
	{
		deviation[i] = solution - solveDGL3.heun(1, 2, j, startDGL3, true);
	}
	std::cout << std::endl;
	for (int i = 0, j = 10; i < 4; ++i, j = 10 * j)
	{
		std::cout << "Abweichung bei Euler bei " << j << " Schritten: " << deviation[i] << std::endl;
		std::cout << "Abweichung bei Heun bei " << j << " Schritten: " << deviation[i + 4] << std::endl;
	}

	std::system("pause");
	return 0;
}