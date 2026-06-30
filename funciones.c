#include "funciones.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

Zona zonas[MAX_ZONAS];
Medicion mediciones[MAX_MEDICIONES];
Medicion medicionActual[MAX_ZONAS];
int cantidadZonas = 0;
int cantidadMediciones = 0;
int cantidadActuales = 0;
Medicion medicionPromedioTemporal;

void leerCadena(char *cadena, int n)
{
    fgets(cadena, n, stdin);
    limpiarSaltoDeLinea(cadena);
}

void convertirMinusculas(char *cadena)
{
    int i = 0;
    while (cadena[i] != '\0')
    {
        cadena[i] = tolower((unsigned char)cadena[i]);
        i++;
    }
}

int validarEnteroConRango(int a, int b)
{
    int n;
    int aux;
    do
    {
        printf("\033[1;33m>> \033[0m");
        aux = scanf("%d", &n);
        while ((getchar()) != '\n')
            ;
        if (aux != 1 || n < a || n > b)
        {
            printf("\033[1;31mEl valor ingresado es incorrecto\033[0m\n");
        }
    } while (aux != 1 || n < a || n > b);
    return n;
}

float validarFloatConRango(int a, int b)
{
    float n;
    int aux;
    do
    {
        printf("\033[1;33m>> \033[0m");
        aux = scanf("%f", &n);
        while ((getchar()) != '\n')
            ;
        if (aux != 1 || n < a || n > b)
        {
            printf("\033[1;31mEl valor ingresado es incorrecto\033[0m\n");
        }
    } while (aux != 1 || n < a || n > b);
    return n;
}

int buscarZonaPorNombre(char *nombre)
{
    if (nombre == NULL)
    {
        return -1;
    }
    convertirMinusculas(nombre);
    for (int i = 0; i < cantidadZonas; i++)
    {
        if (strcmp(zonas[i].nombre, nombre) == 0)
        {
            return i;
        }
    }
    return -1;
}

void limpiarSaltoDeLinea(char *cadena)
{
    if (cadena == NULL)
    {
        return;
    }
    {
        cadena[strcspn(cadena, "\n")] = '\0';
    }
}

int menu()
{
    int opc;
    printf("\n\033[1;36m===== SISTEMA DE GESTION Y PREDICCION DE CONTAMINACION =====\033[0m\n");
    printf("1. Administrar zonas\n");
    printf("2. Registrar nueva medicion\n");
    printf("3. Monitorear contaminacion actual\n");
    printf("4. Predecir contaminacion (24 horas)\n");
    printf("5. Alertas y recomendaciones\n");
    printf("6. Calcular promedio historico\n");
    printf("7. Generar reporte\n");
    printf("8. Cerrar el dia (Pasar datos de hoy al historial)\n");
    printf("\033[1;31m9. Salir\033[0m\n");
    printf("\033[1;33mSeleccione una opcion:\033[0m ");
    opc = validarEnteroConRango(1, 9);
    return opc;
}

int buscarMedicionPorZona(char *zonaNombre)
{
    if (zonaNombre == NULL)
    {
        return -1;
    }
    convertirMinusculas(zonaNombre);
    for (int i = cantidadMediciones - 1; i >= 0; i--)
    {
        if (strcmp(mediciones[i].zonaNombre, zonaNombre) == 0)
        {
            return i;
        }
    }
    return -1;
}

int obtenerUltimaMedicionZona(char *zonaNombre)
{
    return buscarMedicionPorZona(zonaNombre);
}

int buscarMedicionActualPorZona(char *zonaNombre)
{
    if (zonaNombre == NULL)
    {
        return -1;
    }
    convertirMinusculas(zonaNombre);
    for (int i = 0; i < cantidadActuales; i++)
    {
        if (strcmp(medicionActual[i].zonaNombre, zonaNombre) == 0)
        {
            return i;
        }
    }
    return -1;
}

Medicion *obtenerMedicionActualPorZona(char *zonaNombre)
{
    int indice = buscarMedicionActualPorZona(zonaNombre);
    if (indice == -1)
    {
        return NULL;
    }
    return &medicionActual[indice];
}

int obtenerPromedioHistoricoZona(char *zonaNombre, Medicion *promedio)
{
    int contador = 0, limite, cantidadPorZona = 0;
    for (int i = 0; i < cantidadMediciones; i++)
    {
        if (strcmp(mediciones[i].zonaNombre, zonaNombre) == 0)
        {
            cantidadPorZona++;
        }
    }
    if (cantidadPorZona == 0){
        return 0;
    }
    limite = (cantidadPorZona < 30) ? cantidadPorZona : 30;
    memset(promedio, 0, sizeof(Medicion));
    strcpy(promedio->zonaNombre, zonaNombre);
    for (int i = cantidadMediciones - 1; i >= 0 && contador < limite; i--)
    {
        if (strcmp(mediciones[i].zonaNombre, zonaNombre) == 0)
        {
            promedio->co2 += mediciones[i].co2;
            promedio->so2 += mediciones[i].so2;
            promedio->no2 += mediciones[i].no2;
            promedio->pm25 += mediciones[i].pm25;
            promedio->temperatura += mediciones[i].temperatura;
            promedio->humedad += mediciones[i].humedad;
            promedio->velocidadViento += mediciones[i].velocidadViento;
            contador++;
        }
    }
    if (contador > 0)
    {
        promedio->co2 /= contador;
        promedio->so2 /= contador;
        promedio->no2 /= contador;
        promedio->pm25 /= contador;
        promedio->temperatura /= contador;
        promedio->humedad /= contador;
        promedio->velocidadViento /= contador;
    }
    return contador;
}

Medicion *obtenerMedicionParaOpcion(char *zonaNombre, int *usandoHistorico)
{
    int dias, opc;
    Medicion *actual = obtenerMedicionActualPorZona(zonaNombre);
    if (actual != NULL)
    {
        if (usandoHistorico != NULL)
            *usandoHistorico = 0;
        return actual;
    }
    dias = obtenerPromedioHistoricoZona(zonaNombre, &medicionPromedioTemporal);
    if (dias == 0)
    {
        printf("\n\033[1;31mERROR: Actualmente no se ha registrado medicion y tampoco hay datos historicos para la zona %s.\033[0m\n", zonaNombre);
        if (usandoHistorico != NULL)
            *usandoHistorico = 0;
        return NULL;
    }
    printf("\n\033[1;33mActualmente no se ha registrado medicion hoy.\n¿Quieres usar el PROMEDIO HISTORICO guardado (%d dias) para ejecutar esta opcion? (1=Si / 2=No):\033[0m ", dias);
    opc = validarEnteroConRango(1, 2);
    if (opc == 1)
    {
        if (usandoHistorico != NULL)
            *usandoHistorico = 1;
        return &medicionPromedioTemporal;
    }
    if (usandoHistorico != NULL)
        *usandoHistorico = 0;
    return NULL;
}

