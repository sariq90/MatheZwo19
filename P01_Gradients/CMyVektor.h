#pragma once
#include <vector>
#include <iostream>
#include <cmath>

class CMyVektor
{
private:
	std::vector<double> vektor;

public:
	friend CMyVektor operator+(const CMyVektor& a, const CMyVektor& b);
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
	double h = 0.00000001; // h = 10^-8
	double festTerm = funktion(x);
	CMyVektor gradV(x);
	for (int i = 0; i < x.getDimension(); ++i)
	{
		CMyVektor incV(x);
		incV[i] += h;
		gradV[i] = (funktion(incV) - festTerm) / h;
	}
	return gradV;
}
void maximize(CMyVektor x, double(*funktion)(CMyVektor x), double lambda = 1.)
{
	CMyVektor xx(x);
	for (int i = 0; i < 25; ++i)
	{	//Init
		double limit = 0.00001; // Abbruchgrenze
		CMyVektor xn = xx + lambda * gradient(xx, funktion);
		std::cout << "___________________________\n";
		std::cout << "Schritt " << i + 1 << " --- " << "Lambda: " << lambda << std::endl;
		std::cout << "  xx : " << xx << std::endl;
		std::cout << "f(xx): " << funktion(xx) << std::endl;
		std::cout << "  xn : " << xn << std::endl;
		std::cout << "f(xn): " << funktion(xn) << std::endl;
		if (funktion(xn) > funktion(xx))
		{	//Teste auf doppelte Schrittweite
			CMyVektor xt = xx + 2 * lambda * gradient(xx, funktion);
			std::cout << "Teste 2x Schrittweite:\n";
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
			std::cout << "Schleichmodus aktiviert:\n";
			while (funktion(xn) <= funktion(xx) && gradient(xx, funktion).getLaenge() >= limit)
			{
				lambda /= 2;
				std::cout << "Schrittweite halbiert!\n";
				xn = xx + lambda * gradient(xx, funktion);
				std::cout << "  xn : " << xn << std::endl;
				std::cout << "f(xn): " << funktion(xn) << std::endl;
			}
			xx = xn;
		}
		std::cout << "Laenge des Gradienten:\n";
		std::cout << gradient(xx, funktion).getLaenge() << std::endl;
		if (gradient(xx, funktion).getLaenge() < limit)
		{
			std::cout << "Limit erreicht, breche ab.\n";
			break;
		}
	}
	std::cout << "Endwerte:\n";
	std::cout << "  xx : " << xx << std::endl;
	std::cout << "f(xx): " << funktion(xx) << std::endl;
}