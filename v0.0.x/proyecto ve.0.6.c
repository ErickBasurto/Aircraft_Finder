#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 150  // Numero maximo de aviones a manejar

// Estructura que representa un avion
typedef struct {
    char nombre[100];  // Nombre del avion
    int capacidad;     // Numero de pasajeros
    int alcance;       // Alcance en kilometros
    float precio;      // Precio en millones de USD
    char tipo[20];     // Tipo de avion: Comercial, Privado, Militar
    char motor[20];    // Tipo de motor: Turbofan, Helice, Reactivo
    float precioHora;  // Precio por hora de vuelo en USD
    char ala[20];      // Tipo de ala: Fija, Delta, Variable, Tandem
} Avion;

// Funcion para cargar aviones desde un archivo de texto
int cargarAviones(Avion aviones[], const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    int i = 0;
    while (!feof(archivo) && i < MAX_AVIONES) {
        // Leer los datos desde el archivo y asignarlos a la estructura
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
    return i;
}

// Funcion para seleccionar una opcion de una lista predefinida
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
    Avion aviones[MAX_AVIONES];  // Arreglo que almacena aviones
    int total = cargarAviones(aviones, "aviones.txt");  // Cargar datos desde archivo

    if (total == 0) {
        return 1;
    }

    printf("=== Buscador de Aviones ===\n");

    // Definir listas de opciones para tipo de avion, tipo de motor y tipo de ala
    const char *tipos[] = {"Cualquiera", "Comercial", "Privado", "Militar"};
    const char *motores[] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    const char *alas[] = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};

    int tipo_index, motor_index, ala_index;
    seleccionarOpcion("Seleccione el tipo de avion:", tipos, 4, &tipo_index);
    seleccionarOpcion("Seleccione el tipo de motor:", motores, 4, &motor_index);
    seleccionarOpcion("Seleccione el tipo de ala:", alas, 5, &ala_index);

    // Solicitar valores adicionales para filtrar la busqueda
    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;

    printf("Capacidad minima: ");
    scanf("%i", &capacidadMin);

    printf("Alcance minimo (km): ");
    scanf("%i", &alcanceMin);

    printf("Precio maximo (millones USD): ");
    scanf("%f", &precioMax);

    printf("Precio maximo por hora de vuelo (USD): ");
    scanf("%f", &precioHoraMax);

    // Busqueda de aviones que cumplen con los criterios
    printf("\nAviones que cumplen con los requisitos:\n");

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        int coincideTipo = (tipo_index == 0 || strcmp(aviones[i].tipo, tipos[tipo_index]) == 0);
        int coincideMotor = (motor_index == 0 || strcmp(aviones[i].motor, motores[motor_index]) == 0);
        int coincideAla = (ala_index == 0 || strcmp(aviones[i].ala, alas[ala_index]) == 0);

        // Evaluar si el avion cumple con los criterios ingresados
        if (coincideTipo &&
            coincideMotor &&
            coincideAla &&
            aviones[i].capacidad >= capacidadMin &&
            aviones[i].alcance >= alcanceMin &&
            aviones[i].precio <= precioMax &&
            aviones[i].precioHora <= precioHoraMax) {

            // Imprimir los modelos que cumplen los requisitos
            printf("- %s | Tipo: %s | Motor: %s | Ala: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f M | Hora: $%.2f\n",
                   aviones[i].nombre, aviones[i].tipo, aviones[i].motor, aviones[i].ala,
                   aviones[i].capacidad, aviones[i].alcance,
                   aviones[i].precio, aviones[i].precioHora);
            encontrados++;
        }
    }

    // Mensaje si no se encuentran resultados
    if (!encontrados) {
        printf("No se encontraron aviones con esos parametros.\n");
    }

    return 0;
}
