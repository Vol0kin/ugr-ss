#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

long long int gen_arit_entera(int a, long long int xn, int c, int m)
{
	return (a*xn + c) % m;
}

double gen_arit_real_art(int a, double xn, int c, int m)
{
	double x = (a * xn + c) / m;
	
	return (x - (long long int) x) * m;
}

long long int gen_arit_real_art_corregida(int a, long long int xn, int c, int m)
{
	double x = (a * xn + c) / m;
	x = (x - (long long int) x) * m;
	
	return (long long int)(x + 0.5);
}

double gen_real_fmod(int a, double xn, int c, int m)
{
	return fmod((a * x + c), m);
}
