#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace std::chrono;


bool servidor_libre;
double acum_cola, inicio_ocio, ocio, tLlegada, tServicio;
long long int atendidos, encola, infinito, numSimul, reloj, tiempo_llegada,
              tiempo_salida, total_a_atender, tultsuc;

float generaLlegada(float tLlegada)
{
    // Generar valor aleatorio y redondearlo
	float u = (float) ((float) random()/(RAND_MAX+1.0));
	u = ceil(-tLlegada*log(1-u));

	return u != 0 ? u : 1.0;
}

float generaServicio(float tServicio)
{
    // Generar valor aleatorio y redondearlo
	float u = (float) ((float) random()/(RAND_MAX+1.0));
	u = ceil(-tServicio*log(1-u));

    return u != 0 ? u : 1.0;
}

void iniciarValores()
{
    // Inicializar todos los valores
	infinito = 1e30;
	atendidos = 0;
	inicio_ocio = 0.0;
	acum_cola = 0.0;
	reloj = 0;
	tultsuc = reloj;
	servidor_libre = true;
	encola = 0;
	tiempo_llegada = reloj + generaLlegada(tLlegada);
	tiempo_salida = infinito;
	ocio = 0.0;
}

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        tLlegada = atof(argv[1]);
        tServicio = atof(argv[2]);
        total_a_atender = 10000;
        numSimul = 100;
    }
    else if (argc == 4)
    {
        tLlegada = atof(argv[1]);
        tServicio = atof(argv[2]);
        total_a_atender = atoi(argv[3]);
		numSimul = 100;
	}
    else if (argc == 5)
    {
		tLlegada = atof(argv[1]);
        tServicio = atof(argv[2]);
        total_a_atender = atoi(argv[3]);
        numSimul = atoi(argv[4]);
	}
    else
    {
        cerr << "Uso incorrecto del programa" << endl;
        cerr << "Uso correcto: ./" << argv[0] << " <t. llegada> <t servicio> [<num clientes> [<num simulaciones>]]" << endl;
		exit(1);
	}

    double tiempoTotal = 0.0, totalEnCola = 0.0, totalOcio = 0.0;

    // Inicializar semilla
	srand(time(NULL));

    cout << "Num. medio clientes cola, % T. ocio servidor, Num. total clientes, T. total ejecución" << endl;

	for(int i = 0; i < numSimul; i++)
    {
        // Iniciar valores
		iniciarValores();

		high_resolution_clock::time_point tIni, tFin;

        // Obtener tiempo inicial
		tIni = high_resolution_clock::now();

		while(atendidos < total_a_atender)
        {
			if (reloj == tiempo_llegada)
            {
                // Generar llegada
                float llegada = generaLlegada(tLlegada);
				tiempo_llegada = reloj + static_cast<long long int>(llegada);
                
				if (servidor_libre)
                {
					servidor_libre = false;
                    
                    // Generar servicio
                    float servicio = generaServicio(tServicio);
					tiempo_salida = reloj + static_cast<long long int>(servicio);

					ocio += reloj - inicio_ocio;
				}
                else
                {
					acum_cola += (reloj-tultsuc)*encola;
					tultsuc = reloj;
					encola++;
				}
			}

			if (reloj == tiempo_salida)
            {
				atendidos++;

				if (encola > 0)
                {
					acum_cola += (reloj-tultsuc)*encola;
					tultsuc = reloj;
					encola--;

                    // Generar servicio
                    float servicio = generaServicio(tServicio);
					tiempo_salida = reloj + static_cast<long long int>(servicio);
				}
                else
                {
					servidor_libre = true;
					inicio_ocio = reloj;
					tiempo_salida = infinito;
				}
			}

			reloj++;
		}

        tFin = high_resolution_clock::now();

        // Calcular valores estadisticos
        double porcent_ocio = ocio*100/reloj;
        double media_encola = acum_cola/reloj;
        double tiempoIter = duration_cast<duration<double>>(tFin-tIni).count();

        tiempoTotal += tiempoIter;
        totalEnCola += media_encola;
        totalOcio += porcent_ocio;
		
        cout << i << " " << media_encola << " " << porcent_ocio << " " << total_a_atender << " " << tiempoIter << endl;
	}

    double tiempoMedio = tiempoTotal / numSimul,
           colaMedia = totalEnCola / numSimul,
           mediaOcio = totalOcio / numSimul;

    cout << "\nTiempo medio de ejecución: " << tiempoMedio << " seg." << endl;
    cout << "Numero medio de clientes en cola: " << colaMedia << endl;
    cout << "Tiempo medio del servidor en ocio: " << mediaOcio << endl;

	return 0;
}