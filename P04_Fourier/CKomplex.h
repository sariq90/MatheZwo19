#pragma once
#include <cmath>

class CKomplex
{
private:
	double real;
	double imag;

public:
	CKomplex(double a, double b) : real{ a }, imag{ b }{};
	CKomplex(double phi) : real{ cos(phi) }, imag{ sin(phi) }{};
	CKomplex() : real{ 0.0 }, imag{ 0.0 }{};

	double re() { return real; }
	double im() { return imag; }

	double abs() { return sqrt(real*real+imag*imag); }

};

CKomplex operator+(CKomplex a, CKomplex b) { return CKomplex(a.re() + b.re(), a.im() + b.im()); }
CKomplex operator*(CKomplex a, CKomplex b) { return CKomplex(a.re()*b.re() - a.im()*b.im(), a.re()*b.im() + a.im()*b.re()); }
CKomplex operator*(CKomplex a, double b) { return CKomplex(a.re()*b, a.im()*b); }
CKomplex operator*(double b, CKomplex a) { return CKomplex(a.re()*b, a.im()*b); }
