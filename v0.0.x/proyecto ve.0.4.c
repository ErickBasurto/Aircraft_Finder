#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_AVIONES 100

typedef struct {
    char nombre[100];
    int capacidad;
    int alcance;
    float precio;
    char tipo[20];     // Comercial, Privado, Militar
    char motor[20];    // Turbofan, Hélice, Reactivo
    float precioHora;
} Avion;

int cargarAviones(Avion aviones[], const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    char linea[256];
    int i = 0;

    while (fgets(linea, sizeof(linea), archivo) && i < MAX_AVIONES) {
        char *token = strtok(linea, ",");
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

void seleccionarOpcion(const char *mensaje, const char *opciones[], int cantidad, int *resultado) {
    int opcion = -1;
    while (opcion < 1 || opcion > cantidad) {
        printf("%s\n", mensaje);
        for (int i = 0; i < cantidad; i++) {
            printf("%d. %s\n", i + 1, opciones[i]);
        }
        printf("Selecciona una opción: ");
        scanf("%d", &opcion);
        if (opcion < 1 || opcion > cantidad) {
            printf("Opción inválida. Intenta de nuevo.\n\n");
        }
    }
    *resultado = opcion - 1;
}

int main() {
    Avion aviones[MAX_AVIONES];
    int total = cargarAviones(aviones, "aviones.txt");
    if (total == 0) {
        return 1;
    }

    printf("=== Buscador de Aviones ===\n");

    const char *tipos[] = {"Comercial", "Privado", "Militar"};
    const char *motores[] = {"Turbofan", "Hélice", "Reactivo"};

    int tipo_index, motor_index;
    seleccionarOpcion("Seleccione el tipo de avión:", tipos, 3, &tipo_index);
    seleccionarOpcion("Seleccione el tipo de motor:", motores, 3, &motor_index);

    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;

    printf("Capacidad mínima de personas: ");
    scanf("%d", &capacidadMin);
    printf("Alcance mínimo (km): ");
    scanf("%d", &alcanceMin);
    printf("Precio máximo (millones USD): ");
    scanf("%f", &precioMax);
    printf("Precio máximo por hora de vuelo (USD): ");
    scanf("%f", &precioHoraMax);

    printf("\nAviones que cumplen con los requisitos:\n");

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        // Eliminar posibles espacios en tipo/motor
        aviones[i].tipo[strcspn(aviones[i].tipo, "\n")] = '\0';
        aviones[i].motor[strcspn(aviones[i].motor, "\n")] = '\0';

        if (strcmp(aviones[i].tipo, tipos[tipo_index]) == 0 &&
            strcmp(aviones[i].motor, motores[motor_index]) == 0 &&
            aviones[i].capacidad >= capacidadMin &&
            aviones[i].alcance >= alcanceMin &&
            aviones[i].precio <= precioMax &&
            aviones[i].precioHora <= precioHoraMax) {

            printf("- %s | Tipo: %s | Motor: %s | Capacidad: %d | Alcance: %d km | Precio: %.2f M | Hora: $%.2f\n",
                   aviones[i].nombre, aviones[i].tipo, aviones[i].motor,
                   aviones[i].capacidad, aviones[i].alcance,
                   aviones[i].precio, aviones[i].precioHora);
            encontrados++;
        }
    }

    if (!encontrados) {
        printf("No se encontraron aviones con esos parámetros.\n");
    }

    return 0;
}
