#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

#define suceso_llegada 0
#define suceso_salida 1
#define suceso_finsimulacion 3
#define suceso_monitor 4

typedef struct
{
    int suceso;
    float tiempo;
    float retraso;
} suc;
list<suc> lsuc;
suc nodo;
int nada = 0;
list<float> cola;
int m; //numero de servidores
float tparada;
float tlleg;
float tserv;
float valor;

float reloj;
int libres;
int encola;
int ensistema;
int atendidos;
float acum_cola;
float acum_sistema;
float acum_ocio;
float acum_retraso;
float tultsuc_cola;
float tultsuc_ocio;
float tultsuc_sistema;
float acum_sincola;
float init_sincola;
int maximacola;
bool parar;
int num_simulaciones;
// Acumuladores para valores medios
float acumRetrasoMedio = 0.0,
      acumEstanciaMedia = 0.0,
      acumEnColaMedio = 0.0,
      acumEnSistemaMedio = 0.0,
      acumColasNoVaciasMedio = 0.0,
      acumPorcentajeMedioOcio = 0.0,
      acumMaximaCola = 0.0;

float retrasoMedioDev = 0.0,
      estanciaMediaDev = 0.0,
      enColaMedioDev = 0.0,
      enSistemaMedioDev = 0.0,
      colasNoVaciasMedioDev = 0.0,
      porcentajeMedioOcioDev = 0.0,
      maximaColaDev = 0.0;

bool compare(const suc &s1, const suc &s2)
{
    return s1.tiempo < s2.tiempo;
}

/* Inserta de forma ordenada un elemento en la lista de sucesos */
void insertar_lsuc(suc n)
{
    lsuc.push_back(n);
    // Mantener ordenada la lista por el tiempo de los sucesos
    lsuc.sort(compare);
}

float generador_exponencial(float media)
{
    float u;
    u = (float)random();
    u = u / (float)(RAND_MAX + 1.0);
    return (-media * log(1 - u));
}

// Generador de tiempos entre llegadas (exponencial)
float generallegada(float media)
{
    return generador_exponencial(media);
}

// Generador de tiempos de servicio (exponencial)
float generaservicio(float media)
{
    return generador_exponencial(media);
}

/* Procedimiento inicializacion */
void inicializacion()
{
    lsuc.clear();
    cola.clear();
    reloj = 0.0;
    libres = m;
    encola = 0;
    ensistema = 0;
    atendidos = 0;
    acum_cola = 0.0;
    acum_sistema = 0.0;
    acum_ocio = 0.0;
    acum_retraso = 0.0;
    tultsuc_cola = reloj;
    tultsuc_ocio = reloj;
    tultsuc_sistema = reloj;
    acum_sincola = 0.0;
    init_sincola = reloj;
    maximacola = 0;
    nodo.suceso = suceso_llegada;
    nodo.tiempo = reloj + generallegada(tlleg);
    nodo.retraso = nada;
    insertar_lsuc(nodo);
    nodo.suceso = suceso_finsimulacion;
    nodo.tiempo = reloj + tparada;
    nodo.retraso = nada;
    insertar_lsuc(nodo); //tparada es un par�metro de entrada al programa
    //Inicializar el suceso monitor si que quiere trazar alguna medida del rendimiento a lo largo del tiempo, no solo al final
    /* 
nodo.suceso = suceso_monitor;
nodo.tiempo = reloj+1.0;
nodo.retraso = nada;
insertar_lsuc(nodo);
*/
    parar = false;
}

/* Procedimiento temporizacion */
void temporizacion()
{
    nodo = lsuc.front();
    lsuc.pop_front();
    reloj = nodo.tiempo;
    //  printf("\n%.3f",reloj);
}

