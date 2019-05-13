#pragma once
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <exception>
#include "CMyVektor.h"

class CMyMatrix
{
private:
	std::vector<std::vector<double>> matrix;
	unsigned int dim_x;
	unsigned int dim_y;

public:

	CMyMatrix(unsigned int x, unsigned int y)
	{
		matrix.resize(x, std::vector<double>(y, 0.));
		dim_x = x;
		dim_y = y;
	}
	CMyMatrix()
	{
		matrix.resize(0, std::vector<double>(1, 0.));
		dim_x = 0;
		dim_y = 0;
	}
	CMyMatrix(CMyMatrix& cpyfrom)
	{
		this->matrix = cpyfrom.matrix;
		this->dim_x = cpyfrom.dim_x;
		this->dim_y = cpyfrom.dim_y;
	}
	class MatrixRow //Hilfsklasse für Row-Wiedergabe
	{
	private:
		std::vector<double>& row;

	public:
		MatrixRow(std::vector<double>& r) : row(r) {}
		double& operator[](unsigned int y)
		{
			return row.at(y);
		}
	};
	MatrixRow operator[](unsigned int x)
	{
		return MatrixRow(matrix.at(x));
	}
	//Setters
	void setValue(unsigned int x, unsigned int y, double val)
	{
		if (x < dim_x && y < dim_y)
		{
			matrix[x][y] = val;
		}
		else
		{
			std::cout << "Matrix-Dimensionsfehler.\n";
		}
	}

	//Getters
	unsigned int getDimX() const { return dim_x; }
	unsigned int getDimY() const { return dim_y; }
	double getValue(unsigned int x, unsigned int y)
	{
		if (x < dim_x && y < dim_y)
		{
			return matrix[x][y];
		}
		else
		{
			std::cout << "Matrix-Dimensionsfehler.\n";
		}
	}

	//Inversion (nur 2x2!)
	CMyMatrix invers()
	{
		if (dim_x != 2 || dim_y != 2)
			throw std::exception("Nur 2x2-Matrizen unterstuetzt!\n");
		double det = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
		if (det == 0)
			throw std::exception("Determinante ist Null.\n");
		CMyMatrix inv(2, 2);
		inv[0][0] = 1 / det * matrix[1][1];
		inv[1][0] = -1 / det * matrix[1][0];
		inv[0][1] = -1 / det * matrix[0][1];
		inv[1][1] = 1 / det * matrix[0][0];
		return inv;
	}

	void print()
	{
		for (unsigned int i = 0; i < getDimY(); ++i)
		{
			std::cout << "[ ";
			for (unsigned int j = 0; j < getDimX(); ++j)
			{
				std::cout << matrix[j][i] << " ";
			}
			std::cout << "]\n";
		}
	}
};

std::ostream& operator<<(std::ostream& stream, CMyMatrix& m)
{
	for (unsigned int i = 0; i < m.getDimY(); ++i)
	{
		std::cout << "               [";
		for (unsigned int j = 0; j < m.getDimX(); ++j)
		{
			std::cout << " " << m[j][i];
			if(j+1 != m.getDimX())
				 std::cout << ",";
		}
		std::cout << " ]\n";
	}
	return stream;
}

CMyVektor operator*(CMyMatrix A, CMyVektor x)
{
	int xdim = x.getDimension();
	int Adimx = A.getDimX();
	int Adimy = A.getDimY();
	if (xdim != Adimx)
		throw std::exception("Matrix/Vektor-Dimensionsfehler.\n");
	CMyVektor result(xdim);
	for (int i = 0; i < Adimy; ++i)
	{
		for (int j = 0; j < Adimx; ++j)
		{
			result[i] += A[j][i] * x[j];
		}
	}
	return result;
}

CMyMatrix jacobi(CMyVektor x, CMyVektor(*funktion)(CMyVektor x))
{
	CMyVektor f = funktion(x);
	unsigned int fdim = f.getDimension();
	unsigned int xdim = x.getDimension();
	CMyMatrix jac(xdim,fdim);
	double h = 1e-4;
	CMyVektor incV(x);
	for (int i = 0; i < xdim; ++i)
	{
		incV[i] += h;
		CMyVektor df = (funktion(incV) - f) / h; // Berechne grad spaltenweise
		incV[i] = x[i];
		for (int j = 0; j < fdim; ++j) // fülle JacobiMatrix
		{
			jac[i][j] = df[j];
		}
	}
	return jac;
}
//Newton nur für 2x2 wegen Inversen-Ausnutzung!
void newton(CMyVektor xx, CMyVektor(*funktion)(CMyVektor xx))
{
	//x1 = x0 + dx; dx = inv(dfx0)*(-fx0)
	CMyVektor x = xx;
	CMyVektor f = funktion(x);
	double limit = 1e-5;
	double length = 0;
	CMyMatrix df(2, 2);
	for (int i = 0; i < 50; ++i)
	{
		std::cout.precision(2);
		std::cout << std::scientific << "Schritt " << i << ":\n";
		std::cout << "       x     = " << x << std::endl;
		std::cout << "     f(x)    = " << f << std::endl;
		df = jacobi(x,*funktion);
		std::cout << "    f'(x)    = \n" << df;
		df = df.invers();
		std::cout << "    f'(x)^-1 = \n" << df;
		CMyVektor dx = df * f * -1;
		std::cout << "      dx     = " << dx << std::endl;
		length = f.getLaenge();
		std::cout << "    ||f(x)|| = " << length << "\n\n";
		x = x + dx;
		f = funktion(x);
		if (length < limit)
		{
			std::cout << "Ende wegen ||f(x)||<1e-5 bei\n";
			break;
		}
	}
	std::cout << "       x     = " << x << std::endl;
	std::cout << "     f(x)    = " << f << std::endl;
	std::cout << "    ||f(x)|| = " << length << "\n\n";
}
