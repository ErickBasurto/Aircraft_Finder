#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definimos el maximo numero de aviones que se pueden cargar
#define MAX_AVIONES 100

// Estructura que representa un avion con sus caracteristicas principales
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

// Funcion para mostrar la portada desde un archivo de texto
void mostrarPortada() {
    FILE *archivo = fopen("portada.txt", "r"); // Abrimos el archivo en modo lectura
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo de portada.\n");
        return;
    }

    char linea[200];
    // Leemos y mostramos linea por linea hasta el final del archivo
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%s", linea);
    }
    fclose(archivo); // Cerramos el archivo
    printf("\n");
}

// Compara una cadena de entrada con un filtro
// Devuelve 1 si son iguales o si el filtro es "Cualquiera", 0 en caso contrario
int compararCadena(char entrada[], char filtro[]) {
    if (strcmp(filtro, "Cualquiera") == 0) {
        return 1;
    }
    if (strcmp(entrada, filtro) == 0) {
        return 1;
    }
    return 0;
}

// Carga los aviones desde un archivo y devuelve el numero total cargado
int cargarAviones(Avion aviones[], char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "r"); // Abrimos el archivo de aviones
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    int i = 0;
    // Leemos cada linea del archivo y almacenamos los datos en la estructura
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

    fclose(archivo); // Cerramos el archivo
    return i;
}

// Permite al usuario seleccionar una opcion de un conjunto de opciones
int seleccionarOpcion(char mensaje[], char opciones[][20], int cantidad) {
    int opcion = -1;
    while (opcion < 1 || opcion > cantidad) {
        printf("\n%s\n", mensaje);
        for (int i = 0; i < cantidad; i++) {
            printf("%i %s\n", i + 1, opciones[i]);
        }
        printf("Selecciona una opcion: ");
        scanf("%i", &opcion);
        if (opcion < 1 || opcion > cantidad) {
            printf("\nOpcion invalida\n");
        }
    }
    return opcion - 1; // Restamos 1 para usarlo como indice de arreglo
}

// Solicita al usuario los parametros numericos de busqueda
void pedirParametrosBusqueda(int *capacidadMin, int *alcanceMin, float *precioMax, float *precioHoraMax) {
    printf("\nCapacidad minima de pasajeros (en caso de buscar drones = 0): ");
    scanf("%i", capacidadMin);
    while (*capacidadMin < 0) {
        printf("La capacidad minima no puede ser negativa. Intenta de nuevo.\n");
        printf("Capacidad minima: ");
        scanf("%i", capacidadMin);
    }

    printf("Alcance minimo posible del avion (km): ");
    scanf("%i", alcanceMin);
    while (*alcanceMin < 0) {
        printf("El alcance minimo no puede ser negativo. Intenta de nuevo.\n");
        printf("Alcance minimo (km): ");
        scanf("%i", alcanceMin);
    }

    printf("Precio maximo (millones USD): ");
    scanf("%f", precioMax);
    while (*precioMax < 0) {
        printf("El precio maximo no puede ser negativo. Intenta de nuevo.\n");
        printf("Precio maximo (millones USD): ");
        scanf("%f", precioMax);
    }

    printf("Precio maximo por hora de vuelo (USD): ");
    scanf("%f", precioHoraMax);
    while (*precioHoraMax < 0) {
        printf("El precio maximo de vuelo no puede ser negativo. Intenta de nuevo.\n");
        printf("Precio maximo por hora de vuelo (USD): ");
        scanf("%f", precioHoraMax);
    }
}

// Verifica si un avion cumple con todos los criterios de busqueda
int cumpleCriterios(Avion a, char tipo[], char motor[], char ala[], char categoria[], int capacidadMin, int alcanceMin, float precioMax, float precioHoraMax) {
    int cumple = 1;

    if (compararCadena(a.tipo, tipo) == 0) {
        cumple = 0;
    }
    if (compararCadena(a.motor, motor) == 0) {
        cumple = 0;
    }
    if (compararCadena(a.ala, ala) == 0) {
        cumple = 0;
    }
    if (compararCadena(a.categoria, categoria) == 0) {
        cumple = 0;
    }

    if (a.capacidad < capacidadMin) {
        cumple = 0;
    }
    if (a.alcance < alcanceMin) {
        cumple = 0;
    }
    if (a.precio > precioMax) {
        cumple = 0;
    }
    if (a.precioHora > precioHoraMax) {
        cumple = 0;
    }

    return cumple;
}

// Imprime la informacion de un avion
void imprimirAvion(Avion a) {
    printf("- %s | Tipo: %s | Motor: %s | Ala: %s | Categoria: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f M | Hora: $%.2f\n",
           a.nombre, a.tipo, a.motor, a.ala, a.categoria,
           a.capacidad, a.alcance, a.precio, a.precioHora);
}

// Determina si una combinacion de motor y ala es invalida
int seleccionInvalida(int motor_indice, int ala_indice) {
    if (motor_indice == 3 && ala_indice == 4) {
        return 1;
    }
    if (motor_indice == 1 && ala_indice == 4) {
        return 1;
    }
    if (motor_indice == 2 && (ala_indice == 2 || ala_indice == 3)) {
        return 1;
    }
    return 0;
}

