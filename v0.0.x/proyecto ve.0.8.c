#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 100  // Maximo numero de aviones a almacenar

// Estructura que representa un avion
typedef struct {
    char nombre[100];     // Nombre del avion
    int capacidad;        // Numero de pasajeros
    int alcance;          // Alcance en kilometros
    float precio;         // Precio en millones de USD
    char tipo[20];        // Tipo de avion: Comercial, Privado, Militar
    char motor[20];       // Tipo de motor: Turbofan, Helice, Reactivo
    float precioHora;     // Precio por hora de vuelo en USD
    char ala[20];         // Tipo de ala: Fija, Delta, Variable, Tandem
} Avion;

// Carga los aviones desde un archivo de texto
int cargarAviones(Avion aviones[], const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    int i = 0;
    // Lee los datos de cada avion mientras no sea fin de archivo y no se supere el maximo
    while (!feof(archivo) && i < MAX_AVIONES) {
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

// Muestra un menu de opciones y guarda la seleccion del usuario
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
    *resultado = opcion - 1;  // Guarda el indice seleccionado
}

// Solicita al usuario los parametros de busqueda numéricos
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

// Verifica si un avion cumple con todos los criterios de busqueda
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

// Imprime la informacion de un avion en formato legible
void imprimirAvion(Avion a) {
    printf("- %s | Tipo: %s | Motor: %s | Ala: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f M | Hora: $%.2f\n",
           a.nombre, a.tipo, a.motor, a.ala, a.capacidad, a.alcance, a.precio, a.precioHora);
}

// Verifica si la combinacion de motor y tipo de ala es invalida
int SeleccionInvalida(int motor_index, int ala_index) {
    // motores[] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    // alas[]    = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};

    if (motor_index == 2) { // Helice
        if (ala_index == 2 || ala_index == 3) return 1; // Invalido: Delta o Variable
    }

    if (motor_index == 1) { // Turbofan
        if (ala_index == 4) return 1; // Invalido: Tandem
    }

    if (motor_index == 3) { // Reactivo
        if (ala_index == 4) return 1; // Invalido: Tandem
    }

    return 0; // Combinacion valida
}

int main() {
    Avion aviones[MAX_AVIONES];

    // Cargar datos desde el archivo
    int total = cargarAviones(aviones, "aviones.txt");
    if (total == 0) return 1;

    printf("=== Buscador de Aviones ===\n");

    // Opciones posibles para tipo, motor y ala
    const char *tipos[] = {"Cualquiera", "Comercial", "Privado", "Militar"};
    const char *motores[] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    const char *alas[] = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};

    int tipo_index, motor_index, ala_index;

    // Repetir seleccion si la combinacion es invalida
    do {
        seleccionarOpcion("Seleccione el tipo de avion:", tipos, 4, &tipo_index);
        seleccionarOpcion("Seleccione el tipo de motor:", motores, 4, &motor_index);
        seleccionarOpcion("Seleccione el tipo de ala:", alas, 5, &ala_index);

        if (SeleccionInvalida(motor_index, ala_index)) {
            printf("\nCombinacion de motor y tipo de ala no es valida. Por favor, selecciona nuevamente.\n\n");
        }
    } while (SeleccionInvalida(motor_index, ala_index));

    // Obtener parametros de busqueda numericos
    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;

    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Determinar filtros activos (NULL si el usuario eligio "Cualquiera")
    const char *tipoFiltro = (tipo_index == 0) ? NULL : tipos[tipo_index];
    const char *motorFiltro = (motor_index == 0) ? NULL : motores[motor_index];
    const char *alaFiltro = (ala_index == 0) ? NULL : alas[ala_index];

    printf("\n=== Resultados de la busqueda ===\n");

    int encontrados = 0;
    // Filtrar e imprimir los aviones que cumplan los criterios
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro,
                            capacidadMin, alcanceMin, precioMax, precioHoraMax)) {
            imprimirAvion(aviones[i]);
            encontrados++;
        }
    }

    // Mostrar mensaje si no se encontraron coincidencias
    if (encontrados == 0) {
        printf("No se encontraron aviones que cumplan los criterios.\n");
    } else {
        printf("\nTotal encontrados: %i\n", encontrados);
    }

    return 0;
}
