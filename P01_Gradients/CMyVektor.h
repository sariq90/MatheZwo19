#pragma once
#include <vector>
#include <iostream>
#include <cmath>

class CMyVektor
{
private:
	std::vector<double> vektor;

public:
	//friend CMyVektor operator+(const CMyVektor& a, const CMyVektor& b);
	friend std::ostream& operator<<(std::ostream& stream, const CMyVektor& myVec);
	friend CMyVektor gradient(CMyVektor x, double(*funktion)(CMyVektor x));

	//Constructors
	CMyVektor(const std::initializer_list<double> ilist)
	{
		vektor = { ilist };
	}
	CMyVektor(const std::vector<double> vec)
	{
		vektor = vec;
	}
	CMyVektor(const double* arr, int len)
	{
		vektor.assign(arr, arr + len);
	}
	CMyVektor(const CMyVektor& cpyfrom)
	{
		this->vektor = cpyfrom.vektor;
	}
	//Setters
	void setVektor(const std::initializer_list<double> ilist)
	{
		vektor = { ilist };
	}
	void setVektor(const std::vector<double> vec)
	{
		vektor = vec;
	}
	void setVektor(const double* arr, int len)
	{
		vektor.assign(arr, arr + len);
	}
	void setVektor(const CMyVektor& cpyfrom)
	{
		this->vektor = cpyfrom.vektor;
	}
	void setKomponente(const int pos, const double d)
	{
		try
		{
			vektor.at(pos) = d;
		}
		catch (const std::out_of_range&)
		{
			std::cout << "Index out of range." << std::endl;
		}
	}
	//Getters
	int getDimension() const
	{
		return vektor.size();
	}
	double getKomponente(int pos) const
	{
		try
		{
			return vektor.at(pos);
		}
		catch (const std::out_of_range&)
		{
			std::cout << "Index out of range." << std::endl;
		}
	}
	double getLaenge()
	{
		double sum = 0;
		for (int i = 0; i < vektor.size(); ++i)
		{
			sum += vektor.at(i)*vektor.at(i);
		}
		return sqrt(sum);
	}
	//Operators
	double& operator[](int index) //by ref noconst
	{
		try
		{
			return vektor.at(index);
		}
		catch (const std::out_of_range&)
		{
			std::cout << "Index out of range." << std::endl;
		}
	}
	double operator[](int index) const //by val const
	{
		try
		{
			return vektor.at(index);
		}
		catch (const std::out_of_range&)
		{
			std::cout << "Index out of range." << std::endl;
		}
	}
};
CMyVektor operator+(const CMyVektor& a, const CMyVektor& b)
{
	if (a.getDimension() != b.getDimension())
		throw std::length_error("Dimensions do not fit.");
	CMyVektor rvec = a;
	for (int i = 0; i < rvec.getDimension(); ++i)
	{
		rvec[i] += b[i];
	}
	return rvec;
}
CMyVektor operator*(double lambda, const CMyVektor& a)
{
	CMyVektor rvec = a;
	for (int i = 0; i < rvec.getDimension(); ++i)
	{
		rvec.setKomponente(i, lambda*rvec.getKomponente(i));
	}
	return rvec;
}

std::ostream& operator<<(std::ostream& stream, const CMyVektor& myVec)
{
	std::cout << "[ ";
	for (auto i = myVec.vektor.begin(); i != myVec.vektor.end(); ++i)
	{
		std::cout << *i << " ";
	}
	std::cout << "]";
	return stream;
}
//Gradients
CMyVektor gradient(CMyVektor x, double(*funktion)(CMyVektor x))
{
	double h = 1e-8; // h = 10^-8
	double festTerm = funktion(x); //1x ausrechnen reicht pro Anwendung
	CMyVektor gradV(x); //just init
	CMyVektor incV(x);
	for (int i = 0; i < x.getDimension(); ++i)
	{	//für jedes gi: Addiere h nur auf i-te Komponente, berechne grad
		incV[i] += h;
		gradV[i] = (funktion(incV) - festTerm) / h;
		incV[i] = x[i];
	}
	return gradV;
}
void maximize(CMyVektor x, double(*funktion)(CMyVektor x), double lambda = 1.)
{
	CMyVektor xx(x);
	double limit = 1e-5; // Abbruchgrenze
	for (int i = 0; i < 25; ++i)
	{	//Init
		CMyVektor xn = xx + lambda * gradient(xx, funktion);
		std::cout << "_____________________________________\n";
		std::cout << "Schritt " << i  << " --- " << "Lambda: " << lambda << std::endl;
		std::cout << "  x  : " << xx << std::endl;
		std::cout << "f(x) : " << funktion(xx) << std::endl;
		std::cout << "  xn : " << xn << std::endl;
		std::cout << "f(xn): " << funktion(xn) << std::endl;
		if (funktion(xn) > funktion(xx))
		{	//Teste auf doppelte Schrittweite
			CMyVektor xt = xx + 2 * lambda * gradient(xx, funktion);
			std::cout << "Teste 2x Schrittweite (Lambda = " << 2 * lambda << "):\n";
			std::cout << "  xt : " << xt << std::endl;
			std::cout << "f(xt): " << funktion(xt) << std::endl;
			if (funktion(xt) > funktion(xn))
			{	//Setze xt als neues xx und verdopple lambda
				xx = xt;
				lambda *= 2;
				std::cout << "Schrittweite verdoppelt!\n";
			}
			else
			{	//Setze xn als neues xx und behalte lambda
				xx = xn;
			}
		}
		else
		{	//Halbiere Schrittweite solange, bis xn > xx
			do 
			{
				lambda /= 2;
				std::cout << "Schrittweite halbiert!\n";
				xn = xx + lambda * gradient(xx, funktion);
				std::cout << "  xn : " << xn << std::endl;
				std::cout << "f(xn): " << funktion(xn) << std::endl;
			} while (funktion(xn) <= funktion(xx));
			xx = xn;
		}
		std::cout << "||grad f(x)|| = ";
		std::cout << gradient(xx, funktion).getLaenge() << std::endl;
		if (gradient(xx, funktion).getLaenge() < limit)
		{
			std::cout << "Limit erreicht, breche ab.\n";
			break;
		}
	}
	std::cout << "Endwerte:\n";
	std::cout << "  x  : " << xx << std::endl;
	std::cout << "f(x) : " << funktion(xx) << std::endl;
}