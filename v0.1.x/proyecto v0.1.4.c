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

    if (strcmp(tipo, "Cualquiera") != 0 && strcmp(a.tipo, tipo) != 0) cumple = 0;
    if (strcmp(motor, "Cualquiera") != 0 && strcmp(a.motor, motor) != 0) cumple = 0;
    if (strcmp(ala, "Cualquiera") != 0 && strcmp(a.ala, ala) != 0) cumple = 0;
    if (strcmp(categoria, "Cualquiera") != 0 && strcmp(a.categoria, categoria) != 0) cumple = 0;

    if (a.capacidad < capacidadMin || a.alcance < alcanceMin || a.precio > precioMax || a.precioHora > precioHoraMax) cumple = 0;

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
        if (ala_index == 2 || ala_index == 3) return 1; // Reactivo con Delta o Variable
    }
    if (motor_index == 1 || motor_index == 3) {
        if (ala_index == 4) return 1; // Turbofan o Reactivo con Tandem
    }
    return 0;
}

// Funciones de comparación para qsort
int compararPrecioAsc(const void *a, const void *b) {
    float pa = ((Avion *)a)->precio;
    float pb = ((Avion *)b)->precio;
    return (pa > pb) - (pa < pb);
}

int compararPrecioDesc(const void *a, const void *b) {
    return compararPrecioAsc(b, a);
}

int compararPrecioHoraAsc(const void *a, const void *b) {
    float ha = ((Avion *)a)->precioHora;
    float hb = ((Avion *)b)->precioHora;
    return (ha > hb) - (ha < hb);
}

int compararPrecioHoraDesc(const void *a, const void *b) {
    return compararPrecioHoraAsc(b, a);
}

int compararAlcanceAsc(const void *a, const void *b) {
    int aa = ((Avion *)a)->alcance;
    int ab = ((Avion *)b)->alcance;
    return aa - ab;
}

int compararAlcanceDesc(const void *a, const void *b) {
    return compararAlcanceAsc(b, a);
}

int main() {
    Avion aviones[MAX_AVIONES];  // Arreglo de aviones
    int total = cargarAviones(aviones, "aviones.txt");  // Cargar archivo
    if (total == 0) return 1;

    printf("=== Buscador de Aviones ===\n");

    // Listas de opciones
    char tipos[4][20] = {"Cualquiera", "Comercial", "Privado", "Militar"};
    char motores[4][20] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    char alas[5][20] = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};
    char categorias[3][20] = {"Cualquiera", "Servicio", "Descontinuado"};

    int tipo_index, motor_index, ala_index, categoria_index;

    // Selección de filtros con validación de combinaciones inválidas
    do {
        tipo_index = seleccionarOpcion("Seleccione el tipo de avion", tipos, 4);
        motor_index = seleccionarOpcion("Seleccione el tipo de motor", motores, 4);
        ala_index = seleccionarOpcion("Seleccione el tipo de ala", alas, 5);
        categoria_index = seleccionarOpcion("Seleccione la categoria", categorias, 3);

        if (seleccionInvalida(motor_index, ala_index)) {
            printf("Combinacion de motor y ala no valida\n");
        }
    } while (seleccionInvalida(motor_index, ala_index));

    // Parámetros numéricos
    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Copiar filtros a variables
    char tipoFiltro[20], motorFiltro[20], alaFiltro[20], categoriaFiltro[20];
    strcpy(tipoFiltro, tipos[tipo_index]);
    strcpy(motorFiltro, motores[motor_index]);
    strcpy(alaFiltro, alas[ala_index]);
    strcpy(categoriaFiltro, categorias[categoria_index]);

    // Filtrar resultados en un nuevo arreglo
    Avion filtrados[MAX_AVIONES];
    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro, categoriaFiltro, capacidadMin, alcanceMin, precioMax, precioHoraMax)) {
            filtrados[encontrados++] = aviones[i];
        }
    }

    // Preguntar por orden usando printf
    int ordenIndex = -1;
    printf("\nComo desea ordenar los resultados?\n");
    printf("1. Precio (menor a mayor)\n");
    printf("2. Precio (mayor a menor)\n");
    printf("3. Precio por hora (menor a mayor)\n");
    printf("4. Precio por hora (mayor a menor)\n");
    printf("5. Distancia (menor a mayor)\n");
    printf("6. Distancia (mayor a menor)\n");
    printf("7. Sin orden (por orden del archivo)\n");

    while (ordenIndex < 1 || ordenIndex > 7) {
        printf("Selecciona una opcion (1-7): ");
        scanf("%i", &ordenIndex);
        if (ordenIndex < 1 || ordenIndex > 7) {
            printf("Opcion invalida. Intenta de nuevo.\n");
        }
    }
    ordenIndex -= 1; // Ajustar a índice 0-based

    // Aplicar orden si corresponde
    switch (ordenIndex) {
        case 0: qsort(filtrados, encontrados, sizeof(Avion), compararPrecioAsc); break;
        case 1: qsort(filtrados, encontrados, sizeof(Avion), compararPrecioDesc); break;
        case 2: qsort(filtrados, encontrados, sizeof(Avion), compararPrecioHoraAsc); break;
        case 3: qsort(filtrados, encontrados, sizeof(Avion), compararPrecioHoraDesc); break;
        case 4: qsort(filtrados, encontrados, sizeof(Avion), compararAlcanceAsc); break;
        case 5: qsort(filtrados, encontrados, sizeof(Avion), compararAlcanceDesc); break;
        default: break; // Sin orden, ya vienen en orden alfabético
    }

    // Imprimir resultados
    printf("\n=== Resultados de la busqueda ===\n");
    if (encontrados == 0) {
        printf("No se encontraron aviones que cumplan los criterios\n");
    } else {
        for (int i = 0; i < encontrados; i++) {
            imprimirAvion(filtrados[i]);
        }
        printf("\nTotal encontrados: %i\n", encontrados);
    }

    return 0;
}
