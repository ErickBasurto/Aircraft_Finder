#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 100  // Número máximo de aviones

// Estructura que representa un avión
typedef struct {
    char nombre[100];
    int capacidad;
    int alcance;
    float precio;
    char tipo[20];
    char motor[20];
    float precioHora;
    char ala[20];
    char categoria[20];
} Avion;

// Carga los aviones desde un archivo
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

// Muestra un menú y retorna la opción seleccionada
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

    return opcion - 1;
}

// Pide los criterios numéricos de búsqueda
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

// Verifica si un avión cumple los criterios dados
int cumpleCriterios(Avion a, char tipo[], char motor[], char ala[], char categoria[],
                    int capacidadMin, int alcanceMin, float precioMax, float precioHoraMax) {
    int cumple = 1;

    // Comparaciones solo si no es "Cualquiera"
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

    // Comparaciones numéricas
    if (a.capacidad < capacidadMin || a.alcance < alcanceMin || a.precio > precioMax || a.precioHora > precioHoraMax) {
        cumple = 0;
    }

    return cumple;
}

// Imprime la información completa del avión
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

// Verifica combinaciones inválidas entre motor y ala
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
    Avion aviones[MAX_AVIONES];
    int total = cargarAviones(aviones, "aviones.txt");
    if (total == 0) return 1;

    printf("=== Buscador de Aviones ===\n");

    // Opciones con "Cualquiera" como primera opción
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
    } while (seleccionInvalida(motor_index, ala_index));

    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Los filtros ahora siempre contienen texto legible, incluyendo "Cualquiera"
    char tipoFiltro[20], motorFiltro[20], alaFiltro[20], categoriaFiltro[20];
    strcpy(tipoFiltro, tipos[tipo_index]);
    strcpy(motorFiltro, motores[motor_index]);
    strcpy(alaFiltro, alas[ala_index]);
    strcpy(categoriaFiltro, categorias[categoria_index]);

    printf("=== Resultados de la busqueda ===\n");

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro, categoriaFiltro,
                            capacidadMin, alcanceMin, precioMax, precioHoraMax)) {
            imprimirAvion(aviones[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No se encontraron aviones que cumplan los criterios\n");
    } else {
        printf("Total encontrados: %d\n", encontrados);
    }

    return 0;
}