void cargarArchivos()
{
    FILE *archivoZonas = fopen("zonas.dat", "rb");
    if (archivoZonas != NULL)
    {
        if (fread(&cantidadZonas, sizeof(int), 1, archivoZonas) == 1)
        {
            fread(zonas, sizeof(Zona), cantidadZonas, archivoZonas);
        }
        fclose(archivoZonas);
    }
    else
    {
        cantidadZonas = 0;
    }
    FILE *archivoMediciones = fopen("mediciones.dat", "rb");
    if (archivoMediciones != NULL)
    {
        if (fread(&cantidadMediciones, sizeof(int), 1, archivoMediciones) == 1)
        {
            fread(mediciones, sizeof(Medicion), cantidadMediciones, archivoMediciones);
        }
        fclose(archivoMediciones);
    }
    else
    {
        cantidadMediciones = 0;
    }
}

void guardarArchivos()
{
    FILE *archivoZonas = fopen("zonas.dat", "wb");
    if (archivoZonas != NULL)
    {
        fwrite(&cantidadZonas, sizeof(int), 1, archivoZonas);
        fwrite(zonas, sizeof(Zona), cantidadZonas, archivoZonas);
        fclose(archivoZonas);
    }
    FILE *archivoMediciones = fopen("mediciones.dat", "wb");
    if (archivoMediciones != NULL)
    {
        fwrite(&cantidadMediciones, sizeof(int), 1, archivoMediciones);
        fwrite(mediciones, sizeof(Medicion), cantidadMediciones, archivoMediciones);
        fclose(archivoMediciones);
    }
}

const char *obtenerEstado(float valor, float limite)
{
    if (valor <= limite * 0.50f)
    {
        return "Normal";
    }
    if (valor <= limite * 0.85f)
    {
        return "Precaucion";
    }
    if (valor <= limite)
    {
        return "Alerta";
    }
    return "Critica";
}

const char *obtenerEstadoPorTipo(float valor, float limite, int tipo)
{
    switch (tipo)
    {
    case 3:
        if (valor >= 12.0f && valor <= 25.0f)
            return "Normal";
        if ((valor > 25.0f && valor <= 30.0f) || (valor >= 5.0f && valor < 12.0f))
            return "Precaucion";
        if ((valor > 30.0f && valor <= 35.0f) || (valor >= 0.0f && valor < 5.0f))
            return "Alerta";
        return "Critica";
    case 4:
        if (valor >= 10.0f && valor <= 40.0f)
            return "Normal";
        if ((valor >= 5.0f && valor < 10.0f) || (valor > 40.0f && valor <= 60.0f))
            return "Precaucion";
        if ((valor >= 2.0f && valor < 5.0f) || (valor > 60.0f && valor <= 80.0f))
            return "Alerta";
        return "Critica";
    case 5:
        if (valor >= 40.0f && valor <= 70.0f)
            return "Normal";
        if ((valor > 70.0f && valor <= 80.0f) || (valor >= 30.0f && valor < 40.0f))
            return "Precaucion";
        if ((valor > 80.0f && valor <= 90.0f) || (valor >= 20.0f && valor < 30.0f))
            return "Alerta";
        return "Critica";
    default:
        return obtenerEstado(valor, limite);
    }
}

const char *obtenerColorPorEstado(const char *estado)
{
    if (strcmp(estado, "Normal") == 0)
    {
        return "\033[1;32m"; // Verde fuerte
    }
    if (strcmp(estado, "Precaucion") == 0)
    {
        return "\033[1;33m"; // Amarillo fuerte
    }
    if (strcmp(estado, "Alerta") == 0)
    {
        return "\033[38;5;208m"; // Naranja
    }
    if (strcmp(estado, "Critica") == 0)
    {
        return "\033[1;31m"; // Rojo fuerte
    }
    return "\033[0m";
}

void mostrarZonasParaSeleccion()
{
    if (cantidadZonas == 0)
    {
        printf("\033[1;31mNo hay zonas registradas aun.\033[0m\n");
        return;
    }
    listarZonas();
}

void administrarZonas()
{
    int opc;
    do
    {
        printf("\n\033[1;36m===== ADMINISTRAR ZONAS =====\033[0m\n");
        printf("1. Registrar zona\n");
        printf("2. Mostrar zonas\n");
        printf("3. Volver\n");
        printf("\033[1;33mSeleccione una opcion:\033[0m ");
        opc = validarEnteroConRango(1, 3);

        switch (opc)
        {
        case 1:
            registrarZona();
            break;
        case 2:
            listarZonas();
            break;
        case 3:
            printf("\033[1;32mVolviendo al menu principal...\033[0m\n");
            break;
        }
    } while (opc != 3);
}

void registrarZona()
{
    char nombre[50];
    int index, opc, valido;

    if (cantidadZonas >= MAX_ZONAS)
    {
        printf("\033[1;31mNo hay espacio para registrar mas zonas.\033[0m\n");
        return;
    }
    do
    {
        valido = 1; 
        printf("\033[1;36mIngrese el nombre unico de la zona (solo letras y espacios)\033[0m\n");
        leerCadena(nombre, sizeof(nombre));
        if(strlen(nombre) == 0){
            valido = 0;
        }
        for (int i = 0; nombre[i] != '\0'; i++)
        {
            if (!isalpha((unsigned char)nombre[i]) && nombre[i] != ' ')
            {
                valido = 0;
                break; 
            }
        }
        if (!valido)
        {
            printf("\n\033[1;31mERROR: EL NOMBRE SOLO DEBE CONTENER LETRAS Y ESPACIOS. VUELVA A INTENTARLO.\033[0m\n");
            index = 0;
            continue;
        }
        convertirMinusculas(nombre);
        index = buscarZonaPorNombre(nombre);
        if (index != -1)
        {
            printf("\n\033[1;31mERROR: SE HA INGRESADO UN NOMBRE YA REGISTRADO ANTERIORMENTE, POR FAVOR VUELVA A INTENTARLO\033[0m\n");
        }
    } while (index != -1 || !valido);

    printf("\n\033[1;33mEl nombre de la nueva zona es: '%s'.\033[0m\n", nombre);
    printf("\033[1;31mDespues de que se guarde no vas a poder cambiar el nombre.\033[0m\n");
    printf("¿Deseas guardar esta nueva zona? (1=Si / 2=No): ");
    opc = validarEnteroConRango(1, 2);
    if (opc == 2)
    {
        printf("\033[1;31mRegistro de zona cancelado. No se guardaron los cambios.\033[0m\n");
        return;
    }
    strcpy(zonas[cantidadZonas].nombre, nombre);
    cantidadZonas++;
    guardarArchivos();
    printf("\033[1;32mZona registrada correctamente.\033[0m\n");
}

