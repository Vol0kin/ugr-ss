#include <iostream>
#include <cmath>
#include <cstdlib>
#include <list>

using namespace std;

class ModeloServidor
{
    private:
        // Declaracion de identificadores de sucesos
        const int SUCESO_LLEGADA = 0,
                  SUCESO_FIN_SERVICIO_SERVIDOR_A = 1,
                  SUCESO_FIN_SERVICIO_SERVIDOR_B = 2,
                  SUCESO_FIN_SIMULACION = 3;
        
        // Declaracion de tiempos medios
        const double TIEMPO_MEDIO_LLEGADAS = 1.0,
                     TIEMPO_MEDIO_SERVICIO_SERVIDOR_A = 0.8,
                     TIEMPO_MEDIO_SERVICIO_SERVIDOR_B = 1.2;
        
        // Declaracion de tiempos inicial y final
        const double TIEMPO_INICIAL = 0.0,
                     TIEMPO_FINAL = 60.0 * 8;

        // Estructura suceso
        struct Suceso
        {
            int tipoSuceso;
            double tiempo;
            double tiempoLlegada;

            bool operator<(const Suceso& otroSuceso) const
            {
                return tiempo < otroSuceso.tiempo;
            }
        };

        // Lista de sucesos
        list<Suceso> listaSucesos;

        // Declaracion de variables
        double reloj, tiempoParada, tTotalClientesEnSistema;
        int enColaServA, enColaServB, numClientesAtendidos;
        bool libreServA, libreServB;
    
    public:
        // Constructor
        ModeloServidor() {}

        // Destructor
        ~ModeloServidor() {}

        // Metodo de inicializacion del modelo
        void inicializarModelo()
        {
            // Inicializar reloj y tiempo parada
            reloj = TIEMPO_INICIAL;
            tiempoParada = TIEMPO_FINAL;

            // Inicializar colas y disponibilidad de los servidores
            enColaServA = enColaServB = 0;
            libreServA = libreServB = true;

            // Inicializar valores estadisticos
            numClientesAtendidos = 0;
            tTotalClientesEnSistema = 0.0;

            // Limpiar lista de sucesos por si ha quedado algun suceso anterior
            listaSucesos.clear();

            // Generar suceso inicial e insertarlo en la lista de sucesos
            Suceso sucesoInicial = generarSuceso(SUCESO_LLEGADA, TIEMPO_MEDIO_LLEGADAS);
            insertarSuceso(sucesoInicial);

            // Generar suceso final e insertarlo en la lista de sucesos
            Suceso sucesoFinal = generarSuceso(SUCESO_LLEGADA, TIEMPO_MEDIO_LLEGADAS, true);
            insertarSuceso(sucesoFinal);
        }

        // Metodo para generar sucesos
        Suceso generarSuceso(int tipoSuceso, double tiempoMedio, bool esSucesoFinal=false) const
        {
            // Generar nuevo suceso
            Suceso suceso;

            suceso.tipoSuceso = tipoSuceso;
            suceso.tiempo = generadorExponencial(tiempoMedio);
            suceso.tiempoLlegada = esSucesoFinal ? 0.0 : suceso.tiempo;

            return suceso;
        }

        // Metodo para insertar un suceso
        void insertarSuceso(const Suceso& suceso)
        {
            // Insertar el suceso en la lista
            listaSucesos.push_back(suceso);

            // Mantener lista ordenada
            listaSucesos.sort();
        }

        // Metodo generador exponencial
        double generadorExponencial(double media) const
        {
            double u;
            u = (double) random();
            u = (double) (u/(RAND_MAX+1.0));
            return (-media * log(1 - u));
        }
};



int main()
{
    return 0;
}