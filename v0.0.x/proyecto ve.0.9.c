#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AVIONES 100  // Numero maximo de aviones a manejar

// Estructura que representa un avion
typedef struct {
    char nombre[100];  // Nombre del avion
    int capacidad;     // Numero de pasajeros
    int alcance;       // Alcance en kilometros
    float precio;      // Precio en millones de USD
    char tipo[20];     // Tipo de avion: Comercial, Privado, Militar
    char motor[20];    // Tipo de motor: Turbofan, Helice, Reactivo
    float precioHora;  // Precio por hora de vuelo en USD
    char ala[20];      // Tipo de ala: Fija, Delta, Variable, Tandem
    char categoria[20]; // Categoria del avion: Servicio, Descontinuado
} Avion;

// Funcion para cargar aviones desde un archivo de texto
int cargarAviones(Avion aviones[], const char *nombreArchivo) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    int i = 0;
    while (!feof(archivo) && i < MAX_AVIONES) {
        // Leer datos del archivo y asignarlos a la estructura
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

// Funcion para seleccionar una opcion de una lista predefinida
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
    *resultado = opcion - 1;
}

// Funcion para solicitar parametros de busqueda
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

// Funcion para verificar si un avion cumple con los criterios de busqueda
int cumpleCriterios(Avion a, const char *tipo, const char *motor, const char *ala, const char *categoria,
                    int capacidadMin, int alcanceMin, float precioMax, float precioHoraMax) {
    return
        (tipo == NULL || strcmp(a.tipo, tipo) == 0) &&
        (motor == NULL || strcmp(a.motor, motor) == 0) &&
        (ala == NULL || strcmp(a.ala, ala) == 0) &&
        (categoria == NULL || strcmp(a.categoria, categoria) == 0) &&
        a.capacidad >= capacidadMin &&
        a.alcance >= alcanceMin &&
        a.precio <= precioMax &&
        a.precioHora <= precioHoraMax;
}

// Funcion para imprimir los detalles de un avion
void imprimirAvion(Avion a) {
    printf("- %s | Tipo: %s | Motor: %s | Ala: %s | Categoria: %s | Capacidad: %i | Alcance: %i km | Precio: %.2f M | Hora: $%.2f\n",
           a.nombre, a.tipo, a.motor, a.ala, a.categoria, a.capacidad, a.alcance, a.precio, a.precioHora);
}

// Funcion para validar combinaciones de motor y tipo de ala
int SeleccionInvalida(int motor_index, int ala_index) {
    if (motor_index == 2) {
        if (ala_index == 2 || ala_index == 3) return 1;
    }
    if (motor_index == 1 || motor_index == 3) {
        if (ala_index == 4) return 1;
    }
    return 0;
}

int main() {
    Avion aviones[MAX_AVIONES];  // Arreglo para almacenar aviones
    int total = cargarAviones(aviones, "aviones.txt");  // Cargar aviones desde archivo
    if (total == 0) return 1;

    printf("=== Buscador de Aviones ===\n");

    // Opciones para tipo de avion, motor, ala y categoria
    const char *tipos[] = {"Cualquiera", "Comercial", "Privado", "Militar"};
    const char *motores[] = {"Cualquiera", "Turbofan", "Helice", "Reactivo"};
    const char *alas[] = {"Cualquiera", "Fija", "Delta", "Variable", "Tandem"};
    const char *categorias[] = {"Cualquiera", "Servicio", "Descontinuado"};

    int tipo_index, motor_index, ala_index, categoria_index;

    do {
        seleccionarOpcion("Seleccione el tipo de avion:", tipos, 4, &tipo_index);
        seleccionarOpcion("Seleccione el tipo de motor:", motores, 4, &motor_index);
        seleccionarOpcion("Seleccione el tipo de ala:", alas, 5, &ala_index);
        seleccionarOpcion("Seleccione la categoria:", categorias, 3, &categoria_index);

        if (SeleccionInvalida(motor_index, ala_index)) {
            printf("\nCombinacion de motor y tipo de ala no es valida. Por favor, selecciona nuevamente.\n\n");
        }
    } while (SeleccionInvalida(motor_index, ala_index));

    int capacidadMin, alcanceMin;
    float precioMax, precioHoraMax;
    pedirParametrosBusqueda(&capacidadMin, &alcanceMin, &precioMax, &precioHoraMax);

    const char *tipoFiltro = (tipo_index == 0) ? NULL : tipos[tipo_index];
    const char *motorFiltro = (motor_index == 0) ? NULL : motores[motor_index];
    const char *alaFiltro = (ala_index == 0) ? NULL : alas[ala_index];
    const char *categoriaFiltro = (categoria_index == 0) ? NULL : categorias[categoria_index];

    printf("\n=== Resultados de la busqueda ===\n");

    int encontrados = 0;
    for (int i = 0; i < total; i++) {
        if (cumpleCriterios(aviones[i], tipoFiltro, motorFiltro, alaFiltro, categoriaFiltro,
                            capacidadMin, alcanceMin, precioMax, precioHoraMax)) {
            imprimirAvion(aviones[i]);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No se encontraron aviones que cumplan los criterios.\n");
    } else {
        printf("\nTotal encontrados: %i\n", encontrados);
    }

    return 0;
}