void listarZonas()
{
    if (cantidadZonas == 0)
    {
        printf("\033[1;31mNo hay zonas registradas.\033[0m\n");
        return;
    }
    printf("\n\033[1;34m===== ZONAS REGISTRADAS =====\033[0m\n");
    printf("+------------------------+\n");
    printf("| \033[1;36mNombre de la zona\033[0m      |\n");
    printf("+------------------------+\n");
    for (int i = 0; i < cantidadZonas; i++)
    {
        printf("| %-22s |\n", zonas[i].nombre);
    }
    printf("+------------------------+\n");
}

void registrarMedicion()
{
    char nombreZona[50];
    int indiceZona,tipoMedicion,indiceActual,opc,indiceHistorico,indiceGuardar,esReemplazo = 0,opc2;
    if (cantidadZonas == 0)
    {
        printf("\033[1;31mDebe registrar al menos una zona antes de ingresar mediciones.\033[0m\n");
        return;
    }
    listarZonas();
    printf("\033[1;36mIngrese el nombre de la zona:\033[0m ");
    leerCadena(nombreZona, sizeof(nombreZona));
    convertirMinusculas(nombreZona);
    indiceZona = buscarZonaPorNombre(nombreZona);
    if (indiceZona == -1)
    {
        printf("\033[1;31mNo se encontro la zona indicada.\033[0m\n");
        return;
    }
    printf("\n\033[1;36m¿Que tipo de medicion desea registrar?\033[0m\n");
    printf("1. Medicion ACTUAL (Datos de hoy para monitoreo y alertas)\n");
    printf("2. Medicion HISTORICA (Datos pasados para engrosar el promedio)\n");
    tipoMedicion = validarEnteroConRango(1, 2);
    if (tipoMedicion == 1)
    {
        indiceActual = buscarMedicionActualPorZona(nombreZona);
        if (indiceActual != -1)
        {
            printf("\n\033[1;33m[ALERTA DE SOBRESCRITURA]\033[0m Ya existe una medicion ACTUAL para '%s'.\n", nombreZona);
            printf("Si continuas, los datos ingresados hoy se van a reemplazar por los nuevos.\n");
            printf("¿Deseas continuar? (1=Si / 2=No): ");
            opc = validarEnteroConRango(1, 2);
            if (opc == 2)
            {
                printf("\033[1;31mOperacion cancelada.\033[0m\n");
                return;
            }
        }
        else
        {
            if (cantidadActuales >= MAX_ZONAS)
            {
                printf("\033[1;31mNo hay espacio en memoria para mas mediciones actuales.\033[0m\n");
                return;
            }
            indiceActual = cantidadActuales;
            cantidadActuales++;
        }
        strcpy(medicionActual[indiceActual].zonaNombre, nombreZona);
        printf("\n\033[1;34m--- INGRESANDO DATOS ACTUALES PARA %s ---\033[0m\n", nombreZona);
        printf("CO2 (ppm): ");
        medicionActual[indiceActual].co2 = validarFloatConRango(0, 10000);
        printf("SO2 (ug/m3): ");
        medicionActual[indiceActual].so2 = validarFloatConRango(0, 10000);
        printf("NO2 (ug/m3): ");
        medicionActual[indiceActual].no2 = validarFloatConRango(0, 10000);
        printf("PM2.5 (ug/m3): ");
        medicionActual[indiceActual].pm25 = validarFloatConRango(0, 10000);
        printf("Temperatura (C): ");
        medicionActual[indiceActual].temperatura = validarFloatConRango(-10, 45);
        printf("Humedad (%%): ");
        medicionActual[indiceActual].humedad = validarFloatConRango(0, 100);
        printf("Velocidad del viento (km/h): ");
        medicionActual[indiceActual].velocidadViento = validarFloatConRango(0, 150);
        guardarArchivos();
        printf("\n\033[1;32m[EXITO]\033[0m Medicion ACTUAL registrada y guardada correctamente.\n");
    }
    else
    {
        indiceHistorico = buscarMedicionPorZona(nombreZona);
        indiceGuardar = cantidadMediciones;
        if (indiceHistorico != -1)
        {
            printf("\n\033[1;33m[INFO]\033[0m Ya existen registros historicos para '%s'.\n", nombreZona);
            printf("¿Que accion deseas realizar?\n");
            printf("1. Agregar mediciones a los registros\n");
            printf("2. EDITAR/REEMPLAZAR el ultimo registro historico guardado\n");
            opc2 = validarEnteroConRango(1, 2);
            if (opc2 == 2)
            {
                printf("\n\033[1;33m[ALERTA DE SOBRESCRITURA]\033[0m Si continuas, los datos del ultimo registro historico de '%s' se van a reemplazar y actualizar.\n", nombreZona);
                printf("¿Deseas continuar con el reemplazo? (1=Si / 2=No): ");
                int confirmar = validarEnteroConRango(1, 2);
                if (confirmar == 2)
                {
                    printf("\033[1;31mOperacion cancelada.\033[0m\n");
                    return;
                }
                indiceGuardar = indiceHistorico;
                esReemplazo = 1;
            }
        }
        if (!esReemplazo && cantidadMediciones >= MAX_MEDICIONES)
        {
            printf("\033[1;31mLa base de datos historica esta llena (Max: %d).\033[0m\n", MAX_MEDICIONES);
            return;
        }
        strcpy(mediciones[indiceGuardar].zonaNombre, nombreZona);
        printf("\n\033[1;34m--- INGRESANDO REGISTRO HISTORICO PARA %s ---\033[0m\n", nombreZona);
        printf("CO2 (ppm): ");
        mediciones[indiceGuardar].co2 = validarFloatConRango(0, 10000);
        printf("SO2 (ug/m3): ");
        mediciones[indiceGuardar].so2 = validarFloatConRango(0, 10000);
        printf("NO2 (ug/m3): ");
        mediciones[indiceGuardar].no2 = validarFloatConRango(0, 10000);
        printf("PM2.5 (ug/m3): ");
        mediciones[indiceGuardar].pm25 = validarFloatConRango(0, 10000);
        printf("Temperatura (C): ");
        mediciones[indiceGuardar].temperatura = validarFloatConRango(-10, 45);
        printf("Humedad (%%): ");
        mediciones[indiceGuardar].humedad = validarFloatConRango(0, 100);
        printf("Velocidad del viento (km/h): ");
        mediciones[indiceGuardar].velocidadViento = validarFloatConRango(0, 150);
        if (!esReemplazo)
        {
            cantidadMediciones++;
        }
        guardarArchivos();
        if (esReemplazo)
        {
            printf("\n\033[1;32m[EXITO]\033[0m Medicion HISTORICA reemplazada y actualizada correctamente.\n");
        }
        else
        {
            printf("\n\033[1;32m[EXITO]\033[0m Medicion HISTORICA anadida a la base de datos correctamente.\n");
        }
    }
}

