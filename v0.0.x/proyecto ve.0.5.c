#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_AVIONES 100  // Numero maximo de aviones que se pueden manejar

// Estructura que representa un avion
typedef struct {
    char nombre[100];  // Nombre del avion
    int capacidad;     // Numero de pasajeros
    int alcance;       // Alcance en kilometros
    float precio;      // Precio en millones de USD
    char tipo[20];     // Tipo de avion: Comercial, Privado, Militar
    char motor[20];    // Tipo de motor: Turbofan, Helice, Reactivo
    float precioHora;  // Precio por hora de vuelo en USD
} Avion;

// Funcion para cargar los aviones desde un archivo de texto
int cargarAviones(Avion aviones[], const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    char linea[256];  // Buffer para leer las lineas del archivo
    int i = 0;

    while (fgets(linea, sizeof(linea), archivo) && i < MAX_AVIONES) {
        char *token = strtok(linea, ",");  // Separar los datos por coma
        if (!token) continue;
        strcpy(aviones[i].nombre, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        aviones[i].capacidad = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        aviones[i].alcance = atoi(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        aviones[i].precio = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(aviones[i].tipo, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(aviones[i].motor, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        aviones[i].precioHora = atof(token);

        i++;
    }

    fclose(archivo);
    return i;
}

// Funcion para seleccionar opciones de listas predefinidas
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
    *resultado = opcion - 1;
}

int main() {
    Avion aviones[MAX_AVIONES];  // Arreglo para almacenar aviones
    int total = cargarAviones(aviones, "aviones.txt");  // Cargar aviones desde archivo

    if (total == 0) {
        return 1;
    }

    printf("=== Buscador de Aviones ===\n");

    // Definir listas de opciones para tipo de avion y tipo de motor
    const char *tipos[] = {"Comercial", "Privado", "Militar"};
    const char *motores[] = {"Turbofan", "Helice", "Reactivo"};

    int tipo_index, motor_index;
    seleccionarOpcion("Seleccione el tipo de avion:", tipos, 3, &tipo_index);
    seleccionarOpcion("Seleccione el tipo de motor:", motores, 3, &motor_index);

    // Solicitar al usuario los criterios de busqueda
    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;

    printf("Capacidad minima de personas: ");
    scanf("%i", &capacidadMin);

    printf("Alcance minimo (km): ");
    scanf("%i", &alcanceMin);

    printf("Precio maximo (millones USD): ");
    scanf("%f", &precioMax);

    printf("Precio maximo por hora de vuelo (USD): ");
    scanf("%f", &precioHoraMax);

    // Buscar aviones que cumplan con los criterios especificados
    printf("\nAviones que cumplen con los requisitos:\n");

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        // Eliminar posibles espacios en tipo/motor
        aviones[i].tipo[strcspn(aviones[i].tipo, "\n")] = '\0';
        aviones[i].motor[strcspn(aviones[i].motor, "\n")] = '\0';

        // Filtrar por los criterios ingresados
        if (strcmp(aviones[i].tipo, tipos[tipo_index]) == 0 &&
            strcmp(aviones[i].motor, motores[motor_index]) == 0 &&
            aviones[i].capacidad >= capacidadMin &&
            aviones[i].alcance >= alcanceMin &&
            aviones[i].precio <= precioMax &&
            aviones[i].precioHora <= precioHoraMax) {

            // Imprimir los resultados
            printf("- %s | Tipo: %s | Motor: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f M | Hora: $%.2f\n",
                   aviones[i].nombre, aviones[i].tipo, aviones[i].motor,
                   aviones[i].capacidad, aviones[i].alcance,
                   aviones[i].precio, aviones[i].precioHora);
            encontrados++;
        }
    }

    // Mensaje en caso de no encontrar resultados
    if (!encontrados) {
        printf("No se encontraron aviones con esos parametros.\n");
    }

    return 0;
}
