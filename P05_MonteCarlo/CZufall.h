#pragma once
#include <vector>
#include <time.h>


class CZufall
{
public:
	int wert(int a, int b) { return rand() % (b - a + 1) + a; }
	void initialisiere(int n) { srand(n); }
	std::vector<int> test(int a, int b, int N)
	{
		std::vector<int> result = std::vector<int>(b - a + 1, 0);
		for (int i = 0; i < N; ++i)
		{
			int roll = wert(a, b);
			++result[roll - a];
		}
		return result;
	}
	std::vector<int> test_falsch(int a, int b, int N)
	{
		std::vector<int> result = std::vector<int>(b - a + 1, 0);
		for (int i = 0; i < N; ++i)
		{
			initialisiere(time(NULL));
			int roll = wert(a, b);
			++result[roll - a];
		}
		return result;
	}
};