void monitorearContaminacion()
{
    char nombreZona[50];
    int indiceZona, usandoHistorico = 0;
    if (cantidadZonas == 0)
    {
        printf("\033[1;31mNo hay zonas registradas para monitorear.\033[0m\n");
        return;
    }
    listarZonas();
    printf("\033[1;36mIngrese el nombre de la zona:\033[0m ");
    leerCadena(nombreZona, sizeof(nombreZona));
    indiceZona = buscarZonaPorNombre(nombreZona);
    if (indiceZona == -1)
    {
        printf("\033[1;31mNo se encontro la zona indicada.\033[0m\n");
        return;
    }
    Medicion *medicion = obtenerMedicionParaOpcion(nombreZona, &usandoHistorico);
    if (medicion == NULL)
    {
        printf("\033[1;31mNo se puede monitorear la zona %s porque no hay una medicion actual disponible.\033[0m\n", nombreZona);
        return;
    }
    if (usandoHistorico)
    {
        printf("\033[1;33mSe esta usando el historial guardado para esta opcion.\033[0m\n");
    }
    printf("\n\033[1;34mMonitoreo de contaminacion para la zona %s\033[0m\n", nombreZona);
    printf("\033[1;36m%-12s %-12s %-12s\033[0m\n", "Indicador", "Valor", "Estado");
    printf("----------------------------------------\n");
    printf("%-12s %-12.2f %s%-12s%s\n", "PM2.5", medicion->pm25, obtenerColorPorEstado(obtenerEstado(medicion->pm25, LIMITE_PM25)), obtenerEstado(medicion->pm25, LIMITE_PM25), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "NO2", medicion->no2, obtenerColorPorEstado(obtenerEstado(medicion->no2, LIMITE_NO2)), obtenerEstado(medicion->no2, LIMITE_NO2), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "SO2", medicion->so2, obtenerColorPorEstado(obtenerEstado(medicion->so2, LIMITE_SO2)), obtenerEstado(medicion->so2, LIMITE_SO2), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "CO2", medicion->co2, obtenerColorPorEstado(obtenerEstado(medicion->co2, LIMITE_CO2)), obtenerEstado(medicion->co2, LIMITE_CO2), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "Temp.", medicion->temperatura, obtenerColorPorEstado(obtenerEstadoPorTipo(medicion->temperatura, 30.0f, 3)), obtenerEstadoPorTipo(medicion->temperatura, 30.0f, 3), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "Humedad", medicion->humedad, obtenerColorPorEstado(obtenerEstadoPorTipo(medicion->humedad, 70.0f, 5)), obtenerEstadoPorTipo(medicion->humedad, 70.0f, 5), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "Viento", medicion->velocidadViento, obtenerColorPorEstado(obtenerEstadoPorTipo(medicion->velocidadViento, 10.0f, 4)), obtenerEstadoPorTipo(medicion->velocidadViento, 10.0f, 4), "\033[0m");
}

