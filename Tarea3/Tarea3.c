#include "tdas/extra.h"
#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_JUGADORES 2


// Constantes para direcciones
typedef enum { ARRIBA, ABAJO, IZQUIERDA, DERECHA, NINGUNA } Direccion;

// Conversión dirección <-> string (opcional para imprimir)
const char* dir_to_str(Direccion d) {
    switch(d) {
        case ARRIBA: return "Arriba";
        case ABAJO: return "Abajo";
        case IZQUIERDA: return "Izquierda";
        case DERECHA: return "Derecha";
        default: return "Ninguna";
    }
}

// Ítem
typedef struct Item {
    char nombre[50];
    int peso;
    int valor;
    struct Item* siguiente;
} Item;

// arco
typedef struct Conexion {
    Direccion direccion;
    struct Escenario* destino;
    struct Conexion* siguiente;
} Conexion;



// nodo
typedef struct Escenario {
    int id; // identificador único
    char descripcion[256];
    Item* items;               // lista de ítems en este escenario
    Conexion* conexiones;      // lista de conexiones a otros escenarios
    struct Escenario* siguiente;  // para mantener lista de escenarios en el grafo
} Escenario;

// Grafo
typedef struct {
    Escenario* escenarios; // lista de todos los escenarios
    Escenario* inicio;     // escenario inicial
    Escenario* salida;     // escenario final
} Grafo;
Grafo grafo = { NULL, NULL, NULL };

typedef struct ConexionTemporal {
    int origen;
    int arriba;
    int abajo;
    int izquierda;
    int derecha;
    struct ConexionTemporal* siguiente;
} ConexionTemporal;
ConexionTemporal* conexionesTemp = NULL;


typedef struct {
    Item* inventario;    // Lista enlazada de ítems recogidos
    int peso_total;      // Peso acumulado del inventario
    int puntaje;         // Puntaje acumulado por ítems
    int tiempo_restante; // Tiempo que queda para jugar
    Escenario* escenario_actual; // Escenario donde está el jugador
} Jugador;

Jugador jugadores[MAX_JUGADORES];
int jugador_actual = 0;

Escenario* buscarEscenarioPorId(int id) {
    Escenario* actual = grafo.escenarios;
    while (actual != NULL) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return NULL;
}

void agregarConexionTemporal(int origen, int arriba, int abajo, int izquierda, int derecha) {
    ConexionTemporal* c = malloc(sizeof(ConexionTemporal));
    c->origen = origen;
    c->arriba = arriba;
    c->abajo = abajo;
    c->izquierda = izquierda;
    c->derecha = derecha;
    c->siguiente = conexionesTemp;
    conexionesTemp = c;
}

Escenario* crearEscenario(int id, const char* descripcion) {
    Escenario* esc = malloc(sizeof(Escenario));
    esc->id = id;
    strncpy(esc->descripcion, descripcion, 255);
    esc->descripcion[255] = '\0';
    esc->items = NULL;
    esc->conexiones = NULL;
    esc->siguiente = NULL;

    // Insertar al inicio de la lista de escenarios del grafo
    esc->siguiente = grafo.escenarios;
    grafo.escenarios = esc;

    return esc;
}

int esUltimaStage(Escenario* actual) {
    if (actual == NULL || grafo.salida == NULL) return 0;
    return actual->id == grafo.salida->id;
}



void agregarConexion(Escenario* origen, Escenario* destino, Direccion dir) {
    Conexion* nueva = malloc(sizeof(Conexion));
    nueva->direccion = dir;
    nueva->destino = destino;
    nueva->siguiente = origen->conexiones;
    origen->conexiones = nueva;
}


void mostrarEstadoActual() {
    Jugador* jug = &jugadores[jugador_actual];
    Escenario* esc = jug->escenario_actual;
    printf("\n=== Turno del Jugador %d ===\n", jugador_actual + 1);
    if (!esc) {
        printf("Error: escenario actual no definido.\n");
        return;
    }
    printf("--- Escenario actual: %s ---\n", esc->descripcion);

    printf("Ítems disponibles:\n");
    if (esc->items == NULL) {
        printf("  (No hay ítems en este escenario)\n");
    } else {
        int idx = 1;
        for (Item* it = esc->items; it != NULL; it = it->siguiente, idx++) {
            printf("  %d) %s (Peso: %d, Valor: %d)\n", idx, it->nombre, it->peso, it->valor);
        }
    }

    printf("\nInventario (%d kg, %d pts):\n", jug->peso_total, jug->puntaje);
    if (jug->inventario == NULL) {
        printf("  (Inventario vacío)\n");
    } else {
        int idx = 1;
        for (Item* it = jug->inventario; it != NULL; it = it->siguiente, idx++) {
            printf("  %d) %s (Peso: %d, Valor: %d)\n", idx, it->nombre, it->peso, it->valor);
        }
    }

    printf("\nTiempo restante: %d\n", jug->tiempo_restante);

    printf("Direcciones disponibles:\n");
    if (esc->conexiones == NULL) {
        printf("  (No hay direcciones para avanzar)\n");
    } else {
        for (Conexion* c = esc->conexiones; c != NULL; c = c->siguiente) {
            printf("  - %s\n", dir_to_str(c->direccion));
        }
    }
}

