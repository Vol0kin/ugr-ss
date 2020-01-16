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

            bool operator<(const Suceso& otroSuceso) const
            {
                return tiempo < otroSuceso.tiempo;
            }
        };

        // Lista de sucesos
        list<Suceso> listaSucesos;

        // Lista de tiempos de llegada
        list<double> tiemposLlegadas;

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

            // Limpiar lista de tiempos de llegada por si ha quedado alguan llegada anterior
            tiemposLlegadas.clear();

            // Generar suceso inicial e insertarlo en la lista de sucesos
            Suceso sucesoInicial = generarSuceso(SUCESO_LLEGADA, TIEMPO_MEDIO_LLEGADAS);
            insertarSuceso(sucesoInicial);

            // Generar suceso final e insertarlo en la lista de sucesos
            Suceso sucesoFinal = generarSucesoFinSimulacion();
            insertarSuceso(sucesoFinal);
        }

        // Metodo para generar sucesos
        Suceso generarSuceso(int tipoSuceso, double tiempoMedio) const
        {
            // Generar nuevo suceso
            Suceso suceso;

            suceso.tipoSuceso = tipoSuceso;
            suceso.tiempo = reloj + generadorExponencial(tiempoMedio);

            return suceso;
        }

        // Metodo para generar el suceso fin de simulacion
        Suceso generarSucesoFinSimulacion() const
        {
            // Generar nuevo suceso
            Suceso suceso;

            suceso.tipoSuceso = SUCESO_FIN_SIMULACION;
            suceso.tiempo = reloj + TIEMPO_FINAL;

            return suceso;
        }

        // Metodo para insertar un suceso, manteniendo la lista ordenada por tiempo
        void insertarSuceso(const Suceso& suceso)
        {
            // Insertar el suceso en la lista
            listaSucesos.push_back(suceso);

            // Mantener lista ordenada
            listaSucesos.sort();
        }

        // Metodo que representa el suceso llegada de cliente al servidor
        void sucesoLlegadaServidor()
        {
            // Guardar el tiempo en el que ha llegado el cliente
            // Se guarda al final de la lsita de tiempos
            tiemposLlegadas.push_back(reloj);

            // Generar una nueva llegada
            Suceso llegada = generarSuceso(SUCESO_LLEGADA, TIEMPO_MEDIO_LLEGADAS);
            insertarSuceso(llegada);

            // Comprobar si el servidor A esta libre y realizar accion correspondiente
            if (libreServA)
            {
                libreServA = false;
                
                Suceso servicioServA = generarSuceso(SUCESO_FIN_SERVICIO_SERVIDOR_A, TIEMPO_MEDIO_SERVICIO_SERVIDOR_A);
                insertarSuceso(servicioServA);
            }
            else
            {
                enColaServA++;
            }
        }

        void sucesoFinServicioServidorA()
        {
            // Comprobar si hay clientes en la cola del servidor A
            if (enColaServA > 0)
            {
                enColaServA--;
                
                Suceso servicioServA = generarSuceso(SUCESO_FIN_SERVICIO_SERVIDOR_A, TIEMPO_MEDIO_SERVICIO_SERVIDOR_A);
                insertarSuceso(servicioServA);
            }
            else
            {
                libreServA = true;
            }

            // Comprobar si el servidor B esta libre y realizar accion correspondiente
            if (libreServB)
            {
                libreServB = false;
                
                Suceso servicioServB = generarSuceso(SUCESO_FIN_SERVICIO_SERVIDOR_B, TIEMPO_MEDIO_SERVICIO_SERVIDOR_B);
                insertarSuceso(servicioServB);
            }
            else
            {
                enColaServB++;
            }
        }

        void sucesoFinServicioServidorB()
        {
            // Incrementar el numero de clientes atendidos y el tiempo total
            // de los clientes en el servidor
            numClientesAtendidos++;
            tTotalClientesEnSistema += reloj - tiemposLlegadas.front();

            // Eliminar tiempo de llegada del cliente mas antiguo (el primero)
            tiemposLlegadas.pop_front();

            // Comprobar si hay clientes en la cola del servidor A
            if (enColaServB > 0)
            {
                enColaServB--;

                Suceso servicioServB = generarSuceso(SUCESO_FIN_SERVICIO_SERVIDOR_B, TIEMPO_MEDIO_SERVICIO_SERVIDOR_B);
                insertarSuceso(servicioServB);
            }
            else
            {
                libreServB = true;
            }
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



int main(int argc, char* argv[])
{
    return 0;
}