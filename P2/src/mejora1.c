#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef struct 
{
	int posicion;
	float prob_acumulada;
} entrada;

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
entrada* construye_prop_a(int n) 
{
	int i;
	entrada* temp;
	
	if ((temp = (entrada*) malloc(n*sizeof(entrada))) == NULL)
  {
  	fputs("Error reservando memoria para generador uniforme\n",stderr);
  	exit(1);
  }
  
  temp[0].posicion = 0;
	temp[0].prob_acumulada = 1.0/n;
	
	for (i=1;i<n;i++)
	{
		temp[i].posicion = i;
		temp[i].prob_acumulada = temp[i-1].prob_acumulada + 1.0/n;
	}
	
	return temp;
}

/* Construye la tabla de búsqueda de
	 tamaño n para la distribución de
	 la demanda del apartado (b).*/
entrada* construye_prop_b(int n)
{
	int i, max;
	entrada* temp;
	
	if ((temp = (entrada*) malloc(n*sizeof(entrada))) == NULL)
	{
		fputs("Error reservando memoria para generador proporcional\n",stderr);
		exit(1);
	}
	
	max = (n/2)*(n+1);
	temp[0].posicion = 0;
	temp[0].prob_acumulada = n*1.0/max;
	
	for (i=1;i<n;i++)
	{
		temp[i].posicion = i;
		temp[i].prob_acumulada = temp[i-1].prob_acumulada+(float)(n-i)/max;
	}		
	
	return temp;
}

/* Construye la tabla de búsqueda de
	 tamaño n para la distribución de
	 la demanda del apartado (c). */
entrada* construye_prop_c(int n) 
{
	int i, max;
	entrada* temp;
	
	if ((temp = (entrada*) malloc(n*sizeof(entrada))) == NULL)
	{
		fputs("Error reservando memoria para generador triangular\n",stderr);
		exit(1);
	}
	
	max = n*n/4;
	
	int val_medio = n / 2;
	
	int j = 0;
	temp[j].posicion = val_medio;
	temp[j].prob_acumulada = val_medio / max;
	
	j++;
	
	for (i=val_medio - 1;i > 0;i--)
	{
		int dist_medio = val_medio - i;
		float prob = (float)i / max;
		temp[j].posicion = i;
		temp[j].prob_acumulada = temp[j-1].prob_acumulada + prob;
		j++;
		
		temp[j].posicion = val_medio + dist_medio;
		temp[j].prob_acumulada = temp[j-1].prob_acumulada + prob;
		j++;		
	}
	
	return temp;
}

/* Genera un valor de la
   distribución de la demanda codificada en tabla, por el
   método de tablas de búsqueda. 
   tama es el tamaño de la tabla, 100 en nuestro caso particular */
int genera_demanda(entrada* tabla,int tama)
{
	int i, j;
	double u = uniforme();
	
	i = 0;
	
	while((i<tama) && (u>=tabla[i].prob_acumulada))
	{
		i++;
	}
	
	return tabla[i].posicion;;
}

int main(int argc, char* argv[])
{
	// Comprobar los parametros de la linea de comandos
  if (argc < 3)
  {
    printf("Numero de parametros incorrectos\n");
    printf("Uso esperado: ./%s <num. repeticiones> <num. tabla>\n", argv[0]);
    exit(-1);
  }

	// Obtener parametros
  int veces = atoi(argv[1]),
  		tipo_tabla = atoi(argv[2]);
  
  // Comprobar que el tipo de tabla es correcto
  if (tipo_tabla < 0 || tipo_tabla > 2)
  {
  	printf("Tipo de tabla incorrecto. Se esperaba 0, 1 o 2\n");
  	exit(-1);
  }
  
  printf("Numero de aleatorios a generar: %d\n", veces);
  printf("Tipo de tabla a utilzar: %d\n", tipo_tabla);
  
  // Inicializar semilla
  srand(time(NULL));
  
  // Crear tabla
  entrada* tablademanda;
  
  if (tipo_tabla == 0)
  {
  	tablademanda = construye_prop_a(100);
  }
  else if (tipo_tabla == 1)
  {
  	tablademanda = construye_prop_b(100);
  }
  else
  {
  	tablademanda = construye_prop_c(100);
  }
  
  int demanda;

	clock_t inicio = clock();

	// Generar los valores aleatorios
	for (int i = 0; i < veces; i++)
  {
  	demanda = genera_demanda(tablademanda, 100);
  }
  
  clock_t fin = clock();  
  double tiempo = (double) (fin - inicio) / CLOCKS_PER_SEC;
  
  printf("Tiempo de ejecucion: %f s\n", tiempo);
  
  free(tablademanda);

  return 0;
}
