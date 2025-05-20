#include <stdio.h>
#include <string.h>

#define MAX_AVIONES 5

// Definición de la estructura que representa un avión
typedef struct {
    char nombre[50];
    int capacidad;      // Numero de pasajeros
    int alcance;        // En kilometros
    float precio;       // En millones de USD
    char tipo[20];      // "Comercial" o "Privado"
} Avion;

int main() {
    // Declaración de un arreglo con datos de varios modelos de aviones
    Avion aviones[MAX_AVIONES] = {
        {"Airbus A320", 150, 6100, 98.5, "Comercial"},
        {"Boeing 737", 180, 5600, 110.2, "Comercial"},
        {"Embraer E190", 100, 4500, 54.3, "Comercial"},
        {"Gulfstream G650", 18, 12964, 65.0, "Privado"},
        {"Cessna Citation X", 12, 6100, 23.5, "Privado"}
    };

    int capacidad_deseada, alcance_deseado;
    float presupuesto_max;
    char tipo_deseado[20];

    // Solicitud de datos al usuario
    printf("=== Buscador de Aviones ===\n");

    printf("Ingrese capacidad minima de pasajeros: ");
    scanf("%i", &capacidad_deseada);

    printf("Ingrese alcance minimo en km: ");
    scanf("%i", &alcance_deseado);

    printf("Ingrese presupuesto maximo (en millones USD): ");
    scanf("%f", &presupuesto_max);

    printf("Ingrese tipo de avion deseado (Comercial o Privado): ");
    scanf("%s", tipo_deseado);

    // Búsqueda de aviones que cumplen con los requisitos
    printf("\nModelos que cumplen con los requisitos:\n");

    int encontrados = 0;
    for (int i = 0; i < MAX_AVIONES; i++) {
        if (aviones[i].capacidad >= capacidad_deseada &&
            aviones[i].alcance >= alcance_deseado &&
            aviones[i].precio <= presupuesto_max &&
            strcasecmp(aviones[i].tipo, tipo_deseado) == 0) {

            // Impresión de los modelos que cumplen con los requisitos
            printf("- %s | Tipo: %s | Capacidad: %i | Alcance: %d km | Precio: %.2f millones USD\n",
                   aviones[i].nombre, aviones[i].tipo, aviones[i].capacidad,
                   aviones[i].alcance, aviones[i].precio);
            encontrados++;
        }
    }

    // Mensaje en caso de no encontrar modelos que cumplan con los requisitos
    if (!encontrados) {
        printf("No se encontraron aviones que cumplan con los requisitos.\n");
    }

    return 0;
}
