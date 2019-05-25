#pragma once
#include "CMyVektor.h"

class C_DGLSolver
{
private:
	CMyVektor (*funcSys)(CMyVektor y, double x);
	double (*funcHigh)(CMyVektor y, double x);
	bool isHigherOrder;

public:
	C_DGLSolver(CMyVektor(*f_DGL_System)(CMyVektor y, double x)) : funcSys{ f_DGL_System }, funcHigh{ nullptr }, isHigherOrder{ false }{}
	C_DGLSolver(double(*f_DGL_nterOrdnung)(CMyVektor y, double x)) : funcSys{ nullptr }, funcHigh{ f_DGL_nterOrdnung }, isHigherOrder{ true }{}

	CMyVektor ableitungen(CMyVektor y, double x)
	{
		//Auswertung y,x für Sys
		if (!isHigherOrder)
			return funcSys(y, x);
		else //Trafo für High, Ausgabe rechte Seite
		{
			int dim = y.getDimension();
			CMyVektor trafoResult = CMyVektor(dim);
			for (int i = 0; i < dim - 1; ++i)
			{
				trafoResult[i] = y[i + 1];
			}
			trafoResult[dim - 1] = funcHigh(y, x);
			return trafoResult;
		}
	}

	double euler(double start, double end, int steps, CMyVektor y_Start, bool silent)
	{
		double h = (end - start) / steps;
		double x = start;
		CMyVektor y = y_Start;
		CMyVektor yInc = CMyVektor(y_Start.getDimension());

		for (int i = 0; i < steps; ++i)
		{
			yInc = ableitungen(y, x);

			if (!silent)
			{
				std::cout << "Schritt " << i << ":" << std::endl;
				std::cout << "    x = " << x << std::endl;
				std::cout << "    y = " << y << std::endl;
				std::cout << "    y' = " << yInc << std::endl << std::endl;
			}
	
			y = y + h * yInc;
			x += h;
		}
		if (!silent)
		{
			std::cout << "Ende bei: " << std::endl;
			std::cout << "    x = " << x << std::endl;
			std::cout << "    y = " << y << std::endl;
		}
		return y[0];
	}

	double heun(double start, double end, int steps, CMyVektor y_Start, bool silent)
	{
		double h = (end - start) / steps;
		double x = start;
		CMyVektor y = y_Start;
		CMyVektor yInc = CMyVektor(y_Start.getDimension());
		CMyVektor yT = y;
		CMyVektor yIncT = yInc;
		CMyVektor yIncM = yInc;

		for (int i = 0; i < steps; ++i)
		{
			if (!silent)
			{
				std::cout << "Schritt " << i << ":" << std::endl;
				std::cout << "    x = " << x << std::endl;
				std::cout << "    y = " << y << std::endl;
			}

			yInc = ableitungen(y, x);
			yT = y + h * yInc;
			yIncT = ableitungen(yT, x + h);
			yIncM = 0.5*(yInc + yIncT);

			if (!silent)
			{
				std::cout << "    y'_orig = " << yInc << std::endl << std::endl;
				std::cout << "    y_Test = " << yT << std::endl;
				std::cout << "    y'_Test = " << yIncT << std::endl << std::endl;
				std::cout << "    y'_Mittel = " << yIncM << std::endl << std::endl;
			}

			y = y + h * yIncM;
			x += h;
		}
		if (!silent)
		{
			std::cout << "Ende bei: " << std::endl;
			std::cout << "    x = " << x << std::endl;
			std::cout << "    y = " << y << std::endl;
		}
		return y[0];
	}

};
