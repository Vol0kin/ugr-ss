#include <stdio.h>
#include <math.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  double sum = 0.0, sum2 = 0.0;
  int demanda;

  if (argc < 3)
  {
    printf("Numero de parametros incorrectos\n");
    printf("Uso esperado: ./%s <ganancia por venta> <perdida por no venta>", argv[0]);
    exit(-1);
  }

  int x = atoi(argv[1]), y = atoi(argv[2]);

  const int VECES = 100;

  for (int s = 1; s <= 100; s++)
  {
    for (int i = 0; i < VECES; i++)
    {
      demanda = genera_demanda();

      if (s > demanda)
      {
        ganancia = demanda * x - (s - demanda) * y;
      }
      else
      {
        ganancia = s * x;
      }

      sum += ganancia;
      sum2 += ganancia * ganancia;
    }
  }


  return 0;
}
