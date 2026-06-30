#include "funciones.h"
#include <stdio.h>

int main() {
    cargarArchivos();
    if (cantidadZonas == 0 && cantidadMediciones == 0) {
        generarBaseDatosMock();
    }
    int opc;
    do { 
        opc = menu();
        switch (opc) {
            case 1:
                administrarZonas();
                break;
            case 2:
                registrarMedicion();
                break;
            case 3:
                monitorearContaminacion();
                break;
            case 4:
                predecirContaminacion();
                break;
            case 5:
                mostrarAlertas();
                break;
            case 6:
                calcularPromedioHistorico();
                break;
            case 7:
                mostrarReporte();
                break;
            case 8:
                cerrarDia(); 
                break;
            case 9:
                printf("\n\033[1;32mMuchas gracias , vuelva pronto :D\033[0m\n");
                break;
        }
    } while (opc!= 9);
    guardarArchivos();
    return 0;
}