void inicializarJugadores() {
    for (int i = 0; i < MAX_JUGADORES; i++) {
        jugadores[i].inventario = NULL;
        jugadores[i].peso_total = 0;
        jugadores[i].puntaje = 0;
        jugadores[i].tiempo_restante = 10;
        jugadores[i].escenario_actual = grafo.inicio;
    }
    jugador_actual = 0;
}

void liberarInventarios() {
    for (int i = 0; i < MAX_JUGADORES; i++) {
        Item* it = jugadores[i].inventario;
        while (it) {
            Item* siguiente = it->siguiente;
            free(it);
            it = siguiente;
        }
        jugadores[i].inventario = NULL;
    }
}

void reiniciarPartida() {
    liberarInventarios();
    inicializarJugadores();
    printf("Partida reiniciada.\n");
}

void conectarEscenarios() {
    ConexionTemporal* c = conexionesTemp;
    while (c != NULL) {
        Escenario* origen = buscarEscenarioPorId(c->origen);
        if (!origen) {
            c = c->siguiente;
            continue;
        }

        if (c->arriba != -1) {
            Escenario* destino = buscarEscenarioPorId(c->arriba);
            if (destino) agregarConexion(origen, destino, ARRIBA);
        }
        if (c->abajo != -1) {
            Escenario* destino = buscarEscenarioPorId(c->abajo);
            if (destino) agregarConexion(origen, destino, ABAJO);
        }
        if (c->izquierda != -1) {
            Escenario* destino = buscarEscenarioPorId(c->izquierda);
            if (destino) agregarConexion(origen, destino, IZQUIERDA);
        }
        if (c->derecha != -1) {
            Escenario* destino = buscarEscenarioPorId(c->derecha);
            if (destino) agregarConexion(origen, destino, DERECHA);
        }

        c = c->siguiente;
    }
}

// Prototipo antes de leer_escenarios


void agregarItem(Escenario* esc, const char* nombre, int valor, int peso) {
    Item* nuevo = malloc(sizeof(Item));
    strncpy(nuevo->nombre, nombre, 49);
    nuevo->nombre[49] = '\0';
    nuevo->valor = valor;
    nuevo->peso = peso;
    nuevo->siguiente = esc->items;
    esc->items = nuevo;
}

void recogerItems() {
    Jugador* jug = &jugadores[jugador_actual];
    Escenario* esc = jug->escenario_actual;
    if (!esc || !esc->items) {
        printf("No hay ítems para recoger aquí.\n");
        return;
    }

    int idx = 1;
    printf("Ítems disponibles:\n");
    for (Item* it = esc->items; it != NULL; it = it->siguiente, idx++) {
        printf("  %d) %s (Peso: %d, Valor: %d)\n", idx, it->nombre, it->peso, it->valor);
    }
    printf("Ingrese el número del ítem a recoger (0 para cancelar): ");
    int opcion;
    scanf("%d", &opcion);
    if (opcion <= 0) return;

    // Buscar el ítem seleccionado
    Item* prev = NULL;
    Item* it = esc->items;
    idx = 1;
    while (it && idx < opcion) {
        prev = it;
        it = it->siguiente;
        idx++;
    }
    if (!it) {
        printf("Ítem inválido.\n");
        return;
    }
    // Verifica peso máximo (puedes ajustar el límite si tienes uno)
    if (jug->peso_total + it->peso > 20) {
        printf("No puedes cargar más peso.\n");
        return;
    }
    // Quitar el ítem del escenario
    if (prev) prev->siguiente = it->siguiente;
    else esc->items = it->siguiente;

    // Agregar al inventario del jugador
    it->siguiente = jug->inventario;
    jug->inventario = it;
    jug->peso_total += it->peso;
    jug->puntaje += it->valor;
    printf("Recogiste: %s\n", it->nombre);
}

void descartarItems() {
    Jugador* jug = &jugadores[jugador_actual];
    if (!jug->inventario) {
        printf("No tienes ítems para descartar.\n");
        return;
    }
    int idx = 1;
    printf("Ítems en tu inventario:\n");
    for (Item* it = jug->inventario; it != NULL; it = it->siguiente, idx++) {
        printf("  %d) %s (Peso: %d, Valor: %d)\n", idx, it->nombre, it->peso, it->valor);
    }
    printf("Ingrese el número del ítem a descartar (0 para cancelar): ");
    int opcion;
    scanf("%d", &opcion);
    if (opcion <= 0) return;

    // Buscar el ítem seleccionado
    Item* prev = NULL;
    Item* it = jug->inventario;
    idx = 1;
    while (it && idx < opcion) {
        prev = it;
        it = it->siguiente;
        idx++;
    }
    if (!it) {
        printf("Ítem inválido.\n");
        return;
    }
    // Quitar del inventario
    if (prev) prev->siguiente = it->siguiente;
    else jug->inventario = it->siguiente;

    jug->peso_total -= it->peso;
    jug->puntaje -= it->valor;

    // Devolver el ítem al escenario actual
    it->siguiente = jug->escenario_actual->items;
    jug->escenario_actual->items = it;
    printf("Descartaste: %s\n", it->nombre);
}

