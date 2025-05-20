#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 100  // Numero maximo de aviones que se pueden cargar

// Definicion de la estructura Avion
typedef struct {
    char nombre[100];    // Nombre del avion
    int capacidad;       // Cantidad de pasajeros
    int alcance;         // Distancia maxima en kilometros
    float precio;        // Precio en millones de dolares
    char tipo[20];       // Tipo (Comercial, Privado, Militar)
    char motor[20];      // Tipo de motor (Turbofan, Helice, etc.)
    float precioHora;    // Costo por hora de vuelo
    char ala[20];        // Tipo de ala (Fija, Delta, etc.)
    char categoria[20];  // Categoria (Servicio, Descontinuado)
} Avion;

// Carga los aviones desde un archivo de texto y los guarda en un arreglo
int cargarAviones(Avion aviones[], const char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "r");  // Abre el archivo
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    int i = 0;
    // Lee los datos de cada avion hasta el final del archivo
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

    fclose(archivo);  // Cierra el archivo
    return i;         // Devuelve el total de aviones cargados
}

// Muestra un menú con opciones y devuelve el índice elegido por el usuario
int seleccionarOpcion(const char mensaje[], char opciones[][20], int cantidad) {
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
    return opcion - 1;  // Devuelve el indice correspondiente
}

// Solicita al usuario los filtros numéricos para la búsqueda
void pedirParametrosBusqueda(int *capacidad, int *alcance, float *precio, float *precioHora) {
    printf("Capacidad minima: ");
    scanf("%i", capacidad);

    printf("Alcance minimo (km): ");
    scanf("%i", alcance);

    printf("Precio maximo (millones USD): ");
    scanf("%f", precio);

    printf("Precio maximo por hora de vuelo (USD): ");
    scanf("%f", precioHora);
}

// Compara una cadena con el filtro (permite "Cualquiera")
int compararCadena(const char entrada[], const char filtro[]) {
    return strcmp(filtro, "Cualquiera") == 0 || strcmp(entrada, filtro) == 0; //Devuelve 0 si se cumple
}

// Verifica si un avion cumple con todos los criterios seleccionados
int cumpleCriterios(Avion a, const char tipo[], const char motor[], const char ala[], const char categoria[],
                    int capacidadMin, int alcanceMin, float precioMax, float precioHoraMax) {
    return compararCadena(a.tipo, tipo) &&
           compararCadena(a.motor, motor) &&
           compararCadena(a.ala, ala) &&
           compararCadena(a.categoria, categoria) &&
           a.capacidad >= capacidadMin &&
           a.alcance >= alcanceMin &&
           a.precio <= precioMax &&
           a.precioHora <= precioHoraMax;
}

// Imprime todos los datos de un avion
void imprimirAvion(Avion a) {
    printf("- %s | Tipo: %s | Motor: %s | Ala: %s | Categoria: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f M | Hora: $%.2f\n",
           a.nombre, a.tipo, a.motor, a.ala, a.categoria, a.capacidad, a.alcance, a.precio, a.precioHora);
}

// Verifica si la combinación de motor y ala seleccionada es invalida
int seleccionInvalida(int motor, int ala) {
    if ((motor == 3 && ala == 4) || (motor == 1 && ala == 4) || (motor == 2 && (ala == 2 || ala == 3))) {
        return 1;
    }
    return 0;
}

// Ordena los aviones filtrados usando un algoritmo de burbuja segun el criterio elegido
void ordenar(Avion arr[], int n, int modo) {
    Avion temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int swap = 0;
            // Modo de ordenamiento segun el valor elegido por el usuario
            if (modo == 1 && arr[j].precio > arr[j + 1].precio) swap = 1;
            else if (modo == 2 && arr[j].precio < arr[j + 1].precio) swap = 1;
            else if (modo == 3 && arr[j].precioHora > arr[j + 1].precioHora) swap = 1;
            else if (modo == 4 && arr[j].precioHora < arr[j + 1].precioHora) swap = 1;
            else if (modo == 5 && arr[j].alcance > arr[j + 1].alcance) swap = 1;
            else if (modo == 6 && arr[j].alcance < arr[j + 1].alcance) swap = 1;

            if (swap) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Copia un filtro seleccionado desde el arreglo de opciones al filtro final
void copiarFiltro(char destino[], char origen[][20], int indice) {
    for (int i = 0; i < 20; i++) {
        destino[i] = origen[indice][i];
        if (origen[indice][i] == '\0') break;
    }
}

// Funcion principal del programa
int main() {
    Avion aviones[MAX_AVIONES];  // Arreglo para guardar todos los aviones cargados

    int total = cargarAviones(aviones, "aviones.txt");  // Carga los datos desde el archivo
    if (total == 0) return 1;

    printf("=== Buscador de Aviones ===\n");

    // Opciones de menu
    char tipos[4][20] = {"Cualquiera", "Comercial", "Privado", "Militar"};
    char motores[4][20] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    char alas[5][20] = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};
    char categorias[3][20] = {"Cualquiera", "Servicio", "Descontinuado"};

    int tipoIndex, motorIndex, alaIndex, categoriaIndex;

    // Pide opciones al usuario hasta que la combinacion sea valida
    do {
        tipoIndex = seleccionarOpcion("Seleccione el tipo de avion", tipos, 4);
        motorIndex = seleccionarOpcion("Seleccione el tipo de motor", motores, 4);
        alaIndex = seleccionarOpcion("Seleccione el tipo de ala", alas, 5);
        categoriaIndex = seleccionarOpcion("Seleccione la categoria", categorias, 3);

        if (seleccionInvalida(motorIndex, alaIndex)) {
            printf("Combinacion de motor y ala no valida\n");
        }
    } while (seleccionInvalida(motorIndex, alaIndex));

    // Solicita al usuario los valores numericos para la busqueda
    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Copia los filtros seleccionados en variables individuales
    char tipoFiltro[20], motorFiltro[20], alaFiltro[20], categoriaFiltro[20];
    copiarFiltro(tipoFiltro, tipos, tipoIndex);
    copiarFiltro(motorFiltro, motores, motorIndex);
    copiarFiltro(alaFiltro, alas, alaIndex);
    copiarFiltro(categoriaFiltro, categorias, categoriaIndex);

    // Busca los aviones que cumplen todos los criterios
    Avion filtrados[MAX_AVIONES];
    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro, categoriaFiltro,
                            capacidadMin, alcanceMin, precioMax, precioHoraMax)) {
            filtrados[encontrados++] = aviones[i];
        }
    }

    // Pregunta al usuario cómo ordenar los resultados
    int ordenIndex = -1;
    printf("\nComo desea ordenar los resultados?\n");
    printf("1. Precio (menor a mayor)\n");
    printf("2. Precio (mayor a menor)\n");
    printf("3. Precio por hora (menor a mayor)\n");
    printf("4. Precio por hora (mayor a menor)\n");
    printf("5. Distancia (menor a mayor)\n");
    printf("6. Distancia (mayor a menor)\n");
    printf("7. Sin orden\n");

    while (ordenIndex < 1 || ordenIndex > 7) {
        printf("Selecciona una opcion (1-7): ");
        scanf("%i", &ordenIndex);
        if (ordenIndex < 1 || ordenIndex > 7) {
            printf("Opcion invalida. Intenta de nuevo.\n");
        }
    }

    // Ordena los resultados si se selecciono una opcion de ordenamiento
    if (ordenIndex != 7) {
        ordenar(filtrados, encontrados, ordenIndex);
    }

    // Imprime los resultados encontrados
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
