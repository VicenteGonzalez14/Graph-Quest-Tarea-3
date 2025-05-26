#include "extra.h"


#define MAX_LINE_LENGTH 4096
#define MAX_FIELDS      128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 4096
#define MAX_FIELDS      128

char **leer_linea_csv(FILE *archivo, char separador) {
    static char linea[MAX_LINE_LENGTH];
    char **campos = malloc(sizeof(char *) * MAX_FIELDS);
    int idx = 0;

    if (fgets(linea, MAX_LINE_LENGTH, archivo) == NULL)
        return NULL;

    // quitar salto de línea
    linea[strcspn(linea, "\r\n")] = '\0';

    char *ptr = linea;
    while (*ptr && idx < MAX_FIELDS - 1) {
        char temp[MAX_LINE_LENGTH];
        char *dest = temp;

        if (*ptr == '\"') {
            ptr++; // saltar comilla inicial
            while (*ptr) {
                if (*ptr == '\"' && *(ptr + 1) == '\"') {
                    *dest++ = '\"';
                    ptr += 2;
                }
                else if (*ptr == '\"') {
                    ptr++;
                    break;
                }
                else {
                    *dest++ = *ptr++;
                }
            }
        } else {
            while (*ptr && *ptr != separador) {
                *dest++ = *ptr++;
            }
        }

        *dest = '\0';
        campos[idx++] = strdup(temp);

        if (*ptr == separador) ptr++;
    }

    campos[idx] = NULL;
    return campos;
}



List *split_string(const char *str, const char *delim) {
  List *result = list_create();
  char *token = strtok((char *)str, delim);

  while (token != NULL) {
    // Eliminar espacios en blanco al inicio del token
    while (*token == ' ') {
      token++;
    }

    // Eliminar espacios en blanco al final del token
    char *end = token + strlen(token) - 1;
    while (*end == ' ' && end > token) {
      *end = '\0';
      end--;
    }

    // Copiar el token en un nuevo string
    char *new_token = strdup(token);

    // Agregar el nuevo string a la lista
    list_pushBack(result, new_token);

    // Obtener el siguiente token
    token = strtok(NULL, delim);
  }

  return result;
}

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}