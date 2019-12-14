#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;
using namespace std::chrono;

bool servidor_libre;
double acum_cola, inicio_ocio, ocio, reloj, tiempo_llegada, tLlegada, tiempo_salida, tServicio, tultsuc;
long long int atendidos, encola, infinito, numSimul, total_a_atender;

float generaLlegada(float tLlegada)
{
	float u = (float) ((float) random()/(RAND_MAX+1.0));
	return (-tLlegada*log(1-u));
}

float generaServicio(float tServicio)
{
	float u = (float) ((float) random()/(RAND_MAX+1.0));
	return (-tServicio*log(1-u));
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
        cerr << "Uso correcto: ./" << argv[0] << " <t. llegada> <t servicio> <num clientes> [<num simulaciones>]" << endl;
		exit(1);
	}

    double tiempoTotal = 0.0, totalEnCola = 0.0, totalOcio = 0.0;

    // Inicializar semilla
	srand(time(NULL));

    cout << "Num. medio clientes cola, % T. ocio servidor, Num. total clientes, T. total ejecución" << endl;

	for (int i = 0; i < numSimul; i++)
    {
		// Iniciar valores
		iniciarValores();

		high_resolution_clock::time_point tIni, tFin;

        // Obtener tiempo inicial
		tIni = high_resolution_clock::now();

		while(atendidos < total_a_atender)
        {
			reloj = min(tiempo_llegada, tiempo_salida);
            
			if (reloj == tiempo_llegada)
            {
				tiempo_llegada = reloj + generaLlegada(tLlegada);
                
				if (servidor_libre)
                {
					servidor_libre = false;
					tiempo_salida = reloj + generaServicio(tServicio);
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
					tiempo_salida = reloj + generaServicio(tServicio);
				}
                else
                {
					servidor_libre = true;
					inicio_ocio = reloj;
					tiempo_salida = infinito;
				}
			}
		}
        
        tFin = high_resolution_clock::now();

        // Calcular valores estadisticos
        double porcent_ocio = ocio*100/reloj;
        double media_encola = acum_cola/reloj;
        double tiempoIter = duration_cast<duration<double>>(tFin-tIni).count();

        tiempoTotal += tiempoIter;
        totalEnCola += media_encola;
        totalOcio += porcent_ocio;
		
        cout << media_encola << " " << porcent_ocio << " " << total_a_atender << " " << tiempoIter << endl;
	}

    double tiempoMedio = tiempoTotal / numSimul,
           colaMedia = totalEnCola / numSimul,
           mediaOcio = totalOcio / numSimul;

    cout << "\nTiempo medio de ejecución: " << tiempoMedio << " seg." << endl;
    cout << "Numero medio de clientes en cola: " << colaMedia << endl;
    cout << "Tiempo medio del servidor en ocio: " << mediaOcio << endl;

	if (tServicio < tLlegada)
    {
		float p = (float)(tServicio/tLlegada);
		float q = (p*p)/(1-p);
		float pto = 100*(1-p);
        cout <<  endl << "p = tServicio / tLlegada = " << p << endl;
        cout << "Q(n) = (p*p)/(1-p) = " << q << endl;
        cout << "PTO = 100*(1-p) = " << pto << endl;
	}

	return 0;
}