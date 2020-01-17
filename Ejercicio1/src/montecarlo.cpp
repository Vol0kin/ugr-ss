#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>

using namespace std;

// Variables globales
int a = 2000, b = 3000, c = 2600;

// Generador de numeros aleatorios uniforme
float uniforme()
{
    float u;
    u = (float) random();
    u = u/(float)(RAND_MAX+1.0);
    return u;
}

// Generador de demanda
int generaDemanda()
{
    float u, x;
    u = uniforme();

    if (u < (c-a)/(b-a))
    {
        x = a+sqrt((b-a)*(c-a)*u);
    }
    else
    {
        x = b-sqrt((b-a)*(b-c)*(1-u));  
    }

    return (int) x; //se convierte a entero porque es la demanda de huevos de pascua
}

int main(int argc, char* argv[])
{
    
    int numVeces,
        demanda,
        mejorSuministro = a;

    double sum,
           sum2,
           ganancia,
           gananciaMediaEsperada,
           desviacionEsperada,
           mejorGananciaMedia = numeric_limits<double>::min(),
           mejorDesviacion = 0.0,
           dineroPagado;
    
    const double PRECIO_HUEVO_VENTA = 0.6,
                 PRECIO_HUEVO_AGOS = 1.0 * 0.25,
                 PRECIO_HUEVO_DIC = 1.5 * 0.25;


    if (argc == 1)
    {
        numVeces = 100;
    }
    else if (argc == 2)
    {
        numVeces = atoi(argv[1]);
    }
    else
    {
        cerr << "Numero de parametros incorrecto" << endl;
        cerr << "Uso: " << argv[0] << "[num. repeticiones]" << endl;
        exit(1);
    }

    // Inicializar semilla aleatoria
    srand(time(NULL));

    // Modelo de Montecarlo
    for (int s = a; s <= b; s++)
    {
        // Inicializar aucumuladores
        sum = 0.0;
        sum2 = 0.0;

        for (int i = 0; i < numVeces; i++)
        {
            // Obtener la demanda
            demanda = generaDemanda();

            // Obtener la ganancia
            ganancia = (double)demanda * PRECIO_HUEVO_VENTA;

            // Obtener el dinero pagado por el chocolate
            if (s >= demanda)
            {
                dineroPagado = (double)s * PRECIO_HUEVO_AGOS;
            }
            else
            {
                dineroPagado = (double)s * PRECIO_HUEVO_AGOS + (double)(demanda - s) * PRECIO_HUEVO_DIC;
            }

            // Calcular la ganancia total
            ganancia -= dineroPagado;

            sum += ganancia;
            sum2 += ganancia * ganancia;
        }

        gananciaMediaEsperada = sum / numVeces;
        desviacionEsperada = sqrt((sum2 - numVeces * gananciaMediaEsperada * gananciaMediaEsperada)/(numVeces - 1));

        if (gananciaMediaEsperada > mejorGananciaMedia)
        {
            mejorSuministro = s;
            mejorGananciaMedia = gananciaMediaEsperada;
            mejorDesviacion = desviacionEsperada;
        }

        cout << "Demanda maxima que se puede satisfacer con el chocolate de agosto:" << s
             << "\t\tKilos de chocolate comprados: " << s * 0.25
             << "\t\tGanancia esperada: " << gananciaMediaEsperada << " +/- " << desviacionEsperada << endl;

    }

    cout << "\n\n--------------------RESULTADOS FINALES--------------------" << endl;    
    cout << "Mejor ganancia media: " << mejorGananciaMedia << " +/- " << mejorDesviacion << " euros" << endl;
    cout << "Demanda maxima que se puede satisfacer con el chocolate de agosto: "
         << mejorSuministro << " unidades" << endl;
    cout << "Kilos de chocolate comprados para satisfacer la demanda: " << mejorSuministro * 0.25 << endl;
    
    return 0;
}