void avanzarDireccion() {
    Jugador* jug = &jugadores[jugador_actual];
    Escenario* esc = jug->escenario_actual;
    if (!esc || !esc->conexiones) {
        printf("No hay direcciones para avanzar.\n");
        return;
    }
    int idx = 0;
    Conexion* opciones[10];
    printf("Direcciones disponibles:\n");
    for (Conexion* c = esc->conexiones; c != NULL; c = c->siguiente) {
        printf("  %d) %s -> Escenario ID: %d, Desc: %s\n", idx + 1, dir_to_str(c->direccion), c->destino->id, c->destino->descripcion);
        opciones[idx] = c;
        idx++;
    }
    printf("Ingrese el número de la dirección a avanzar (0 para cancelar): ");
    int opcion;
    scanf("%d", &opcion);
    if (opcion <= 0 || opcion > idx) return;

    jug->escenario_actual = opciones[opcion - 1]->destino;
    jug->tiempo_restante--;
    printf("Avanzaste a: %s\n", jug->escenario_actual->descripcion);
}


void leer_escenarios() {
    FILE *archivo = fopen("data/graphquest.csv", "r");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        exit(1);
    }

    printf("Archivo abierto correctamente\n");

    char **campos = leer_linea_csv(archivo, ','); // Leer encabezados y descartar
    if (campos) free(campos);

    while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
        if (!campos[0]) {
            printf("Línea ignorada: ID nulo\n");
            continue;
        } 

        printf("Leyendo línea con ID: %s\n", campos[0]);
        fflush(stdout);


        int id = atoi(campos[0]);
        char* descripcion = campos[2];

        Escenario* esc = buscarEscenarioPorId(id);
        if (!esc) esc = crearEscenario(id, descripcion);

        // Procesar ítems si existen
        if (campos[3] && strlen(campos[3]) > 0) {
            List* items = split_string(campos[3], ";");

            for (char* item = list_first(items); item != NULL; item = list_next(items)) {
                List* valores = split_string(item, ",");

                char* item_name = list_first(valores);
                char* valor_str = list_next(valores);
                char* peso_str  = list_next(valores);

                if (item_name && valor_str && peso_str) {
                    int item_valor = atoi(valor_str);
                    int item_peso  = atoi(peso_str);
                    agregarItem(esc, item_name, item_valor, item_peso);
                } else {
                    printf("Item malformado: %s\n", item);
                }

                list_clean(valores);
                free(valores);
            }

            list_clean(items);
            free(items);
        }

        // Conexiones
        int arriba = atoi(campos[4]);
        int abajo = atoi(campos[5]);
        int izquierda = atoi(campos[6]);
        int derecha = atoi(campos[7]);
        agregarConexionTemporal(id, arriba, abajo, izquierda, derecha);

        // Salida / entrada
        if (strcmp(campos[8], "Sí") == 0 || strcmp(campos[8], "si") == 0 || strcmp(campos[8], "SI") == 0)
            grafo.salida = esc;

        if (strcmp(campos[1], "Entrada principal") == 0)
            grafo.inicio = esc;

        free(campos);
    }

    fclose(archivo);
    printf("Escenarios cargados correctamente\n");
}



int main() {
    char opcion;

    printf("Cargando escenarios...\n");
    leer_escenarios();

    if (!grafo.inicio) {
        printf(" Error: No se encontró el escenario inicial (Entrada principal).\n");
        exit(1);
    }

    printf("Conectando escenarios...\n");
    conectarEscenarios();

    printf("Inicializando jugador...\n");
    inicializarJugador();

    do {
        puts("========================================");
        puts("             Menu del juego             ");
        puts("========================================");

        mostrarEstadoActual();

        puts("1) Recoger ítem(s)");
        puts("2) Descartar ítem(s)");
        puts("3) Avanzar en una Dirección");
        puts("4) Reiniciar Partida");
        puts("5) Salir");

        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);

        switch (opcion) {
            case '1': /* recogerItems(); */ break;
            case '2': /* descartarItems(); */ break;
            case '3': /* avanzarDireccion(); */ break;
            case '4': /* reiniciarPartida(); */ break;
            case '5':
                printf("Saliendo del juego...\n");
                break;
            default:
                printf("Opción inválida, intenta de nuevo.\n");
        }

        printf("\nPresiona Enter para continuar...");
        getchar(); getchar();

    } while (opcion != '5');

    return 0;
}