// Funciones para ordenar aviones usando el algoritmo burbuja segun diferentes criterios
void ordenarPorPrecioAsc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].precio > arr[j + 1].precio) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorPrecioDesc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].precio < arr[j + 1].precio) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorPrecioHoraAsc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].precioHora > arr[j + 1].precioHora) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorPrecioHoraDesc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].precioHora < arr[j + 1].precioHora) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorAlcanceAsc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].alcance > arr[j + 1].alcance) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void ordenarPorAlcanceDesc(Avion arr[], int n) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].alcance < arr[j + 1].alcance) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    mostrarPortada(); // Mostramos la portada al inicio

    Avion aviones[MAX_AVIONES];
    int total = cargarAviones(aviones, "aviones.txt"); // Cargamos la base de datos
    if (total == 0) {
        return 1; // Si hubo error al cargar, salimos
    }

    printf("=== Buscador de Aviones ===\n");
    printf("El siguiente programa tiene como objetivo filtar aviones de una base de datos en base a las preferencias o parametros que busque el usuario para ello es necesario contestar el siguiente llenado de parametros.\n");

    // Arreglos de opciones disponibles
    char tipos[4][20] = {"Cualquiera", "Comercial", "Privado", "Militar"};
    char motores[4][20] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    char alas[5][20] = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};
    char categorias[3][20] = {"Cualquiera", "Servicio", "Descontinuado"};

    int tipo_indice, motor_indice, ala_indice, categoria_indice;

    // Repetimos hasta que el usuario elija una combinacion valida
    do {
        tipo_indice = seleccionarOpcion("Seleccione el tipo de avion", tipos, 4);
        motor_indice = seleccionarOpcion("Seleccione el tipo de motor", motores, 4);
        ala_indice = seleccionarOpcion("Seleccione el tipo de ala", alas, 5);
        categoria_indice = seleccionarOpcion("Seleccione la categoria (Servicio = Avion aun en servicio,  Descontinuado = El avion ya ha sido descontinuado)", categorias, 3);

        if (seleccionInvalida(motor_indice, ala_indice) == 1) {
            printf("Combinacion de motor y ala no valida\n");
        }
    } while (seleccionInvalida(motor_indice, ala_indice) == 1);

    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Copiamos las opciones seleccionadas como filtros
    char tipoFiltro[20], motorFiltro[20], alaFiltro[20], categoriaFiltro[20];
    strcpy(tipoFiltro, tipos[tipo_indice]);
    strcpy(motorFiltro, motores[motor_indice]);
    strcpy(alaFiltro, alas[ala_indice]);
    strcpy(categoriaFiltro, categorias[categoria_indice]);

    // Filtramos los aviones segun los criterios
    Avion filtrados[MAX_AVIONES];
    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro, categoriaFiltro,
                            capacidadMin, alcanceMin, precioMax, precioHoraMax)) {
            filtrados[encontrados++] = aviones[i];
        }
    }

    if (encontrados == 0) {
        printf("\nNo se encontraron aviones con esas especificaciones.\n");
        return 0;
    } else {
        printf("\nSe encontraron aviones con tus parametros.\n");
    }

    // Permite ordenar los resultados segun el criterio elegido
    int ordenIndice = -1;
    printf("\nComo desea ordenar los resultados?\n");
    printf("1. Precio (menor a mayor)\n");
    printf("2. Precio (mayor a menor)\n");
    printf("3. Precio por hora de vuelo (menor a mayor)\n");
    printf("4. Precio por hora de vuelo (mayor a menor)\n");
    printf("5. Distancia (menor a mayor)\n");
    printf("6. Distancia (mayor a menor)\n");
    printf("7. Sin orden (por orden alfabetico)\n");

    while (ordenIndice < 1 || ordenIndice > 7) {
        printf("Selecciona una opcion (1-7): ");
        scanf("%i", &ordenIndice);
        if (ordenIndice < 1 || ordenIndice > 7) {
            printf("Opcion invalida. Intenta de nuevo.\n");
        }
    }

    // Ejecutamos el tipo de ordenamiento elegido
    switch (ordenIndice) {
        case 1: {
            ordenarPorPrecioAsc(filtrados, encontrados);
            break;
        }
        case 2: {
            ordenarPorPrecioDesc(filtrados, encontrados);
            break;
        }
        case 3: {
            ordenarPorPrecioHoraAsc(filtrados, encontrados);
            break;
        }
        case 4: {
            ordenarPorPrecioHoraDesc(filtrados, encontrados);
            break;
        }
        case 5: {
            ordenarPorAlcanceAsc(filtrados, encontrados);
            break;
        }
        case 6: {
            ordenarPorAlcanceDesc(filtrados, encontrados);
            break;
        }
        default: {
            break; // Sin orden
        }
    }

    // Imprimimos los resultados
    printf("\n=== Resultados de la busqueda ===\n");
    for (int i = 0; i < encontrados; i++) {
        imprimirAvion(filtrados[i]);
    }
    printf("\nTotal encontrados: %i\n", encontrados);

    return 0;
}
