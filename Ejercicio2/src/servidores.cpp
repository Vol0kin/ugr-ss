#include <iostream>
#include <cmath>
#include <cstdlib>
#include <list>
#include <vector>
#include <numeric>

using namespace std;

class ModeloServidor
{
    private:
        // Declaracion de identificadores de sucesos
        static const int SUCESO_LLEGADA = 0,
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

        // Suceso actual
        Suceso sucesoActual;

        // Variable que representa el fin de la simulacion
        bool finSimulacion;

        // Vector para acumular los tiempos medios
        vector<double> tiemposMediosEstancia;

        // Numero de simulaciones
        int numSimulaciones;

        ////////////////////////////////////////////////////////////////////////
        // Metodos privados
        ////////////////////////////////////////////////////////////////////////

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
            // Se guarda al final de la lista de tiempos
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

        // Metodo que representa el suceso fin de servicio en el servidor A
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

        // Metodo que representa el suceso fin de servicio en el servidor B
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

        // Metodo que representa el suceso fin de simulacion
        void sucesoFinSimulacion()
        {
            finSimulacion = true;

            double tiempoMedioEstancia = calcularTiempoMedioEstancia();
            tiemposMediosEstancia.push_back(tiempoMedioEstancia);
        }

        // Metodo generador exponencial
        double generadorExponencial(double media) const
        {
            double u;
            u = (double) random();
            u = (double) (u/(RAND_MAX+1.0));
            return (-media * log(1 - u));
        }

        // Metodo para calcular el tiempo de estancia medio al terminar una simulacion
        double calcularTiempoMedioEstancia() const
        {
            return tTotalClientesEnSistema / numClientesAtendidos;
        }

    public:
        // Constructor
        ModeloServidor(int numSimulaciones):numSimulaciones(numSimulaciones) {}

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

            finSimulacion = false;
        }        

        // Metodo que permite determinar si ha finalizado la simulacion
        bool esFinSimulacion() const
        {
            return finSimulacion;
        }

        // Metodo para obtener el siguiente suceso
        void siguienteSuceso()
        {
            // Obtener suceso y actualizar lista de sucesos y reloj
            sucesoActual = listaSucesos.front();
            listaSucesos.pop_front();

            reloj = sucesoActual.tiempo;
        }

        // Metodo para procesar el suceso actual
        void procesarSucesoActual()
        {
            switch(sucesoActual.tipoSuceso)
            {
                case SUCESO_LLEGADA:
                    sucesoLlegadaServidor();
                    break;
                case SUCESO_FIN_SERVICIO_SERVIDOR_A:
                    sucesoFinServicioServidorA();
                    break;
                case SUCESO_FIN_SERVICIO_SERVIDOR_B:
                    sucesoFinServicioServidorB();
                    break;
                case SUCESO_FIN_SIMULACION:
                    sucesoFinSimulacion();
                    break;
            }
        }

        // Metodo para generar el informe final
        void generarInforme() const
        {
            // Obtener suma de los valores medios y suma de los cuadrados de los valores medios
            double sum = accumulate(tiemposMediosEstancia.begin(), tiemposMediosEstancia.end(), 0.0);
            double sum2 = inner_product(tiemposMediosEstancia.begin(),
                                        tiemposMediosEstancia.end(),
                                        tiemposMediosEstancia.begin(),
                                        0.0);
            
            // Obtener valores medios
            double tMedioEstancia = sum / numSimulaciones,
                   desv = sqrt((sum2 - numSimulaciones * tMedioEstancia * tMedioEstancia) / (numSimulaciones - 1));
            
            // Mostrar resultados por pantalla
            cout << "\n\nRESULTADOS PARA " << numSimulaciones << " SIMULACIONES" << endl;
            cout << "Tiempo medio de estancia: " << tMedioEstancia << " +/- " <<  desv << " minutos" << endl;             
        }
};


int main(int argc, char* argv[])
{
    // Comprobar que se pasa el numero de simulaciones como argumento
    if (argc != 2)
    {
        cerr << "ERROR: Uso incorrecto del programa" << endl;
        cerr << "Uso: " << argv[0] << " <num. simulaciones>" << endl;
        exit(1);
    }

    // Obtener el numero de simulaciones
    int numSimulaciones = atoi(argv[1]);

    // Inicializar semilla aleatoria
    srand(time(NULL));

    // Crear instancia del modelo de simulacion
    ModeloServidor* modeloServidor = new ModeloServidor(numSimulaciones);

    // Simular numSimulaciones veces
    for (int i = 0; i < numSimulaciones; i++)
    {
        cout << "Simulacion " << i << "..." << endl;

        // Iniciar el modelo antes de simular
        modeloServidor->inicializarModelo();

        while(!modeloServidor->esFinSimulacion())
        {
            modeloServidor->siguienteSuceso();
            modeloServidor->procesarSucesoActual();
        }
    }

    // Generar informe mostrando la media para todas las simulaciones
    modeloServidor->generarInforme();

    delete modeloServidor;

    return 0;
}