void llegada()
{
    acum_sistema += (reloj - tultsuc_sistema) * ensistema;
    tultsuc_sistema = reloj;
    ensistema++;
    nodo.suceso = suceso_llegada;
    nodo.tiempo = reloj + generallegada(tlleg);
    nodo.retraso = nada;
    insertar_lsuc(nodo);
    if (libres > 0)
    {
        acum_ocio += (reloj - tultsuc_ocio) * libres;
        tultsuc_ocio = reloj;
        libres--;
        nodo.suceso = suceso_salida;
        nodo.tiempo = reloj + generaservicio(tserv);
        nodo.retraso = nada;
        insertar_lsuc(nodo);
    }
    else
    {
        if (encola == 0)
            acum_sincola += reloj - init_sincola;
        acum_cola += (reloj - tultsuc_cola) * encola;
        tultsuc_cola = reloj;
        encola++;
        if (encola > maximacola)
            maximacola = encola;
        cola.push_back(reloj);
    }
}

void salida()
{
    acum_sistema += (reloj - tultsuc_sistema) * ensistema;
    tultsuc_sistema = reloj;
    ensistema--;
    atendidos++;
    acum_retraso += nodo.retraso;
    if (encola > 0)
    {
        acum_cola += (reloj - tultsuc_cola) * encola;
        tultsuc_cola = reloj;
        encola--;
        if (encola == 0)
            init_sincola = reloj;
        valor = cola.front();
        cola.pop_front();
        nodo.suceso = suceso_salida;
        nodo.tiempo = reloj + generaservicio(tserv);
        nodo.retraso = reloj - valor;
        insertar_lsuc(nodo);
    }
    else
    {
        acum_ocio += (reloj - tultsuc_ocio) * libres;
        tultsuc_ocio = reloj;
        libres++;
    }
}

void fin()
{
    parar = true; //para detener la simulaci�n
                  //habr� que hacer las �ltimas actualizaciones de algunas variables
    float retrasomedio = acum_retraso / atendidos;
    printf("\nTiempo medio de espera en cola = %.3f", retrasomedio);
    float estanciamedia = retrasomedio + tserv;
    printf("\nTiempo medio de estancia en el sistema = %.3f", estanciamedia);
    acum_cola += (reloj - tultsuc_cola) * encola;

    //printf("\nTiempo medio de espera en cola bis = %.3f",acum_cola/atendidos);
    //printf("\nTiempo medio de estancia en el sistema bis= %.3f",acum_cola/atendidos+tserv);

    float encolamedio = acum_cola / reloj;
    printf("\nNumero medio de personas en cola = %.3f", encolamedio);
    acum_sistema += (reloj - tultsuc_sistema) * ensistema;
    float ensistemamedio = acum_sistema / reloj;
    
    printf("\nNumero medio de personas en el sistema = %.3f", ensistemamedio);
    if (encola == 0)
        acum_sincola += reloj - init_sincola;
    
    float colasnovaciasmedio = acum_cola / (reloj - acum_sincola);
    printf("\nLongitud media de colas no vacias = %.3f", colasnovaciasmedio);
    acum_ocio += (reloj - tultsuc_ocio) * libres;
    float porcentajemedioocio = 100 * acum_ocio / (m * reloj);
    printf("\nporcentaje medio de tiempo de ocio por servidor = %.3f", porcentajemedioocio);
    printf("\nLongitud m�xima de la cola = %d", maximacola);
    printf("\n");

    acumRetrasoMedio += retrasomedio;
    acumEstanciaMedia += estanciamedia;
    acumEnColaMedio += encolamedio;
    acumEnSistemaMedio += ensistemamedio;
    acumColasNoVaciasMedio += colasnovaciasmedio;
    acumPorcentajeMedioOcio += porcentajemedioocio;
    acumMaximaCola += maximacola;

    retrasoMedioDev += pow(retrasomedio, 2);
    estanciaMediaDev += pow(estanciamedia, 2);
    enColaMedioDev += pow(encolamedio, 2);
    enSistemaMedioDev += pow(ensistemamedio, 2);
    colasNoVaciasMedioDev += pow(colasnovaciasmedio, 2);
    porcentajeMedioOcioDev += pow(porcentajemedioocio, 2);
    maximaColaDev += pow(maximacola, 2);
}

