#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 100

// Estructura que representa un avion
typedef struct {
    char nombre[100];  // Nombre del avion
    int capacidad;     // Numero de pasajeros
    int alcance;       // En kilometros
    float precio;      // En millones de USD
    char tipo[20];     // "Comercial" o "Privado"
} Avion;

// Funcion para cargar aviones desde un archivo CSV
int cargarAvionesDesdeCSV(Avion aviones[], const char* archivo_csv) {
    FILE* archivo = fopen(archivo_csv, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo %s\n", archivo_csv);
        return 0;
    }

    char linea[200];
    int count = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        if (count >= MAX_AVIONES) break;

        // Eliminar salto de linea
        linea[strcspn(linea, "\n")] = 0;

        // Separar los campos
        char* nombre = strtok(linea, ",");
        char* capacidad = strtok(NULL, ",");
        char* alcance = strtok(NULL, ",");
        char* precio = strtok(NULL, ",");
        char* tipo = strtok(NULL, ",");

        if (nombre && capacidad && alcance && precio && tipo) {
            // Quitar comillas si las hay
            if (nombre[0] == '"') {
                memmove(nombre, nombre + 1, strlen(nombre));  // Eliminar primera comilla
                char* final = strchr(nombre, '"');
                if (final) *final = '\0';  // Cortar despues de la ultima comilla
            }

            strncpy(aviones[count].nombre, nombre, sizeof(aviones[count].nombre));
            aviones[count].capacidad = atoi(capacidad);
            aviones[count].alcance = atoi(alcance);
            aviones[count].precio = atof(precio);
            strncpy(aviones[count].tipo, tipo, sizeof(aviones[count].tipo));

            count++;
        }
    }

    fclose(archivo);
    return count;
}

int main() {
    Avion aviones[MAX_AVIONES];  // Declaracion del arreglo de aviones
    int total_aviones = cargarAvionesDesdeCSV(aviones, "aviones.csv");  // Cargar datos desde CSV

    if (total_aviones == 0) {
        printf("No se pudieron cargar aviones.\n");
        return 1;
    }

    int capacidad_deseada, alcance_deseada;
    float presupuesto_max;
    char tipo_deseado[20];

    // Solicitud de datos al usuario
    printf("=== Buscador de Aviones ===\n");

    printf("Ingrese capacidad minima de pasajeros: ");
    scanf("%i", &capacidad_deseada);

    printf("Ingrese alcance minimo en km: ");
    scanf("%i", &alcance_deseada);

    printf("Ingrese presupuesto maximo (en millones USD): ");
    scanf("%f", &presupuesto_max);

    printf("Ingrese tipo de avion deseado (Comercial o Privado): ");
    scanf("%s", tipo_deseado);

    // Busqueda de aviones que cumplen con los requisitos
    printf("\nModelos que cumplen con los requisitos:\n");

    int encontrados = 0;
    for (int i = 0; i < total_aviones; i++) {
        if (aviones[i].capacidad >= capacidad_deseada &&
            aviones[i].alcance >= alcance_deseada &&
            aviones[i].precio <= presupuesto_max &&
            strcasecmp(aviones[i].tipo, tipo_deseado) == 0) {

            // Impresion de los modelos encontrados
            printf("- %s | Tipo: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f millones USD\n",
                   aviones[i].nombre, aviones[i].tipo, aviones[i].capacidad,
                   aviones[i].alcance, aviones[i].precio);
            encontrados++;
        }
    }

    // Mensaje en caso de no encontrar aviones que cumplan con los requisitos
    if (!encontrados) {
        printf("No se encontraron aviones que cumplan con los requisitos.\n");
    }

    return 0;
}
