#include <stdio.h>
#include <string.h>

#define MAX_AVIONES 100

typedef struct {
    char nombre[50];
    int capacidad;
    int alcance;
    float precio;
    char tipo[20];
} Avion;

int cargarAvionesDesdeArchivo(Avion aviones[], const char* aviones.txt) {
    FILE* archivo = fopen(aviones.txt, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo %s\n", aviones.txt);
        return 0;
    }

    int count = 0;
    while (fscanf(archivo, "%s %d %d %f %s",
                  aviones[count].nombre,
                  &aviones[count].capacidad,
                  &aviones[count].alcance,
                  &aviones[count].precio,
                  aviones[count].tipo) == 5) {
        count++;
        if (count >= MAX_AVIONES) break;
    }

    fclose(archivo);
    return count;
}

int main() {
    Avion aviones[MAX_AVIONES];
    int total_aviones = cargarAvionesDesdeArchivo(aviones, "aviones.txt");

    if (total_aviones == 0) {
        printf("No se pudieron cargar aviones.\n");
        return 1;
    }

    int capacidad_deseada, alcance_deseado;
    float presupuesto_max;
    char tipo_deseado[20];

    printf("=== Buscador de Aviones ===\n");

    printf("Ingrese capacidad mínima de pasajeros: ");
    scanf("%d", &capacidad_deseada);

    printf("Ingrese alcance mínimo en km: ");
    scanf("%d", &alcance_deseado);

    printf("Ingrese presupuesto máximo (en millones USD): ");
    scanf("%f", &presupuesto_max);

    printf("Ingrese tipo de avión deseado (Comercial o Privado): ");
    scanf("%s", tipo_deseado);

    printf("\nModelos que cumplen con los requisitos:\n");

    int encontrados = 0;
    for (int i = 0; i < total_aviones; i++) {
        if (aviones[i].capacidad >= capacidad_deseada &&
            aviones[i].alcance >= alcance_deseado &&
            aviones[i].precio <= presupuesto_max &&
            strcasecmp(aviones[i].tipo, tipo_deseado) == 0) {
            printf("- %s | Tipo: %s | Capacidad: %d | Alcance: %d km | Precio: %.2f millones USD\n",
                   aviones[i].nombre, aviones[i].tipo, aviones[i].capacidad,
                   aviones[i].alcance, aviones[i].precio);
            encontrados++;
        }
    }

    if (!encontrados) {
        printf("No se encontraron aviones que cumplan con los requisitos.\n");
    }

    return 0;
}
