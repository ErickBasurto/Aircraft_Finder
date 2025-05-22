#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 100

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

// Función que compara una cadena con el filtro
// Devuelve 1 si es "Cualquiera" o si coincide exactamente, si no, devuelve 0
int compararCadena(char entrada[], char filtro[]) {
    if (strcmp(filtro, "Cualquiera") == 0) return 1;
    if (strcmp(entrada, filtro) == 0) return 1;
    return 0;
}

// Función para cargar aviones desde archivo
int cargarAviones(Avion aviones[], char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    int i = 0;
    while (!feof(archivo) && i < MAX_AVIONES) {
        fscanf(archivo, " %99[^,],%i,%i,%f,%19[^,],%19[^,],%f,%19[^,],%19[^\n]\n", //Evita carácteres fantasma
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

// Función para mostrar un menú y obtener una opción
int seleccionarOpcion(char mensaje[], char opciones[][20], int cantidad) {
    int opcion = -1;
    while (opcion < 1 || opcion > cantidad) {
        printf("%s\n", mensaje);
        for (int i = 0; i < cantidad; i++) {
            printf("%i %s\n", i + 1, opciones[i]);
        }
        printf("Selecciona una opcion: ");
        scanf("%i", &opcion);
        if (opcion < 1 || opcion > cantidad) {
            printf("Opcion invalida\n");
        }
    }
    return opcion - 1;
}

// Función para pedir parámetros numéricos
void pedirParametrosBusqueda(int *capacidadMin, int *alcanceMin, float *precioMax, float *precioHoraMax) {
    printf("Capacidad minima: ");
    scanf("%i", capacidadMin);
    if (*capacidadMin < 0){
        printf("La capacidad minima no puede ser negativa. Intenta de nuevo.\n\n");
        while (*capacidadMin < 0){
            printf("Capacidad minima: ");
            scanf("%i", capacidadMin);
        }
    }

    printf("Alcance minimo (km): ");
    scanf("%i", alcanceMin);
    if (*alcanceMin < 0){
        printf("El alcance minimo no puede ser negativo. Intenta de nuevo.\n\n");
        while (*capacidadMin < 0){
            printf("Capacidad minima: ");
            scanf("%i", alcanceMin);
        }
    }

    printf("Precio maximo (millones USD): ");
    scanf("%f", precioMax);
    if (*precioMax < 0){
        printf("El precio máximo no puede ser negativo. Intenta de nuevo.\n\n");
        while (*capacidadMin < 0){
            printf("Precio maximo (millones USD): ");
            scanf("%f", precioMax);
        }
    }

    printf("Precio maximo por hora de vuelo (USD): ");
    scanf("%f", precioHoraMax);
    if (*precioHoraMax < 0){
        printf("El precio maximo de vuelo no puede ser negativo. Intenta de nuevo.\n\n");
        while (*capacidadMin < 0){
            printf("Precio maximo por hora de vuelo (USD) : ");
            scanf("%f", precioHoraMax);
        }
    }
}

// Verifica si el avión cumple los criterios. Retorna 1 si cumple, 0 si no.
int cumpleCriterios(Avion a, char tipo[], char motor[], char ala[], char categoria[], int capacidadMin, int alcanceMin, float precioMax, float precioHoraMax) {
    int cumple = 1;

    // Comparación de texto
    if (compararCadena(a.tipo, tipo) == 0) cumple = 0;
    if (compararCadena(a.motor, motor) == 0) cumple = 0;
    if (compararCadena(a.ala, ala) == 0) cumple = 0;
    if (compararCadena(a.categoria, categoria) == 0) cumple = 0;

    // Comparación de números
    if (a.capacidad < capacidadMin) cumple = 0;
    if (a.alcance < alcanceMin) cumple = 0;
    if (a.precio > precioMax) cumple = 0;
    if (a.precioHora > precioHoraMax) cumple = 0;

    return cumple;
}

// Muestra la información del avión
void imprimirAvion(Avion a) {
    printf("- %s | Tipo: %s | Motor: %s | Ala: %s | Categoria: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f M | Hora: $%.2f\n",
           a.nombre, a.tipo, a.motor, a.ala, a.categoria,
           a.capacidad, a.alcance, a.precio, a.precioHora);
}

// Verifica combinaciones inválidas entre motor y ala
int seleccionInvalida(int motor_index, int ala_index) {
    if (motor_index == 3 && ala_index == 4) return 1;
    if (motor_index == 1 && ala_index == 4) return 1;
    if (motor_index == 2 && (ala_index == 2 || ala_index == 3)) return 1;
    return 0;
}

// Funciones de ordenamiento (Burbuja) por distintos criterios

void ordenarPorPrecioAsc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].precio > arr[j + 1].precio) {
                temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorPrecioDesc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].precio < arr[j + 1].precio) {
                temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorPrecioHoraAsc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].precioHora > arr[j + 1].precioHora) {
                temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorPrecioHoraDesc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].precioHora < arr[j + 1].precioHora) {
                temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorAlcanceAsc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].alcance > arr[j + 1].alcance) {
                temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorAlcanceDesc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].alcance < arr[j + 1].alcance) {
                temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    Avion aviones[MAX_AVIONES];
    int total = cargarAviones(aviones, "aviones.txt");
    if (total == 0) return 1;

    printf("=== Buscador de Aviones ===\n");

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

        if (seleccionInvalida(motor_index, ala_index) == 1) {
            printf("Combinacion de motor y ala no valida\n");
        }
    } while (seleccionInvalida(motor_index, ala_index) == 1);

    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Copiar texto del filtro
    char tipoFiltro[20], motorFiltro[20], alaFiltro[20], categoriaFiltro[20];
    for (int i = 0; i < 20; i++) {
        tipoFiltro[i] = tipos[tipo_index][i];
        motorFiltro[i] = motores[motor_index][i];
        alaFiltro[i] = alas[ala_index][i];
        categoriaFiltro[i] = categorias[categoria_index][i];
    }

    Avion filtrados[MAX_AVIONES];
    int encontrados = 0;

    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro, categoriaFiltro,
                            capacidadMin, alcanceMin, precioMax, precioHoraMax) == 1) {
            filtrados[encontrados++] = aviones[i];
        }
    }

    if (encontrados == 0){
        printf("No se encontraron  aviones con esas especificaciones.");
        return 0;
    }
    else{
        printf(("¡SE ENCONTRARON AVIONES CON TUS PARÁMETROS!"));
    }
//Opcion para ahorro de memoria.
//COlocar comparar encontrados
//Si encontrados =0 entonces no hace lo de esto // Parametro de no hacer
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

    switch (ordenIndex) {
        case 1: ordenarPorPrecioAsc(filtrados, encontrados); break;
        case 2: ordenarPorPrecioDesc(filtrados, encontrados); break;
        case 3: ordenarPorPrecioHoraAsc(filtrados, encontrados); break;
        case 4: ordenarPorPrecioHoraDesc(filtrados, encontrados); break;
        case 5: ordenarPorAlcanceAsc(filtrados, encontrados); break;
        case 6: ordenarPorAlcanceDesc(filtrados, encontrados); break;
        default: break;
    }

    printf("\n=== Resultados de la busqueda ===\n");
    if (encontrados !=0) {
            for (int i = 0; i < encontrados; i++) {
            imprimirAvion(filtrados[i]);
        }
        printf("\nTotal encontrados: %i\n", encontrados);
    }

    return 0;}