void predecirContaminacion()
{
    char nombreZona[50];
    int indiceZona, usandoHistorico = 0, encontrados = 0;
    float basePm25, baseNo2, baseSo2, baseCo2, sumaPm25 = 0, sumaNo2 = 0, sumaSo2 = 0, sumaCo2 = 0, predPm25, predNo2, predSo2, predCo2, predTemp, predHum, predViento;
    if (cantidadZonas == 0)
    {
        printf("\033[1;31mNo hay zonas registradas para predecir.\033[0m\n");
        return;
    }
    listarZonas();
    printf("\033[1;36mIngrese el nombre de la zona:\033[0m ");
    leerCadena(nombreZona, sizeof(nombreZona));
    indiceZona = buscarZonaPorNombre(nombreZona);
    if (indiceZona == -1)
    {
        printf("\033[1;31mNo se encontro la zona indicada.\033[0m\n");
        return;
    }
    Medicion *medicion = obtenerMedicionParaOpcion(nombreZona, &usandoHistorico);
    if (medicion == NULL)
    {
        printf("\033[1;31mNo se puede predecir para la zona %s porque no hay una medicion actual disponible.\033[0m\n", nombreZona);
        return;
    }
    if (usandoHistorico)
    {
        printf("\033[1;33mSe esta usando el historial guardado para esta opcion.\033[0m\n");
    }
    basePm25 = medicion->pm25;
    baseNo2 = medicion->no2;
    baseSo2 = medicion->so2;
    baseCo2 = medicion->co2;
    for (int i = cantidadMediciones - 1; i >= 0 && encontrados < 3; i--)
    {
        if (strcmp(mediciones[i].zonaNombre, nombreZona) == 0)
        {
            sumaPm25 += mediciones[i].pm25;
            sumaNo2 += mediciones[i].no2;
            sumaSo2 += mediciones[i].so2;
            sumaCo2 += mediciones[i].co2;
            encontrados++;
        }
    }
    if (encontrados > 0)
    {
        basePm25 = ((sumaPm25 / encontrados) * 0.6f) + (medicion->pm25 * 0.4f);
        baseNo2 = ((sumaNo2 / encontrados) * 0.6f) + (medicion->no2 * 0.4f);
        baseSo2 = ((sumaSo2 / encontrados) * 0.6f) + (medicion->so2 * 0.4f);
        baseCo2 = ((sumaCo2 / encontrados) * 0.6f) + (medicion->co2 * 0.4f);
    }
    predPm25 = basePm25 + (medicion->temperatura * 0.05f) - (medicion->humedad * 0.01f) + (10.0f - medicion->velocidadViento) * 0.08f;
    predNo2 = baseNo2 + (medicion->temperatura * 0.03f) + (10.0f - medicion->velocidadViento) * 0.05f;
    predSo2 = baseSo2 + (medicion->temperatura * 0.04f) + (medicion->humedad * 0.005f);
    predCo2 = baseCo2 + (medicion->temperatura * 0.02f) + (10.0f - medicion->velocidadViento) * 0.05f;
    predTemp = medicion->temperatura + (medicion->humedad > 70.0f ? 0.5f : 0.0f) - (medicion->velocidadViento > 10.0f ? 0.4f : 0.0f);
    predHum = medicion->humedad + (predTemp - medicion->temperatura) * 0.15f;
    predViento = medicion->velocidadViento - (medicion->humedad > 80.0f ? 0.6f : 0.0f) + (medicion->temperatura > 20.0f ? 0.7f : 0.0f);
    printf("\n\033[1;34mPrediccion para las proximas 24 horas en la zona %s (Aplicando Promedio Ponderado)\033[0m\n", nombreZona);
    printf("\033[1;36m%-12s %-12s %-12s\033[0m\n", "Indicador", "Valor previsto", "Estado");
    printf("----------------------------------------\n");
    printf("%-12s %-12.2f %s%-12s%s\n", "PM2.5", predPm25, obtenerColorPorEstado(obtenerEstado(predPm25, LIMITE_PM25)), obtenerEstado(predPm25, LIMITE_PM25), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "NO2", predNo2, obtenerColorPorEstado(obtenerEstado(predNo2, LIMITE_NO2)), obtenerEstado(predNo2, LIMITE_NO2), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "SO2", predSo2, obtenerColorPorEstado(obtenerEstado(predSo2, LIMITE_SO2)), obtenerEstado(predSo2, LIMITE_SO2), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "CO2", predCo2, obtenerColorPorEstado(obtenerEstado(predCo2, LIMITE_CO2)), obtenerEstado(predCo2, LIMITE_CO2), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "Temp.", predTemp, obtenerColorPorEstado(obtenerEstadoPorTipo(predTemp, 30.0f, 3)), obtenerEstadoPorTipo(predTemp, 30.0f, 3), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "Humedad", predHum, obtenerColorPorEstado(obtenerEstadoPorTipo(predHum, 70.0f, 5)), obtenerEstadoPorTipo(predHum, 70.0f, 5), "\033[0m");
    printf("%-12s %-12.2f %s%-12s%s\n", "Viento", predViento, obtenerColorPorEstado(obtenerEstadoPorTipo(predViento, 10.0f, 4)), obtenerEstadoPorTipo(predViento, 10.0f, 4), "\033[0m");
}

