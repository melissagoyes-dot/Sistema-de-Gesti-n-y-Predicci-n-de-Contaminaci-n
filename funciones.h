#include <stdio.h>
#include <string.h>

#define MAX_ZONAS 50
#define MAX_MEDICIONES 200
#define LIMITE_PM25 50.0f
#define LIMITE_NO2 200.0f
#define LIMITE_SO2 125.0f
#define LIMITE_CO2 1000.0f

typedef struct {
    char nombre[50];
} Zona;

typedef struct {
    char zonaNombre[50];
    float co2;
    float so2;
    float no2;
    float pm25;
    float temperatura;
    float humedad;
    float velocidadViento;
} Medicion;

extern Zona zonas[MAX_ZONAS];
extern Medicion mediciones[MAX_MEDICIONES];
extern Medicion medicionActual[MAX_ZONAS];
extern int cantidadZonas;
extern int cantidadMediciones;
extern int cantidadActuales;
extern Medicion medicionPromedioTemporal;

int menu();
void administrarZonas();
void registrarZona();
void listarZonas();
void eliminarZona();
void registrarMedicion();
void monitorearContaminacion();
void predecirContaminacion();
void mostrarAlertas();
void calcularPromedioHistorico();
void mostrarReporte();
void guardarArchivos();
void cargarArchivos();
void generarBaseDatosMock();
void leerCadena(char *cadena, int n);
void limpiarSaltoDeLinea(char *cadena);
void convertirMinusculas(char *cadena);
int validarEnteroConRango(int a, int b);
float validarFloatConRango(float a, float b);
int buscarZonaPorNombre(char *nombre);
int buscarMedicionPorZona(char *zonaNombre);
int buscarMedicionActualPorZona(char *zonaNombre);
int obtenerUltimaMedicionZona(char *zonaNombre);
Medicion *obtenerMedicionActualPorZona(char *zonaNombre);
int obtenerPromedioHistoricoZona(char *zonaNombre, Medicion *promedio);
Medicion *obtenerMedicionParaOpcion(char *zonaNombre, int *usandoHistorico);
const char *obtenerEstado(float valor, float limite);
const char *obtenerEstadoPorTipo(float valor, float limite, int tipo);
const char *obtenerColorPorEstado(const char *estado);
void mostrarZonasParaSeleccion();
void mostrarDetalleAlertasZona(char *zonaNombre);
int contarAlertasEnMedicion(Medicion *medicion);
void cerrarDia();