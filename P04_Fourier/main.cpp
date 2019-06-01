#include "CKomplex.h"
#include "Fourier.h"
#include <vector>
#include <iostream>
using namespace std;

int main()
{
	vector<CKomplex> inp = werte_einlesen("Daten_original.txt");
	vector<CKomplex> outp = transform(inp, true);

	werte_ausgeben("out_-1.txt", outp);
	werte_ausgeben("out_1.txt", outp,1);
	werte_ausgeben("out_0_1.txt", outp,0.1);

	vector<CKomplex> invinp_min1 = werte_einlesen("out_-1.txt");
	vector<CKomplex> invinp_1 = werte_einlesen("out_1.txt");
	vector<CKomplex> invinp_0_1 = werte_einlesen("out_0_1.txt");

	vector<CKomplex> invoutp_min1 = transform(invinp_min1, false);
	vector<CKomplex> invoutp_1 = transform(invinp_1, false);
	vector<CKomplex> invoutp_0_1 = transform(invinp_0_1, false);

	double dev_min1 = deviation(inp, invoutp_min1);
	double dev_1 = deviation(inp, invoutp_1);
	double dev_0_1 = deviation(inp, invoutp_0_1);

	cout << "Maximale Abweichung bei Standard-Epsilon: " << dev_min1 << endl;
	cout << "Maximale Abweichung bei epsilon = 0.1: " << dev_0_1 << endl;
	cout << "Maximale Abweichung bei epsilon = 1.0: " << dev_1 << endl;

	system("pause");
	return 0;
}