void mostrarDetalleAlertasZona(char *zonaNombre)
{
    Medicion *medicion = obtenerMedicionActualPorZona(zonaNombre);
    if (medicion == NULL)
    {
        printf("\033[1;31mNo hay una medicion actual disponible para evaluar alertas en la zona %s.\033[0m\n", zonaNombre);
        return;
    }
    printf("\n\033[1;31m===== DETALLE DE ALERTAS Y RECOMENDACIONES - %s =====\033[0m\n", zonaNombre);
    printf("- PM2.5: %.2f ug/m3 | Estado: %s%s%s [Limite Seguro: 0.1 - %.1f ug/m3]\n",
           medicion->pm25, obtenerColorPorEstado(obtenerEstado(medicion->pm25, LIMITE_PM25)),
           obtenerEstado(medicion->pm25, LIMITE_PM25), "\033[0m", LIMITE_PM25);
    if (medicion->pm25 <= 0.0f)
    {
        printf("  \033[1;33mFallo de Sensor:\033[0m Valor irrealmente bajo, naturalmente no se podria dar este valor. Enviar cuadrilla tecnica para limpiar o recalibrar el sensor de particulas.\n");
    }
    else if (strcmp(obtenerEstado(medicion->pm25, LIMITE_PM25), "Normal") != 0)
    {
        printf("  \033[1;31mRecomendacion (Polucion Alta):\033[0m %s\n", medicion->pm25 > 50.0f ? "Suspension de clases presenciales y prohibicion de circulacion de vehiculos pesados." : "Uso recomendado de mascarillas en grupos vulnerables y reduccion de aforo en parques.");
    }
    printf("\n- NO2: %.2f ug/m3 | Estado: %s%s%s [Limite Seguro: 0.1 - %.1f ug/m3]\n",
           medicion->no2, obtenerColorPorEstado(obtenerEstado(medicion->no2, LIMITE_NO2)),
           obtenerEstado(medicion->no2, LIMITE_NO2), "\033[0m", LIMITE_NO2);
           
    if (medicion->no2 <= 0.0f)
    {
        printf("  \033[1;33mRecomendacion (Fallo de Sensor):\033[0m Lectura nula de gases, naturalmente no se podria dar este valor. Revisar conexiones electricas de la estacion de monitoreo.\n");
    }
    else if (strcmp(obtenerEstado(medicion->no2, LIMITE_NO2), "Normal") != 0)
    {
        printf("  \033[1;31mRecomendacion (Polucion Alta):\033[0m %s\n", medicion->no2 > 200.0f ? "Cierre temporal de vias principales y activacion estricta de Pico y Placa todo el dia." : "Promocion de teletrabajo al 50%% en empresas del sector para reducir trafico.");
    }
    printf("\n- SO2: %.2f ug/m3 | Estado: %s%s%s [Limite Seguro: 0.1 - %.1f ug/m3]\n",
           medicion->so2, obtenerColorPorEstado(obtenerEstado(medicion->so2, LIMITE_SO2)),
           obtenerEstado(medicion->so2, LIMITE_SO2), "\033[0m", LIMITE_SO2);
           
    if (medicion->so2 <= 0.0f)
    {
        printf("  \033[1;33mRecomendacion (Fallo de Sensor):\033[0m Posible obstruccion en el filtro de toma de aire del sensor de azufre, naturalmente no se podria dar este valor. Reemplazar repuesto.\n");
    }
    else if (strcmp(obtenerEstado(medicion->so2, LIMITE_SO2), "Normal") != 0)
    {
        printf("  \033[1;31mRecomendacion (Polucion Alta):\033[0m %s\n", medicion->so2 > 125.0f ? "Clausura temporal de plantas industriales emisoras y advertencia de lluvia acida." : "Alerta a hospitales por incremento de problemas respiratorios e inspeccion de industrias.");
    }
    printf("\n- CO2: %.2f ppm | Estado: %s%s%s [Limite Seguro: 350.0 - %.1f ppm]\n",
           medicion->co2, obtenerColorPorEstado(obtenerEstado(medicion->co2, LIMITE_CO2)),
           obtenerEstado(medicion->co2, LIMITE_CO2), "\033[0m", LIMITE_CO2);  
    if (medicion->co2 < 350.0f)
    {
        printf("  \033[1;33mRecomendacion (Fallo de Sensor):\033[0m El nivel de CO2 es inferior al fondo natural global terrestre, naturalmente no se podria dar este valor. Recalibrar sensor infrarrojo inmediatamente.\n");
    }
    else if (strcmp(obtenerEstado(medicion->co2, LIMITE_CO2), "Normal") != 0)
    {
        printf("  \033[1;31mRecomendacion (Polucion Alta):\033[0m %s\n", medicion->co2 > 1000.0f ? "Evacuacion preventiva de tuneles viales y pasos a desnivel." : "Activacion de corredores peatonales exclusivos y desvios de trafico.");
    }
    const char *estadoTemp = obtenerEstadoPorTipo(medicion->temperatura, 30.0f, 3);
    printf("\n- Temperatura: %.2f C | Estado: %s%s%s [Rango Seguro: 12.0 C - 25.0 C]\n",
           medicion->temperatura, obtenerColorPorEstado(estadoTemp), estadoTemp, "\033[0m");
    if (strcmp(estadoTemp, "Normal") != 0)
    {
        if (medicion->temperatura > 25.0f)
        {
            printf("  \033[1;33mRecomendacion (Calor Extremo):\033[0m Riesgo de golpe de calor. Suspender actividades fisicas intensas al aire libre y mantener hidratacion constante en centros educativos.\n");
        }
        else
        {
            printf("  \033[1;34mRecomendacion (Frio Extremo):\033[0m Riesgo de hipotermia. Monitorear calefaccion en centros de asistencia y habilitar albergues nocturnos para personas vulnerables.\n");
        }
    }
    const char *estadoViento = obtenerEstadoPorTipo(medicion->velocidadViento, 10.0f, 4);
    printf("\n- Viento: %.2f km/h | Estado: %s%s%s [Rango Seguro: 10.0 km/h - 40.0 km/h]\n",
           medicion->velocidadViento, obtenerColorPorEstado(estadoViento), estadoViento, "\033[0m");
    if (strcmp(estadoViento, "Normal") != 0)
    {
        if (medicion->velocidadViento > 40.0f)
        {
            printf("  \033[1;31mRecomendacion (Viento Fuerte):\033[0m Riesgo de desprendimiento de estructuras. Retirar objetos sueltos de balcones y evitar transitar cerca de vallas publicitarias.\n");
        }
        else
        {
            printf("  \033[1;33mRecomendacion (Viento Debil):\033[0m Estancamiento del aire. Prohibicion estricta de quemas agricolas y uso de maquinaria a combustion para evitar acumulacion de smog.\n");
        }
    }
    const char *estadoHum = obtenerEstadoPorTipo(medicion->humedad, 70.0f, 5);
    printf("\n- Humedad: %.2f %% | Estado: %s%s%s [Rango Seguro: 40%% - 70%%]\n",
           medicion->humedad, obtenerColorPorEstado(estadoHum), estadoHum, "\033[0m");  
    if (strcmp(estadoHum, "Normal") != 0)
    {
        if (medicion->humedad > 70.0f)
        {
            printf("  \033[1;34mRecomendacion (Humedad Alta):\033[0m Mayor percepcion termica y proliferacion de moho. Asegurar ventilacion mecanica en interiores y reducir aforos en espacios cerrados.\n");
        }
        else
        {
            printf("  \033[1;33mRecomendacion (Humedad Baja):\033[0m Aire muy seco que irrita vias respiratorias y multiplica riesgo de incendios. Activar riego preventivo en parques y recomendar uso de humidificadores.\n");
        }
    }
}

void mostrarAlertas()
{
    char nombreZona[50];
     int indiceZona;
    if (cantidadZonas == 0)
    {
        printf("\033[1;31mNo hay zonas registradas para evaluar alertas.\033[0m\n");
        return;
    }
    listarZonas();
    printf("\033[1;36mIngrese el nombre de la zona:\033[0m ");
    leerCadena(nombreZona, sizeof(nombreZona));
    indiceZona = buscarZonaPorNombre(nombreZona);
    if (indiceZona == -1)
    {
        printf("\033[1;31mNo se encontro la zona indicada.\033[0m\n");
        return;
    }
    if (obtenerMedicionActualPorZona(nombreZona) == NULL)
    {
        printf("\033[1;31mNo hay mediciones actuales registradas para %s para el dia actual. Ingrese una medicion nueva para evaluar alertas y recomendaciones.\033[0m\n", nombreZona);
        return;
    }
    mostrarDetalleAlertasZona(nombreZona);
}

