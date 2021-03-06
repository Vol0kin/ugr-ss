#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/* Genera un número uniformemente distribuido en el
   intervalo [0,1) a partir de uno de los generadores
	 disponibles en C. Lo utiliza el generador de demanda */
double uniforme()
{
	int t = rand();
	double f = ((double)RAND_MAX+1.0);
	
	return (double)t/f;
}
/* Construye la tabla de búsqueda de
	 tamaño n para la distribución de
	 la demanda del apartado (a). */
float* construye_prop_a(int n) 
{
	int i;
	float* temp;
	
	if ((temp = (float*) malloc(n*sizeof(float))) == NULL)
  {
  	fputs("Error reservando memoria para generador uniforme\n",stderr);
  	exit(1);
  }
  
	temp[0] = 1.0/n;
	
	for (i=1;i<n;i++)
		temp[i] = temp[i-1]+1.0/n;
	
	return temp;
}

/* Construye la tabla de búsqueda de
	 tamaño n para la distribución de
	 la demanda del apartado (b).*/
float* construye_prop_b(int n)
{
	int i, max;
	float* temp;
	
	if ((temp = (float*) malloc(n*sizeof(float))) == NULL)
	{
		fputs("Error reservando memoria para generador proporcional\n",stderr);
		exit(1);
	}
	
	max = (n/2)*(n+1);
	temp[0] = n*1.0/max;
	
	for (i=1;i<n;i++)
		temp[i] = temp[i-1]+(float)(n-i)/max;
	
	return temp;
}

/* Construye la tabla de búsqueda de
	 tamaño n para la distribución de
	 la demanda del apartado (c). */
float* construye_prop_c(int n) 
{
	int i, max;
	float* temp;
	
	if ((temp = (float*) malloc(n*sizeof(float))) == NULL)
	{
		fputs("Error reservando memoria para generador triangular\n",stderr);
		exit(1);
	}
	
	max = n*n/4;
	temp[0] = 0.0;
	
	for (i=1;i<(n/2);i++)
		temp[i] = temp[i-1]+(float)i/max;
	
	for (i=(n/2);i<n;i++)
		temp[i] = temp[i-1]+(float)(n-i)/max;
	
	return temp;
}

/* Genera un valor de la
   distribución de la demanda codificada en tabla, por el
   método de tablas de búsqueda. 
   tama es el tamaño de la tabla, 100 en nuestro caso particular */
int genera_demanda(float* tabla,int tama)
{
	int i;
	double u = uniforme();
	
	i = 0;
	while((i<tama) && (u>=tabla[i]))
		i++;
	
	return i;
}

int genera_demanda_mejorada()
{
	double u = uniforme();
	
	int i = (int) (u * 100);
	
	return i;
}

int main(int argc, char* argv[])
{
	// Comprobar los parametros de la linea de comandos
  if (argc < 2)
  {
    printf("Numero de parametros incorrectos\n");
    printf("Uso esperado: ./%s <num. repeticiones>\n", argv[0]);
    exit(-1);
  }

	// Obtener parametros
  int veces = atoi(argv[1]);
  
  printf("Numero de aleatorios a generar: %d\n", veces);
  
  // Inicializar semilla
  srand(time(NULL));
  
  // Crear tabla
  float* tablademanda = construye_prop_a(100);
  int demanda;

	clock_t inicio = clock();

	// Generar los valores aleatorios
	for (int i = 0; i < veces; i++)
  {
  	demanda = genera_demanda_mejorada();
  }
  
  clock_t fin = clock();  
  double tiempo = (double) (fin - inicio) / CLOCKS_PER_SEC;
  
  printf("Tiempo de ejecucion: %f s\n", tiempo);
  
  free(tablademanda);

  return 0;
}
