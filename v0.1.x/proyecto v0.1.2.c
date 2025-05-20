#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 100  // Número máximo de aviones

// Estructura que representa un avión
typedef struct {
    char nombre[100];       // Nombre del avión
    int capacidad;          // Capacidad de pasajeros
    int alcance;            // Alcance en kilómetros
    float precio;           // Precio en millones de USD
    char tipo[20];          // Tipo de avión (Comercial, Privado, Militar)
    char motor[20];         // Tipo de motor (Turbofan, Hélice, Reactivo)
    float precioHora;       // Precio por hora de vuelo
    char ala[20];           // Tipo de ala (Fija, Delta, Variable, Tandem)
    char categoria[20];     // Categoría (Servicio, Descontinuado)
} Avion;

// Función para cargar aviones desde un archivo
int cargarAviones(Avion aviones[], char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    int i = 0;
    while (!feof(archivo) && i < MAX_AVIONES) {
        fscanf(archivo, " %99[^,],%i,%i,%f,%19[^,],%19[^,],%f,%19[^,],%19[^\n]\n",
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

// Función para mostrar un menú de selección y devolver el índice elegido
int seleccionarOpcion(char mensaje[], char opciones[][20], int cantidad) {
    int opcion = -1;

    while (opcion < 1 || opcion > cantidad) {
        printf("%s\n", mensaje);
        for (int i = 0; i < cantidad; i++) {
            printf("%i. %s\n", i + 1, opciones[i]);
        }
        printf("Selecciona una opcion: ");
        scanf("%i", &opcion);
        if (opcion < 1 || opcion > cantidad) {
            printf("Opcion invalida\n");
        }
    }

    return opcion - 1;
}

// Función para pedir los parámetros de búsqueda numéricos
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

// Función para verificar si un avión cumple con los criterios dados
int cumpleCriterios(Avion a, char tipo[], char motor[], char ala[], char categoria[], int capacidadMin, int alcanceMin, float precioMax, float precioHoraMax) {
    int cumple = 1;

    if (strcmp(tipo, "Cualquiera") != 0 && strcmp(a.tipo, tipo) != 0) {
        cumple = 0;
    }
    if (strcmp(motor, "Cualquiera") != 0 && strcmp(a.motor, motor) != 0) {
        cumple = 0;
    }
    if (strcmp(ala, "Cualquiera") != 0 && strcmp(a.ala, ala) != 0) {
        cumple = 0;
    }
    if (strcmp(categoria, "Cualquiera") != 0 && strcmp(a.categoria, categoria) != 0) {
        cumple = 0;
    }

    if (a.capacidad < capacidadMin || a.alcance < alcanceMin || a.precio > precioMax || a.precioHora > precioHoraMax) {
        cumple = 0;
    }

    return cumple;
}

// Función para imprimir los detalles de un avión
void imprimirAvion(Avion a) {
    printf("- %s | Tipo: %s | Motor: %s | Ala: %s | Categoria: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f M | Hora: $%.2f\n",
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

// Función para verificar combinaciones inválidas entre motor y ala
int seleccionInvalida(int motor_index, int ala_index) {
    if (motor_index == 2) {
        if (ala_index == 2 || ala_index == 3) return 1;
    }
    if (motor_index == 1 || motor_index == 3) {
        if (ala_index == 4) return 1;
    }
    return 0;
}

int main() {
    Avion aviones[MAX_AVIONES];  // Arreglo de aviones
    int total = cargarAviones(aviones, "aviones.txt");  // Carga de archivo
    if (total == 0) return 1;

    printf("=== Buscador de Aviones ===\n");

    // Listas de opciones para cada criterio
    char tipos[4][20] = {"Cualquiera", "Comercial", "Privado", "Militar"};
    char motores[4][20] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    char alas[5][20] = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};
    char categorias[3][20] = {"Cualquiera", "Servicio", "Descontinuado"};

    int tipo_index, motor_index, ala_index, categoria_index;

    do {
        tipo_index = seleccionarOpcion("Seleccione el tipo de avion", tipos, 4);
        motor_index = seleccionarOpcion("Seleccione el tipo de motor", motores, 4);
        ala_index = seleccionarOpcion("Seleccione el tipo de ala", alas, 5);
        categoria_index = seleccionarOpcion("Seleccione la categoria", categorias, 3);

        if (seleccionInvalida(motor_index, ala_index)) {
            printf("Combinacion de motor y ala no valida\n");
        }
    } while (seleccionInvalida(motor_index, ala_index)==1);

    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Se copian los filtros seleccionados a variables para uso posterior
    char tipoFiltro[20], motorFiltro[20], alaFiltro[20], categoriaFiltro[20];
    strcpy(tipoFiltro, tipos[tipo_index]);
    strcpy(motorFiltro, motores[motor_index]);
    strcpy(alaFiltro, alas[ala_index]);
    strcpy(categoriaFiltro, categorias[categoria_index]);

    printf("\n=== Resultados de la busqueda ===\n");

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro, categoriaFiltro, capacidadMin, alcanceMin, precioMax, precioHoraMax)==1) {
            imprimirAvion(aviones[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No se encontraron aviones que cumplan los criterios\n");
    } else {
        printf("\nTotal encontrados: %i\n", encontrados);
    }

    return 0;
}
