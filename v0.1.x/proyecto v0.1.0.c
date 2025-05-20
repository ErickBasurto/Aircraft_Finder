#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 100  // Numero maximo de aviones a manejar

// Estructura que representa un avion con sus atributos
typedef struct {
    char nombre[100];      // Nombre del avion
    int capacidad;         // Numero de pasajeros
    int alcance;           // Alcance en kilometros
    float precio;          // Precio en millones de USD
    char tipo[20];         // Tipo: Comercial, Privado, Militar
    char motor[20];        // Tipo de motor: Turbofan, Helice, Reactivo
    float precioHora;      // Precio por hora de vuelo en USD
    char ala[20];          // Tipo de ala: Fija, Delta, Variable, Tandem
    char categoria[20];    // Categoria: Servicio, Descontinuado
} Avion;

// Carga aviones desde un archivo de texto al arreglo de aviones
int cargarAviones(Avion aviones[], char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    int i = 0;
    while (!feof(archivo) && i < MAX_AVIONES) {
        fscanf(archivo, " %99[^,],%d,%d,%f,%19[^,],%19[^,],%f,%19[^,],%19[^\n]\n",
               aviones[i].nombre,
               &aviones[i].capacidad,
               &aviones[i].alcance,
               &aviones[i].precio,
               aviones[i].tipo,
               aviones[i].motor,
               &aviones[i].precioHora,
               aviones[i].ala,
               aviones[i].categoria);
        i++;
    }

    fclose(archivo);
    return i;
}

// Muestra un menú y solicita una opción válida al usuario
int seleccionarOpcion(char mensaje[], char opciones[][20], int cantidad) {
    int opcion = -1;

    while (opcion < 1 || opcion > cantidad) {
        printf("%s\n", mensaje);
        for (int i = 0; i < cantidad; i++) {
            printf("%d. %s\n", i + 1, opciones[i]);
        }
        printf("Selecciona una opcion: ");
        scanf("%d", &opcion);
        if (opcion < 1 || opcion > cantidad) {
            printf("Opcion invalida\n");
        }
    }

    return opcion - 1; // Retorna el índice (comenzando desde 0)
}

// Solicita al usuario los criterios mínimos de búsqueda
void pedirParametrosBusqueda(int *capacidadMin, int *alcanceMin, float *precioMax, float *precioHoraMax) {
    printf("Capacidad minima: ");
    scanf("%d", capacidadMin);

    printf("Alcance minimo (km): ");
    scanf("%d", alcanceMin);

    printf("Precio maximo (millones USD): ");
    scanf("%f", precioMax);

    printf("Precio maximo por hora de vuelo (USD): ");
    scanf("%f", precioHoraMax);
}

// Verifica si un avión cumple los criterios indicados
int cumpleCriterios(Avion a, char tipo[], char motor[], char ala[], char categoria[],
                    int capacidadMin, int alcanceMin, float precioMax, float precioHoraMax) {

    int cumple = 1;

    // Compara tipo si se especificó
    if (strcmp(tipo, "") != 0 && strcmp(a.tipo, tipo) != 0) {
        cumple = 0;
    }

    // Compara motor si se especificó
    if (strcmp(motor, "") != 0 && strcmp(a.motor, motor) != 0) {
        cumple = 0;
    }

    // Compara ala si se especificó
    if (strcmp(ala, "") != 0 && strcmp(a.ala, ala) != 0) {
        cumple = 0;
    }

    // Compara categoria si se especificó
    if (strcmp(categoria, "") != 0 && strcmp(a.categoria, categoria) != 0) {
        cumple = 0;
    }

    // Compara valores numéricos
    if (a.capacidad < capacidadMin || a.alcance < alcanceMin ||
        a.precio > precioMax || a.precioHora > precioHoraMax) {
        cumple = 0;
    }

    return cumple;
}

// Imprime la información completa de un avión
void imprimirAvion(Avion a) {
    printf("- %s | Tipo: %s | Motor: %s | Ala: %s | Categoria: %s | Capacidad: %d | Alcance: %d km | Precio: %.2f M | Hora: $%.2f\n",
           a.nombre,
           a.tipo,
           a.motor,
           a.ala,
           a.categoria,
           a.capacidad,
           a.alcance,
           a.precio,
           a.precioHora);
}

// Verifica combinaciones inválidas de motor y ala
int seleccionInvalida(int motor_index, int ala_index) {
    if (motor_index == 2) { // Helice
        if (ala_index == 2 || ala_index == 3) return 1; // Delta o Variable
    }
    if (motor_index == 1 || motor_index == 3) { // Turbofan o Reactivo
        if (ala_index == 4) return 1; // Tandem
    }
    return 0;
}

int main() {
    Avion aviones[MAX_AVIONES];  // Arreglo que almacena los aviones
    int total = cargarAviones(aviones, "aviones.txt");  // Carga desde el archivo
    if (total == 0) return 1;

    printf("=== Buscador de Aviones ===\n");

    // Listas de opciones
    char tipos[4][20] = {"", "Comercial", "Privado", "Militar"};
    char motores[4][20] = {"", "Turbofan", "Helice", "Reactivo"};
    char alas[5][20] = {"", "Fija", "Delta", "Variable", "Tandem"};
    char categorias[3][20] = {"", "Servicio", "Descontinuado"};

    int tipo_index, motor_index, ala_index, categoria_index;

    // Bucle hasta que la combinación motor-ala sea válida
    do {
        tipo_index = seleccionarOpcion("Seleccione el tipo de avion", tipos, 4);
        motor_index = seleccionarOpcion("Seleccione el tipo de motor", motores, 4);
        ala_index = seleccionarOpcion("Seleccione el tipo de ala", alas, 5);
        categoria_index = seleccionarOpcion("Seleccione la categoria", categorias, 3);

        if (seleccionInvalida(motor_index, ala_index)) {
            printf("Combinacion de motor y ala no valida\n");
        }
    } while (seleccionInvalida(motor_index, ala_index));

    // Solicita al usuario los criterios numéricos
    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Define filtros en texto vacío si el usuario seleccionó "Cualquiera"
    char tipoFiltro[20], motorFiltro[20], alaFiltro[20], categoriaFiltro[20];

    strcpy(tipoFiltro, tipos[tipo_index]);
    strcpy(motorFiltro, motores[motor_index]);
    strcpy(alaFiltro, alas[ala_index]);
    strcpy(categoriaFiltro, categorias[categoria_index]);

    // Búsqueda e impresión de resultados
    printf("=== Resultados de la busqueda ===\n");

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro, categoriaFiltro,
                            capacidadMin, alcanceMin, precioMax, precioHoraMax)) {
            imprimirAvion(aviones[i]);
            encontrados++;
        }
    }

    // Si no se encontraron resultados
    if (encontrados == 0) {
        printf("No se encontraron aviones que cumplan los criterios\n");
    } else {
        printf("Total encontrados: %d\n", encontrados);
    }

    return 0;
}
