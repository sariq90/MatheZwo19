#pragma once
#include "CZufall.h"


class CLotto
{
private:
	CZufall rnd;
	std::vector<int> tippzettel;
	int tippzahl = 6;
	int tippmax = 49;
	int tippmin = 1;

	std::vector<int> ziehung()
	{
		std::vector<int> result;
		while (result.size() < tippzahl)
		{
			int next = rnd.wert(tippmin, tippmax);
			bool dup = false;
			for (int i = 0; i < result.size(); ++i)
			{
				if (next == result[i])
				{
					dup = true;
					break;
				}
			}
			if (!dup)
				result.push_back(next);
		}
		return result;
	}

public:
	CLotto(int n)
	{
		if (n < 0)
			rnd.initialisiere(time(NULL));
		else
			rnd.initialisiere(n);
	}
	void tippe(std::vector<int> tipp)
	{
		if (tipp.size() == tippzahl)
		{
			for (int i = 0; i < tippzahl; ++i)
			{
				for (int j = i+1; j < tippzahl; ++j)
				{
					if (tipp[i] == tipp[j] || tipp[i] < tippmin || tipp[i] > tippmax)
					{
						// Multiple Ziehzahlen oder auﬂerhalb Ziehbereich
						return;
					}
				}
			}
			tippzettel = tipp;
		}
	}
	int lottoZiehungTipp()
	{
		if (tippzettel.size() == 0)
			return -1;
		std::vector<int> z = ziehung();
		int treffer = 0;
		for (int i = 0; i < tippzahl; ++i)
		{
			for (int j = 0; j < tippzahl; ++j)
			{
				if (z[j] == tippzettel[i])
					++treffer;
			}
		}
		return treffer;
	}
	int lottoZiehungZiehung()
	{
		std::vector<int> z1 = ziehung();
		std::vector<int> z2 = ziehung();
		int treffer = 0;
		for (int i = 0; i < tippzahl; ++i)
		{
			for (int j = 0; j < tippzahl; ++j)
			{
				if (z1[j] == z2[i])
					++treffer;
			}
		}
		return treffer;
	}
	double monteCarlo(int genau, int N, bool ziehung)
	{
		int result = 0;
		int treffer = 0;
		for (int i = 0; i < N; ++i)
		{
			if (ziehung)
				treffer = lottoZiehungZiehung();
			else
				treffer = lottoZiehungTipp();
			if (treffer == genau)
				result++;
		}
		return 1. * result / N;
	}
};