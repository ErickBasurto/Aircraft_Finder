#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 100  // Numero maximo de aviones

// Estructura que representa un avion
typedef struct {
    char nombre[100];       // Nombre del avion
    int capacidad;          // Capacidad de pasajeros
    int alcance;            // Alcance en kilometros
    float precio;           // Precio en millones de USD
    char tipo[20];          // Tipo de avion (Comercial, Privado, Militar)
    char motor[20];         // Tipo de motor (Turbofan, Helice, Reactivo)
    float precioHora;       // Precio por hora de vuelo
    char ala[20];           // Tipo de ala (Fija, Delta, Variable, Tandem)
    char categoria[20];     // Categoria (Servicio, Descontinuado)
} Avion;

// Funcion para cargar aviones desde un archivo
int cargarAviones(Avion aviones[], char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    int i = 0;
    // Leer hasta el fin del archivo o hasta MAX_AVIONES
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

// Funcion para mostrar un menu de seleccion y devolver el indice elegido
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
    return opcion - 1;  // Retornar indice basado en 0
}

// Funcion para pedir los parametros de busqueda numericos
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

// Funcion para verificar si un avion cumple con los criterios dados
int cumpleCriterios(Avion a, char tipo[], char motor[], char ala[], char categoria[], int capacidadMin, int alcanceMin, float precioMax, float precioHoraMax) {
    int cumple = 1;

    // Verificar cada filtro, si no es "Cualquiera" debe coincidir
    if (strcmp(tipo, "Cualquiera") != 0 && strcmp(a.tipo, tipo) != 0) cumple = 0;
    if (strcmp(motor, "Cualquiera") != 0 && strcmp(a.motor, motor) != 0) cumple = 0;
    if (strcmp(ala, "Cualquiera") != 0 && strcmp(a.ala, ala) != 0) cumple = 0;
    if (strcmp(categoria, "Cualquiera") != 0 && strcmp(a.categoria, categoria) != 0) cumple = 0;

    // Verificar criterios numericos
    if (a.capacidad < capacidadMin || a.alcance < alcanceMin || a.precio > precioMax || a.precioHora > precioHoraMax) cumple = 0;

    return cumple;
}

// Funcion para imprimir los detalles de un avion
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

// Funcion que verifica si una combinacion de motor y ala es invalida segun logica de diseño
int seleccionInvalida(int motor_index, int ala_index) {
    // Motor Reactivo no se usa con ala Tandem
    if (motor_index == 3 && ala_index == 4) return 1;

    // Motor Turbofan tampoco se combina con Tandem
    if (motor_index == 1 && ala_index == 4) return 1;

    // Motor Hélice no se combina con Delta ni Variable
    if (motor_index == 2 && (ala_index == 2 || ala_index == 3)) return 1;

    return 0; // Combinacion valida
}


// Ordenamiento burbuja por precio ascendente
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

// Ordenamiento burbuja por precio descendente
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

// Ordenamiento por precio hora ascendente
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

// Ordenamiento por precio hora descendente
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

// Ordenamiento por alcance ascendente
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

// Ordenamiento por alcance descendente
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
    Avion aviones[MAX_AVIONES];  // Arreglo principal de aviones
    int total = cargarAviones(aviones, "aviones.txt");
    if (total == 0) return 1;

    printf("=== Buscador de Aviones ===\n");

    // Listas de opciones para filtros
    char tipos[4][20] = {"Cualquiera", "Comercial", "Privado", "Militar"};
    char motores[4][20] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    char alas[5][20] = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};
    char categorias[3][20] = {"Cualquiera", "Servicio", "Descontinuado"};

    int tipo_index, motor_index, ala_index, categoria_index;

    // Seleccion de filtros con verificacion de combinaciones invalidas
    do {
        tipo_index = seleccionarOpcion("Seleccione el tipo de avion", tipos, 4);
        motor_index = seleccionarOpcion("Seleccione el tipo de motor", motores, 4);
        ala_index = seleccionarOpcion("Seleccione el tipo de ala", alas, 5);
        categoria_index = seleccionarOpcion("Seleccione la categoria", categorias, 3);

        if (seleccionInvalida(motor_index, ala_index)) {
            printf("Combinacion de motor y ala no valida\n");
        }
    } while (seleccionInvalida(motor_index, ala_index));

    // Solicitar parametros numericos
    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    // Copiar filtros
    char tipoFiltro[20], motorFiltro[20], alaFiltro[20], categoriaFiltro[20];
    strcpy(tipoFiltro, tipos[tipo_index]);
    strcpy(motorFiltro, motores[motor_index]);
    strcpy(alaFiltro, alas[ala_index]);
    strcpy(categoriaFiltro, categorias[categoria_index]);

    // Aplicar filtros
    Avion filtrados[MAX_AVIONES];
    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro, categoriaFiltro, capacidadMin, alcanceMin, precioMax, precioHoraMax)) {
            filtrados[encontrados++] = aviones[i];
        }
    }

    // Elegir tipo de orden
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

    // Aplicar orden si se selecciono
    switch (ordenIndex) {
        case 1: ordenarPorPrecioAsc(filtrados, encontrados); break;
        case 2: ordenarPorPrecioDesc(filtrados, encontrados); break;
        case 3: ordenarPorPrecioHoraAsc(filtrados, encontrados); break;
        case 4: ordenarPorPrecioHoraDesc(filtrados, encontrados); break;
        case 5: ordenarPorAlcanceAsc(filtrados, encontrados); break;
        case 6: ordenarPorAlcanceDesc(filtrados, encontrados); break;
        default: break; // Sin orden
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
