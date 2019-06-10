#include "CZufall.h"
#include "CLotto.h"
#include <iostream>
using namespace std;

int main()
{
	CZufall z;
	z.initialisiere(0);
	std::vector<int> ausg;
	//b) Fortlaufend pseudorandom mit 0
	for (int i = 0; i < 3; ++i)
	{
		cout << "Seed: 0\n";
		ausg = z.test(3, 7, 10000);
		for (int i = 0; i < ausg.size(); ++i)
			cout << i+3 << ": " << ausg[i] << endl;
		cout << endl;
	}
	//c) Neu-ansetzend pseudorandom mit 0-2
	for (int i = 0; i < 3; ++i)
	{
		cout << "Seed: " << i << endl;
		z.initialisiere(i);
		ausg = z.test(3, 7, 10000);
		for (int i = 0; i < ausg.size(); ++i)
			cout << i + 3 << ": " << ausg[i] << endl;
		cout << endl;
	}
	//d) Fortlaufend pseudorandom mit time
	z.initialisiere(time(NULL));
	for (int i = 0; i < 3; ++i)
	{
		cout << "Seed: Time\n";
		ausg = z.test(3, 7, 10000);
		for (int i = 0; i < ausg.size(); ++i)
			cout << i + 3 << ": " << ausg[i] << endl;
		cout << endl;
	}
	//e) Reset nach 1. Wert mit time
	cout << "Seed: Falsch\n";
	ausg = z.test_falsch(3, 7, 10000);
	for (int i = 0; i < ausg.size(); ++i)
		cout << i + 3 << ": " << ausg[i] << endl;
	cout << endl;
	//3)
	cout << "*******************\n* FORTUNA LOTERIA *\n*******************\n\n";

	CLotto lotto(-1);
	vector<int> tipp = { 5,7,13,23,30,42 };
	lotto.tippe(tipp);
	int h = 3;
	cout << "Monte-Carlo Ziehung-Tipp:\nWahrscheinlichkeit auf "<<h<<" Treffer bei 100000: "
		<< lotto.monteCarlo(h, 1e5, false) * 100 << "%\n";
	cout << "Wahrscheinlichkeit auf " << h << " Treffer bei 1000000: "
		<< lotto.monteCarlo(h, 1e6, false) * 100 << "%\n";
	cout << "Monte-Carlo Ziehung-Ziehung:\nWahrscheinlichkeit auf " << h << " Treffer bei 100000: "
		<< lotto.monteCarlo(h, 1e5, true) * 100 << "%\n";
	cout << "Wahrscheinlichkeit auf " << h << " Treffer bei 1000000: "
		<< lotto.monteCarlo(h, 1e6, true) * 100 << "%\n";

	system("pause");
	return 0;
}