void calcularPromedioHistorico()
{
    char nombreZona[50];
    int indiceZona,cantidadPorZona = 0,limite = 0,contador = 0;
    float promPm25 = 0.0, promNo2 = 0.0, promSo2 = 0.0, promCo2 = 0.0,promTemp = 0.0, promHum = 0.0, promViento = 0.0;
    if (cantidadMediciones == 0)
    {
        printf("\033[1;31mNo existen mediciones para calcular promedios.\033[0m\n");
        return;
    }
    listarZonas();
    printf("\n\033[1;36mIngrese el nombre de la zona:\033[0m ");
    leerCadena(nombreZona, sizeof(nombreZona));
    indiceZona = buscarZonaPorNombre(nombreZona);
    if (indiceZona == -1)
    {
        printf("\033[1;31mNo se encontro la zona indicada.\033[0m\n");
        return;
    }
    for (int i = 0; i < cantidadMediciones; i++)
    {
        if (strcmp(mediciones[i].zonaNombre, nombreZona) == 0)
        {
            cantidadPorZona++;
        }
    }
    if (cantidadPorZona == 0)
    {
        printf("\033[1;31mNo hay mediciones para esa zona.\033[0m\n");
        return;
    }
    if (cantidadPorZona < 30)
    {
        printf("\033[1;33mNo existen suficientes registros para calcular el promedio de los ultimos 30 dias.\033[0m\n");
        printf("Se utilizara la cantidad disponible: \033[1;36m%d\033[0m\n", cantidadPorZona);
    }
    limite = (cantidadPorZona < 30) ? cantidadPorZona : 30;
    for (int i = cantidadMediciones - 1; i >= 0 && contador < limite; i--)
    {
        if (strcmp(mediciones[i].zonaNombre, nombreZona) == 0)
        {
            promPm25 += mediciones[i].pm25;
            promNo2 += mediciones[i].no2;
            promSo2 += mediciones[i].so2;
            promCo2 += mediciones[i].co2;
            promTemp += mediciones[i].temperatura;
            promHum += mediciones[i].humedad;
            promViento += mediciones[i].velocidadViento;
            contador++;
        }
    }
    promPm25 /= contador;
    promNo2 /= contador;
    promSo2 /= contador;
    promCo2 /= contador;
    promTemp /= contador;
    promHum /= contador;
    promViento /= contador;
    printf("\n\033[1;34mPromedio historico de la zona %s (Ultimos %d dias)\033[0m\n", nombreZona, contador);
    printf("\033[1;36m%-18s %-10s %-12s\033[0m\n", "Variable/Elemento", "Promedio", "Estado");
    printf("---------------------------------------------------\n");
    printf("%-18s %-10.2f %s%-12s%s\n", "PM2.5 (ug/m3)", promPm25, obtenerColorPorEstado(obtenerEstado(promPm25, LIMITE_PM25)), obtenerEstado(promPm25, LIMITE_PM25), "\033[0m");
    printf("%-18s %-10.2f %s%-12s%s\n", "NO2 (ug/m3)", promNo2, obtenerColorPorEstado(obtenerEstado(promNo2, LIMITE_NO2)), obtenerEstado(promNo2, LIMITE_NO2), "\033[0m");
    printf("%-18s %-10.2f %s%-12s%s\n", "SO2 (ug/m3)", promSo2, obtenerColorPorEstado(obtenerEstado(promSo2, LIMITE_SO2)), obtenerEstado(promSo2, LIMITE_SO2), "\033[0m");
    printf("%-18s %-10.2f %s%-12s%s\n", "CO2 (ppm)", promCo2, obtenerColorPorEstado(obtenerEstado(promCo2, LIMITE_CO2)), obtenerEstado(promCo2, LIMITE_CO2), "\033[0m");
    printf("%-18s %-10.2f %s%-12s%s\n", "Temperatura (C)", promTemp, obtenerColorPorEstado(obtenerEstadoPorTipo(promTemp, 30.0f, 3)), obtenerEstadoPorTipo(promTemp, 30.0f, 3), "\033[0m");
    printf("%-18s %-10.2f %s%-12s%s\n", "Humedad (%%)", promHum, obtenerColorPorEstado(obtenerEstadoPorTipo(promHum, 70.0f, 5)), obtenerEstadoPorTipo(promHum, 70.0f, 5), "\033[0m");
    printf("%-18s %-10.2f %s%-12s%s\n", "Viento (km/h)", promViento, obtenerColorPorEstado(obtenerEstadoPorTipo(promViento, 10.0f, 4)), obtenerEstadoPorTipo(promViento, 10.0f, 4), "\033[0m");
}

int contarAlertasEnMedicion(Medicion *medicion)
{
    if (medicion == NULL)
        return 0;
    int alertas = 0;
    if (strcmp(obtenerEstado(medicion->pm25, LIMITE_PM25), "Normal") != 0)
        alertas++;
    if (strcmp(obtenerEstado(medicion->no2, LIMITE_NO2), "Normal") != 0)
        alertas++;
    if (strcmp(obtenerEstado(medicion->so2, LIMITE_SO2), "Normal") != 0)
        alertas++;
    if (strcmp(obtenerEstado(medicion->co2, LIMITE_CO2), "Normal") != 0)
        alertas++;
    if (strcmp(obtenerEstadoPorTipo(medicion->velocidadViento, 10.0f, 4), "Normal") != 0)
        alertas++;
    if (strcmp(obtenerEstadoPorTipo(medicion->humedad, 70.0f, 5), "Normal") != 0)
        alertas++;
    return alertas;
}

