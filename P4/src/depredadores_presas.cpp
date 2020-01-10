#include <iostream>
#include <cstdlib>

using namespace std;

#define CSV 0

/* ************************************************************************** */
// Declarar datos que utilizaran las funciones

// Numero de ecuaciones
const int numeq = 2;

// Parametros aij
double a11, a12, a21, a22;

// Intervalo de calculo
double dt;

// Tiempos
double t, tinic, tfin;

// Estados
double estado[numeq], oldestado[numeq];
/* ************************************************************************** */

// Procedimiento de salida
void salida()
{
    if (CSV)
    {
        cout << estado[0] << "," << estado[1] << "," << t << endl;

    }
    else
    {
        cout << "x: " << estado[0] << "\ty: " << estado[1] << "\tt: " << t << endl;
    }
}

// Procedimiento de derivacion
void derivacion(double est[numeq], double f[numeq], double tt)
{
    f[0] = a11 * est[0] - a12 * est[0] * est[1];
	f[1] = a21 * est[0] * est[1] - a22 * est[1];
}

// Metodo de integracion de Euler
void oneStepEuler(double inp[numeq], double out[numeq], double tt, double hh)
{
    double f[numeq];

    derivacion(inp, f, tt);

    for (int i=0; i<numeq; i++)
    {
        out[i] = inp[i] + hh * f[i];
    }
}

// Metodo de integracion de Runge-Kutta
void oneStepRungeKutta(double inp[numeq], double out[numeq], double tt, double hh)
{
    double time, f[numeq], incr, k[numeq][4];

    for (int i=0; i<numeq; i++)
    {
        out[i] = inp[i];
    }

    time = tt;

    for (int j=0; j<4; j++)
    {
        derivacion(out, f, time);

        for (int i=0; i<numeq; i++)
        {
            k[i][j] = f[i];
        }

        if (j<2)
        {
            incr = hh/2;
        }
        else
        {
            incr = hh;
        }

        time = tt + incr;

        for (int i=0; i<numeq; i++)
        {
            out[i] = inp[i] + k[i][j] * incr;
        }
    }

    for (int i=0; i<numeq; i++)
    {
        out[i] = inp[i] + hh / 6 *(k[i][0] + 2 * k[i][1] + 2 * k[i][2] + k[i][3]);
    }
}

// Procedimiento de integracion
void integracion(int metodo)
{
    do
    {
        salida();
        
        oldestado[0] = estado[0];
        oldestado[1] = estado[1];

        if (metodo)
        {
            oneStepRungeKutta(oldestado, estado, t, dt);
        }
        else
        {
            oneStepEuler(oldestado, estado, t, dt);
        }

        t += dt;
    } while (t < tfin);
}


int main(int argc, char* argv[])
{
    int metodo;

    if (argc == 11)
    {
        a11 = atof(argv[1]);
        a12 = atof(argv[2]);
        a21 = atof(argv[3]);
        a22 = atof(argv[4]);
        dt = atof(argv[5]);
        tinic = atof(argv[6]);
        tfin = atof(argv[7]);
        estado[0] = atof(argv[8]);
        estado[1] = atof(argv[9]);
        metodo = atoi(argv[10]);
    }
    else
    {
        cerr << "ERROR al ejecutar el programa. Numero de argumentos incorrecto" << endl;
        cerr << "Uso: " << argv[0] << " a11 a12 a21 a22 dt tinic tfin x0 y0 metodo" << endl;
        cerr << "metodo: 0 Euler | 1 Runge-Kutta" << endl;
        exit(-1);
    }

    if (metodo < 0 && metodo > 1)
    {
        cerr << "Valor incorrecto del parametro metodo. Se esperaba 0 para Euler o 1 para Runge-Kutta" << endl;
        exit(-1);
    }

    // Establecer tiempo
    t = tinic;

    // Llamar al metodo de intgegracion
    integracion(metodo);

    return 0;
}