float calcularDesviacion(float sum, float media)
{
    float dev = sum - (num_simulaciones * pow(media, 2));
    dev /= (num_simulaciones - 1);

    return sqrt(dev);
}

void mostrarInformacion()
{
    float retraso = acumRetrasoMedio / num_simulaciones,
          estancia = acumEstanciaMedia / num_simulaciones,
          cola = acumEnColaMedio / num_simulaciones,
          sistema = acumEnSistemaMedio / num_simulaciones,
          vacias = acumColasNoVaciasMedio / num_simulaciones,
          ocio = acumPorcentajeMedioOcio / num_simulaciones,
          maxima = acumMaximaCola / num_simulaciones;
    
    float retrasoDev = calcularDesviacion(retrasoMedioDev, retraso),
          estanciaDev = calcularDesviacion(estanciaMediaDev, estancia),
          colaDev = calcularDesviacion(enColaMedioDev, cola),
          sistemaDev = calcularDesviacion(enSistemaMedioDev, sistema),
          vaciasDev = calcularDesviacion(colasNoVaciasMedioDev, vacias),
          ocioDev = calcularDesviacion(porcentajeMedioOcioDev, ocio),
          maximaDev = calcularDesviacion(maximaColaDev, maxima);
    
    cout << fixed;
    cout << setprecision(3);
    cout << "\nVALORES MEDIOS Y DESVIACIONES" << endl;
    cout << "Tiempo medio de espera en cola -> media: " << retraso << " dev: " << retrasoDev << endl;
    cout << "Tiempo medio de estancia en el sistema -> media: " << estancia << " dev: " << estanciaDev << endl;
    cout << "Numero medio de personas en cola -> media: " << cola << " dev: " << colaDev << endl;
    cout << "Numero medio de personas en el sistema -> media: " << sistema << " dev: " << sistemaDev << endl;
    cout << "Longitud media de colas no vacias -> media: " << vacias << " dev: " << vaciasDev << endl;
    cout << "Porcentaje medio de tiempo de ocio por servidor -> media: " << ocio << " dev: " << ocioDev << endl;
    cout << "Longitud máxima de la cola -> media: " << maxima << " dev: " << maximaDev << endl;
}

//Si se desea monitorizar, por ejemplo el n�mero medio de clientes en sistema a lo largo del tiempo se puede usar el suceso monitor siguiente, que habr� que inicializar
void monitor()
{
    nodo.suceso = suceso_monitor;
    nodo.tiempo = reloj + 100.0;
    nodo.retraso = nada;
    insertar_lsuc(nodo);
    cout << "tiempo y media en sistema " << reloj << " " << (acum_sistema + (reloj - tultsuc_sistema) * ensistema) / reloj << endl;
    //printf("\n %.3f, %3f","tiempo y media en sistema",reloj,(acum_sistema+(reloj - tultsuc_sistema) * ensistema)/reloj);
}

/* Procedimiento suceso */
void suceso()
{
    switch (nodo.suceso)
    {
    case suceso_llegada:
        llegada();
        break;
    case suceso_salida:
        salida();
        break;
    case suceso_finsimulacion:
        fin();
        break;
    case suceso_monitor:
        monitor();
        break;
    }
}

int main(int argc, char *argv[])
{

    if (argc != 6)
    {
        printf("\n\nFormato Argumentos -> <numero_servidores tiempo de parada tlleg tserv num_simulaciones>\n\n");
        exit(1);
    }
    sscanf(argv[1], "%d", &m);
    sscanf(argv[2], "%f", &tparada);
    sscanf(argv[3], "%f", &tlleg);
    sscanf(argv[4], "%f", &tserv);
    sscanf(argv[5], "%d", &num_simulaciones);

    // Inicializar semilla solo una vez
    srandom(time(NULL));

    for (int i = 0; i < num_simulaciones; i++)
    {
        inicializacion();
        while (!parar)
        {
            temporizacion();
            suceso();
        }
    }

    mostrarInformacion();
}