void mostrarReporte()
{
    int dias, hayDatosActuales = 0, cantidadAlertas;
    if (cantidadZonas == 0)
    {
        printf("\033[1;31mNo hay zonas registradas para generar un reporte.\033[0m\n");
        return;
    }
    FILE *reporte = fopen("reporte.txt", "w");
    if (reporte == NULL)
    {
        printf("\033[1;31mNo se pudo crear el archivo reporte.txt\033[0m\n");
        return;
    }
    fprintf(reporte, "\n===== TABLA 1: PROMEDIOS HISTORICOS (ULTIMOS 30 DIAS) =====\n");
    fprintf(reporte, "%-22s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n", "Zona", "CO2", "SO2", "NO2", "PM2.5", "Temp", "Hum", "Viento");
    fprintf(reporte, "----------------------------------------------------------------------------------------\n");
    printf("\n\033[1;36m===== TABLA 1: PROMEDIOS HISTORICOS (ULTIMOS 30 DIAS) =====\033[0m\n");
    printf("\033[1;34m%-22s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\033[0m\n", "Zona", "CO2", "SO2", "NO2", "PM2.5", "Temp", "Hum", "Viento");
    printf("----------------------------------------------------------------------------------------\n");
    for (int i = 0; i < cantidadZonas; i++)
    {
        Medicion prom;
        dias = obtenerPromedioHistoricoZona(zonas[i].nombre, &prom);
        if (dias == 0)
        {
            fprintf(reporte, "%-22s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n", zonas[i].nombre, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A");
            printf("%-22s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n", zonas[i].nombre, "N/A", "N/A", "N/A", "N/A", "N/A", "N/A", "N/A");
            continue;
        }
        fprintf(reporte, "%-22s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f\n",
                zonas[i].nombre, prom.co2, prom.so2, prom.no2, prom.pm25, prom.temperatura, prom.humedad, prom.velocidadViento);
        printf("%-22s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f\n",
               zonas[i].nombre, prom.co2, prom.so2, prom.no2, prom.pm25, prom.temperatura, prom.humedad, prom.velocidadViento);
    }
    fprintf(reporte, "\n\n===== TABLA 2: DATOS INGRESADOS HOY=====\n");
    fprintf(reporte, "%-22s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n", "Zona", "CO2", "SO2", "NO2", "PM2.5", "Temp", "Hum", "Viento", "Alertas");
    fprintf(reporte, "-------------------------------------------------------------------------------------------------\n");
    printf("\n\n\033[1;36m===== TABLA 2: DATOS INGRESADOS HOY=====\033[0m\n");
    printf("\033[1;34m%-22s %-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\033[0m\n", "Zona", "CO2", "SO2", "NO2", "PM2.5", "Temp", "Hum", "Viento", "Alertas");
    printf("-------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < cantidadZonas; i++)
    {
        Medicion *actual = obtenerMedicionActualPorZona(zonas[i].nombre);
        if (actual != NULL)
        {
            hayDatosActuales = 1;
            cantidadAlertas = contarAlertasEnMedicion(actual);
            fprintf(reporte, "%-22s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8d\n",
                    zonas[i].nombre, actual->co2, actual->so2, actual->no2, actual->pm25,
                    actual->temperatura, actual->humedad, actual->velocidadViento, cantidadAlertas);
            printf("%-22s %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8.2f %-8d\n",
                   zonas[i].nombre, actual->co2, actual->so2, actual->no2, actual->pm25,
                   actual->temperatura, actual->humedad, actual->velocidadViento, cantidadAlertas);
        }
    }
    if (!hayDatosActuales)
    {
        fprintf(reporte, "No se han registrado mediciones el dia de hoy.\n");
        printf("\033[1;33mNo se han registrado mediciones el dia de hoy.\033[0m\n");
    }
    fclose(reporte);
}

void generarBaseDatosMock()
{
    remove("zonas.dat");
    remove("mediciones.dat");
    cantidadZonas = 0;
    cantidadMediciones = 0;
    cantidadActuales = 0;
    char nombresZonas[9][50] = {
        "eugenio espejo", "centro historico", "eloy alfaro",
        "quitumbe", "la delicia", "calderon",
        "cumbaya", "valle de los chillos", "choco andino"};

    for (int i = 0; i < 9; i++)
    {
        strcpy(zonas[cantidadZonas].nombre, nombresZonas[i]);
        cantidadZonas++;
    }
    for (int z = 0; z < cantidadZonas; z++)
    {
        for (int dia = 1; dia <= 5; dia++)
        {
            strcpy(mediciones[cantidadMediciones].zonaNombre, zonas[z].nombre);
            float base_co2 = 410.0f + (dia * 2.0f);
            float base_so2 = 10.0f + dia;
            float base_no2 = 30.0f + dia;
            float base_pm25 = 15.0f + (dia * 0.5f);
            float base_temp = 14.0f + dia;
            float base_viento = 8.0f;
            float base_humedad = 60.0f;
            if (strcmp(zonas[z].nombre, "centro historico") == 0 || strcmp(zonas[z].nombre, "eugenio espejo") == 0)
            {
                base_no2 += 80.0f;
                base_viento = 4.0f;
            }
            else if (strcmp(zonas[z].nombre, "quitumbe") == 0 || strcmp(zonas[z].nombre, "eloy alfaro") == 0)
            {
                base_so2 += 70.0f;
                base_pm25 += 20.0f;
            }
            else if (strcmp(zonas[z].nombre, "calderon") == 0)
            {
                base_pm25 += 25.0f;
                base_humedad = 40.0f;
                base_viento = 15.0f;
            }
            else if (strcmp(zonas[z].nombre, "cumbaya") == 0 || strcmp(zonas[z].nombre, "valle de los chillos") == 0)
            {
                base_temp += 5.0f;
                base_co2 += 150.0f;
            }
            else if (strcmp(zonas[z].nombre, "choco andino") == 0)
            {
                base_no2 = 5.0f;
                base_so2 = 2.0f;
                base_pm25 = 8.0f;
                base_humedad = 85.0f;
            }
            mediciones[cantidadMediciones].co2 = base_co2;
            mediciones[cantidadMediciones].so2 = base_so2;
            mediciones[cantidadMediciones].no2 = base_no2;
            mediciones[cantidadMediciones].pm25 = base_pm25;
            mediciones[cantidadMediciones].temperatura = base_temp;
            mediciones[cantidadMediciones].humedad = base_humedad;
            mediciones[cantidadMediciones].velocidadViento = base_viento;
            cantidadMediciones++;
        }
    }
    guardarArchivos();
    printf("\n\033[1;33m[INFO]\033[0m Base de Datos inicializada. Quedan \033[1;36m%d\033[0m espacios libres para nuevas zonas.\n", MAX_ZONAS - cantidadZonas);
}

void cerrarDia()
{
    int opc;
    if (cantidadActuales == 0)
    {
        printf("\n\033[1;33m[INFO]\033[0m No hay mediciones actuales registradas el dia de hoy para cerrar.\n");
        return;
    }
    printf("\n\033[1;36m¿Esta seguro que desea Cerrar el Dia?\033[0m\n");
    printf("Esto tomara todas las mediciones actuales, las guardara en el historial y preparara el sistema para manana.\n");
    printf("(1=Si / 2=No): ");
    opc = validarEnteroConRango(1, 2);
    if (opc == 2)
    {
        printf("\033[1;31mOperacion cancelada.\033[0m\n");
        return;
    }
    printf("\n\033[1;34mIniciando cierre del dia...\033[0m\n");
    int procesadas = 0;
    for (int i = 0; i < cantidadActuales; i++)
    {
        if (cantidadMediciones >= MAX_MEDICIONES)
        {
            printf("\n\033[1;33m[ADVERTENCIA]\033[0m La base de datos historica esta llena. No se pudieron guardar todas las mediciones.\n");
            break;
        }
        strcpy(mediciones[cantidadMediciones].zonaNombre, medicionActual[i].zonaNombre);
        mediciones[cantidadMediciones].co2 = medicionActual[i].co2;
        mediciones[cantidadMediciones].so2 = medicionActual[i].so2;
        mediciones[cantidadMediciones].no2 = medicionActual[i].no2;
        mediciones[cantidadMediciones].pm25 = medicionActual[i].pm25;
        mediciones[cantidadMediciones].temperatura = medicionActual[i].temperatura;
        mediciones[cantidadMediciones].humedad = medicionActual[i].humedad;
        mediciones[cantidadMediciones].velocidadViento = medicionActual[i].velocidadViento;
        cantidadMediciones++;
        procesadas++;
    }
    cantidadActuales = 0;
    guardarArchivos();
    printf("\033[1;32m[EXITO]\033[0m Se ha cerrado el dia correctamente. \033[1;36m%d\033[0m mediciones pasaron al historial.\n", procesadas);
    printf("El sistema ahora esta limpio y listo para registrar las mediciones de un nuevo dia.\n");
}