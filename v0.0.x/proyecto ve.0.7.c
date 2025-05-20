#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 150  // Maximo de aviones a cargar

// Estructura que representa un avion con sus propiedades
typedef struct {
    char nombre[100];     // Nombre modelo del avion
    int capacidad;        // Capacidad de pasajeros
    int alcance;          // Alcance en km
    float precio;         // Precio en millones USD
    char tipo[20];        // Tipo: Comercial, Privado, Militar
    char motor[20];       // Tipo de motor: Turbofan, Helice, Reactivo
    float precioHora;     // Precio por hora de vuelo en USD
    char ala[20];         // Tipo de ala: Fija, Delta, Variable, Tandem
} Avion;

// Carga aviones desde archivo de texto en el arreglo "aviones".
// Retorna el numero total de aviones cargados.
int cargarAviones(Avion aviones[], const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return 0;  // No se pudo abrir archivo
    }

    int i = 0;
    // Leer cada linea hasta fin de archivo o maximo aviones
    while (!feof(archivo) && i < MAX_AVIONES) {
        // Formato de lectura: nombre,capacidad,alcance,precio,tipo,motor,precioHora,ala
        fscanf(archivo, " %99[^,],%i,%i,%f,%19[^,],%19[^,],%f,%19[^\n]\n",
               aviones[i].nombre,
               &aviones[i].capacidad,
               &aviones[i].alcance,
               &aviones[i].precio,
               aviones[i].tipo,
               aviones[i].motor,
               &aviones[i].precioHora,
               aviones[i].ala);
        i++;
    }

    fclose(archivo);
    return i;  // Retorna cuantos aviones cargados
}

// Muestra un menu con opciones y pide seleccion valida al usuario.
// resultado es indice de la opcion seleccionada (0-based)
void seleccionarOpcion(const char *mensaje, const char *opciones[], int cantidad, int *resultado) {
    int opcion = -1;
    while (opcion < 1 || opcion > cantidad) {
        printf("%s\n", mensaje);
        for (int i = 0; i < cantidad; i++) {
            printf("%i. %s\n", i + 1, opciones[i]);
        }
        printf("Selecciona una opcion: ");
        scanf("%i", &opcion);
        if (opcion < 1 || opcion > cantidad) {
            printf("Opcion invalida. Intenta de nuevo.\n\n");
        }
    }
    *resultado = opcion - 1;  // Convertir a 0-based
}

// Pide los parametros numericos para filtrar la busqueda de aviones.
void pedirParametrosBusqueda(int *capacidadMin, int *alcanceMin, float *precioMax, float *precioHoraMax) {
    printf("Capacidad minima: ");
    scanf("%i", capacidadMin);

    printf("Alcance minimo (km): ");
    scanf("%i", alcanceMin);

    printf("Precio maximo (millones USD): ");
    scanf("%f", precioMax);

    printf("Precio maximo por hora de vuelo (USD): ");
    scanf("%f", precioHoraMax);
}

// Devuelve 1 si el avion cumple todos los criterios de busqueda, 0 si no.
int cumpleCriterios(Avion a, const char *tipo, const char *motor, const char *ala,
                    int capacidadMin, int alcanceMin, float precioMax, float precioHoraMax) {
    return
        (tipo == NULL || strcmp(a.tipo, tipo) == 0) &&
        (motor == NULL || strcmp(a.motor, motor) == 0) &&
        (ala == NULL || strcmp(a.ala, ala) == 0) &&
        a.capacidad >= capacidadMin &&
        a.alcance >= alcanceMin &&
        a.precio <= precioMax &&
        a.precioHora <= precioHoraMax;
}

// Imprime la informacion detallada de un avion
void imprimirAvion(Avion a) {
    printf("- %s | Tipo: %s | Motor: %s | Ala: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f M | Hora: $%.2f\n",
           a.nombre, a.tipo, a.motor, a.ala, a.capacidad, a.alcance, a.precio, a.precioHora);
}

int main() {
    Avion aviones[MAX_AVIONES];
    // Cargar datos desde archivo
    int total = cargarAviones(aviones, "aviones.txt");
    if (total == 0) return 1;  // Terminar si no se pudo cargar datos

    printf("=== Buscador de Aviones ===\n");

    // Opciones sin acentos, corresponden a los datos sin acentos
    const char *tipos[] = {"Cualquiera", "Comercial", "Privado", "Militar"};
    const char *motores[] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    const char *alas[] = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};

    int tipo_index, motor_index, ala_index;

    // Pedir al usuario que seleccione cada filtro
    seleccionarOpcion("Seleccione el tipo de avion:", tipos, 4, &tipo_index);
    seleccionarOpcion("Seleccione el tipo de motor:", motores, 4, &motor_index);
    seleccionarOpcion("Seleccione el tipo de ala:", alas, 5, &ala_index);

    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;

    // Pedir parametros numericos para filtrar la busqueda
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Variables para almacenar filtro (NULL si "Cualquiera" seleccionado)
    const char *tipoFiltro = (tipo_index == 0) ? NULL : tipos[tipo_index];
    const char *motorFiltro = (motor_index == 0) ? NULL : motores[motor_index];
    const char *alaFiltro = (ala_index == 0) ? NULL : alas[ala_index];

    printf("\n=== Resultados de la busqueda ===\n");

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro,
                            capacidadMin, alcanceMin, precioMax, precioHoraMax)) {
            imprimirAvion(aviones[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No se encontraron aviones que cumplan los criterios.\n");
    } else {
        printf("\nTotal encontrados: %i\n", encontrados);
    }

    printf("\n");
    return 0;
}
