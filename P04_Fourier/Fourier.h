#pragma once
#include <fstream>   
#include <vector>
#include "CKomplex.h"
#include <cmath>
using namespace std;
const double pi = 3.14159265358979323846;

vector<CKomplex>  werte_einlesen(const char *dateiname)
{
	int i, N, idx;
	double re, im;
	vector<CKomplex> werte;
	// File oeffnen
	ifstream fp;
	fp.open(dateiname);
	// Dimension einlesen
	fp >> N;
	// Werte-Vektor anlegen
	werte.resize(N);
	CKomplex null(0, 0);
	for (i = 0; i < N; i++)
		werte[i] = null;
	// Eintraege einlesen und im Werte-Vektor ablegen
	while (!fp.eof())
	{
		fp >> idx >> re >> im;
		CKomplex a(re, im);
		werte[idx] = a;
	}
	// File schliessen
	fp.close();

	return werte;
}

void werte_ausgeben(const char *dateiname, vector<CKomplex> werte, double epsilon = -1.0)
{
	int i;
	int N = werte.size();
	// File oeffnen
	ofstream fp;
	fp.open(dateiname);
	// Dimension in das File schreiben
	fp << N << endl;
	// Eintraege in das File schreiben
	fp.precision(10);
	for (i = 0; i < N; i++)
		if (werte[i].abs() > epsilon)
			fp << i << "\t" << werte[i].re() << "\t" << werte[i].im() << endl;
	// File schliessen
	fp.close();
}

vector<CKomplex> transform(vector<CKomplex> input, bool hin)
{
	// Initialisierung
	int N = input.size();
	vector<CKomplex> output;
	output.resize(N);
	CKomplex null(0, 0);
	for (int i = 0; i < N; ++i)
	{
		output[i] = null;
	}

	if (hin)	//Hintransformation
	{
		for (int n = 0; n < N; ++n)
		{
			CKomplex sum = null;
			for (int k = 0; k < N; ++k)
			{
				sum = sum + input[k] * CKomplex(-2 * pi * k * n / N);
			}
			output[n] = 1 / sqrt(N) * sum;
		}
	}
	else		//Ruecktransformation
	{
		for (int k = 0; k < N; ++k)
		{
			CKomplex sum = null;
			for (int n = 0; n < N; ++n)
			{
				sum = sum + input[n] * CKomplex(2 * pi * k * n / N);
			}
			output[k] = 1 / sqrt(N) * sum;
		}
	}
	return output;
}

double deviation(vector<CKomplex> a, vector<CKomplex> b)
{
	int N = a.size();
	double max = 0.0;
	for (int i = 0; i < N; ++i)
	{
		double dif = a[i].abs() - b[i].abs();
		if (dif < 0)
			dif = -1 * dif;
		if (dif > max)
			max = dif;
	}
	return max;
}