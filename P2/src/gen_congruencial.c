#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

long long int gen_arit_entera(int a, long long int xn, int c, int m)
{
	return (a*xn + c) % m;
}

long long int gen_arit_real_art(int a, long long int xn, int c, int m)
{
	double x = (a * (double) xn + c) / m;
	x = (x - (long long int) x) * m;
	
	return (long long int) x;
}

long long int gen_arit_real_art_corregida(int a, long long int xn, int c, int m)
{
	double x = (a * (double) xn + c) / m;
	x = (x - (long long int) x) * m;
	
	return (long long int)(x + 0.5);
}

long long int gen_real_fmod(int a, long long int xn, int c, int m)
{
	return (long long int) fmod((a * xn + c), m);
}

// Funcion para comprobar si se ha generado un valor anteriormente generado
int comprobar_valor_repetido(long long int* valores_generados, int tam, long long int new_val)
{
	int ya_generado = 0;
	
	for (int i = 0; i < tam; i++)
	{
		if (valores_generados[i] == new_val)
		{
			ya_generado = 1;
		}
	}
	
	return ya_generado;
}

int main(int argc, char* argv[])
{
	// Comprobar tipo de generador a utilizar
	if (argc < 2)
	{
		printf("Numero de parametros incorrectos\n");
    printf("Uso esperado: ./%s <tipo de generador>\n", argv[0]);
    printf("0 -> Generador aritmetica entera\n");
    printf("1 -> Generador aritmetica real artesanal\n");
    printf("2 -> Generador aritmetica real corregida\n");
    printf("3 -> Generador fmod\n");
    exit(-1);
	}
	
	int tipo_generador = atoi(argv[1]);
	
	if (tipo_generador < 0 || tipo_generador > 3)
	{
		printf("Tipo de generador incorrecto\n");
		printf("0 -> Generador aritmetica entera\n");
    printf("1 -> Generador aritmetica real artesanal\n");
    printf("2 -> Generador aritmetica real corregida\n");
    printf("3 -> Generador fmod\n");
    exit(-1);
	}
	
	// Establecer generador a utilizar
	long long int (*gen) (int, long long int, int, int);
	
	if (tipo_generador == 0)
	{
		gen = gen_arit_entera;
	}
	else if (tipo_generador == 1)
	{
		gen = gen_arit_real_art;
	}
	else if (tipo_generador == 2)
	{
		gen = gen_arit_real_art_corregida;
	}
	else
	{
		gen = gen_real_fmod;
	}
	
	// Establecer valor inicial de x
	long long int x0 = 303;
	
	// Establecer resto de parametros de los generadores
	int a1 = 2061, a2 = 2060, c = 4231, m = 1e4;
	
	// Crear x1 y x2
	long long int x1, x2;
	
	// Crear vectores para comprobar los periodos
	long long int* valores_gen1;
	long long int* valores_gen2;
	
	// Establecer numero de elementos en los arrays
	int num_gen1 = 0, num_gen2 = 0;
	
	// Reservar memoria para los generadores
	if ((valores_gen1 = (long long int *) malloc(2 * m * sizeof(long long int))) == NULL)
	{
		fputs("Error reservando memoria para valores generados por generador 1\n",stderr);
  	exit(1);
	}
	
	if ((valores_gen2 = (long long int *) malloc(2 * m * sizeof(long long int))) == NULL)
	{
		fputs("Error reservando memoria para valores generados por generador 2\n",stderr);
  	exit(1);
	}
	
	// Prueba para el periodo del generador 1
	x1 = gen(a1, x0, c, m);
	
	while (comprobar_valor_repetido(valores_gen1, num_gen1, x1) == 0)
	{
		valores_gen1[num_gen1] = x1;
		num_gen1++;
		
		x1 = gen(a1, x1, c, m);
	}
	
	printf("Periodo del generador 1: %d\n", num_gen1);
	
	// Prueba para el periodo del generador 2
	x2 = gen(a2, x0, c, m);
	
	while (comprobar_valor_repetido(valores_gen2, num_gen2, x2) == 0)
	{		
		valores_gen2[num_gen2] = x2;
		num_gen2++;
		
		x2 = gen(a2, x2, c, m);
	}
	
	printf("Periodo del generador 2: %d\n", num_gen2);
	
	// Liberar memoria
	free(valores_gen1);
	free(valores_gen2);
	
	return 0;
}
