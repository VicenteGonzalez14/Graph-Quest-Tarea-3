# Tarea3 

## Descripcion
Este programa ejecuta un juego de aventura llamado Graph Quest implementado con un grafo, el cual consiste en recorrer un laberinto y llegar a la salida a base de las decisiones que toma el jugador a lo largo de su partida, puede recoger y soltar items, y moverse en distintas direcciones pero todo esto con un limite de tiempo. 

## ## Requisitos Previos
- Tener instalado [Visual Studio Code](https://code.visualstudio.com/).
- Instalar la extensión **C/C++** (Microsoft).
- Tener instalado un compilador de C (como **gcc**). Si estás en Windows, se recomienda instalar [MinGW](https://www.mingw-w64.org/) o utilizar el entorno [WSL](https://learn.microsoft.com/en-us/windows/wsl/).

### Pasos para compilar y ejecutar:

1. **Clonar el repositorio desde GitHub:**
    - Abre una terminal o línea de comandos.
    - Clona el repositorio usando el siguiente comando:
   ```bash
    git clone https://github.com/usuario/nombre-repositorio.git
   ```

2. **Abrir el proyecto en Visual Studio Code**
    - Inicia Visual Studio Code.
    - Selecciona `Archivo > Abrir carpeta...` y elige la carpeta donde clonaste el repositorio.

3. **Compilar el código**
    - Abre el archivo principal (por ejemplo, `Tarea3.c`).
    - Abre la terminal integrada (`Terminal > Nueva terminal`).
    - En la terminal, compila el programa con el siguiente comando (ajusta el nombre si el archivo principal tiene otro nombre):
    
    ```bash
        gcc -o Tarea3 Tarea3.c tdas/list.c tdas/extra.c
    ```

4. **Ejecuta el programa**
    - Una vez compilado, puedes ejecutar la aplicación con:
    
    ```bash
    ./tarea3
    ```
## FUNCIONALIDADES

1. Gestión de Escenarios
Carga de Escenarios: El programa carga un conjunto de escenarios desde un archivo CSV (graphquest.csv) que describe los nodos del grafo.

Creación y Búsqueda de Escenarios: Cada escenario tiene un ID único y una descripción, y puede estar conectado a otros escenarios en direcciones específicas (arriba, abajo, izquierda, derecha).

Conexiones Entre Escenarios: Los escenarios pueden tener conexiones entre ellos, representadas por una lista de Conexiones (dirección y destino). El juego permite al jugador moverse entre estos escenarios.

2. Inventario de Ítems
Recolección de Ítems: Los escenarios contienen ítems que el jugador puede recoger, cada ítem tiene un nombre, peso y valor.

Gestión de Inventario: El jugador puede acumular ítems en su inventario, y debe gestionar el peso total de su inventario para poder avanzar por el laberinto.

Descartar Ítems: El jugador tiene la opción de descartar ítems de su inventario para reducir el peso y facilitar el movimiento.

3. Gestión del Jugador
Jugador y Estados: El programa permite que haya múltiples jugadores (en este caso, hasta 2 jugadores). Cada jugador tiene su propio inventario, puntaje y escenario actual.

Control del Jugador: El jugador puede realizar diferentes acciones en su turno, tales como recoger ítems, descartar ítems, y avanzar en una dirección.

4. Movimiento a través del Laberinto
Avanzar Entre Escenarios: El jugador puede moverse entre los escenarios conectados. El movimiento depende de las conexiones disponibles (arriba, abajo, izquierda, derecha).

Condiciones para Avanzar: Cada movimiento consume tiempo, y el jugador no puede avanzar si el escenario no tiene conexiones disponibles en la dirección deseada.

5. Tiempo y Puntaje
Tiempo Restante: El jugador tiene un tiempo limitado para completar el juego. Cada acción (como recoger o descartar ítems) reduce el tiempo restante.

Puntaje: Los ítems recogidos suman puntaje al jugador, el cual se ve reflejado en su estado actual.

6. Lógica de Conexiones Temporales
Conexiones Temporales: Durante la carga de escenarios, las conexiones entre escenarios son primero almacenadas temporalmente en una estructura (ConexionTemporal) antes de ser procesadas y agregadas al grafo de escenarios.

7. Interfaz de Usuario
Menú de Opciones: El juego presenta un menú interactivo donde el jugador puede elegir qué acción realizar: recoger ítems, descartar ítems, avanzar en una dirección, reiniciar la partida o salir del juego.

Estado Actual del Jugador: En cada turno, el programa muestra información relevante del jugador: el escenario actual, los ítems disponibles, el inventario del jugador, y las direcciones disponibles para moverse.

8. Reinicio de la Partida
Reiniciar Partida: El jugador puede reiniciar la partida, lo que implica reiniciar su inventario, el tiempo restante y el escenario actual.

9. Validación de Entradas
Comprobación de Conexiones Válidas: Al leer las conexiones entre escenarios, el programa asegura que solo se agreguen conexiones válidas (no se permiten conexiones hacia escenarios inexistentes).

Manejo de Datos Malformados: El programa verifica que los datos leídos del archivo CSV sean válidos, y maneja los casos donde los ítems o conexiones no estén definidos correctamente.

## Ejemplo de uso
 El jugador ve su estado actual, incluyendo el escenario en el que está, su inventario y las direcciones disponibles para moverse.
### Mostrar Estado Actual

Ingrese su opción: 3

    --- Turno del Jugador 1 ---
    Escenario actual: Entrada principal
    Ítems disponibles:

    Linterna (Peso: 5, Valor: 10)
    Inventario (0 kg, 0 pts):
    (Inventario vacío)
    Tiempo restante: 10
    Direcciones disponibles:

    Arriba

    Abajo

    Izquierda

    Derecha

    Presione cualquier tecla para continuar...

### Opción 4: El jugador decide recoger un ítem disponible en el escenario y agregarlo a su inventario.
Recoger ítem(s)

    Ingrese su opción: 4
    Ítems disponibles:

    Linterna (Peso: 5, Valor: 10)
    Ingrese el número del ítem a recoger (0 para cancelar): 1
    Ítem recogido: Linterna (Peso: 5, Valor: 10)
    Inventario (5 kg, 10 pts):

    Linterna (Peso: 5, Valor: 10)
    Tiempo restante: 9

    Presione cualquier tecla para continuar...

### Opción 5: El jugador decide avanzar en una dirección disponible y se mueve a un nuevo escenario.
Avanzar en una dirección

    Ingrese su opción: 5
    Direcciones disponibles:

    Arriba -> Escenario ID: 2, Descripción: Librería

    Abajo -> Escenario ID: 3, Descripción: Caverna oscura

    Izquierda -> Escenario ID: 4, Descripción: Estudio
    
    Derecha -> Escenario ID: 5, Descripción: Cocina
    Ingrese el número de la dirección a avanzar (0 para cancelar): 2
    Avanzaste a: Caverna oscura
    Tiempo restante: 8
    
    Presione cualquier tecla para continuar...

### Opción 6: El jugador decide descartar un ítem de su inventario para reducir peso.
Descartar ítem(s)

    Ingrese su opción: 6
    Ítems en tu inventario:
    
    Linterna (Peso: 5, Valor: 10)
    Ingrese el número del ítem a descartar (0 para cancelar): 1
    Ítem descartado: Linterna (Peso: 5, Valor: 10)
    Inventario (0 kg, 0 pts):
    (Inventario vacío)
    Tiempo restante: 7
    
    Presione cualquier tecla para continuar...

### Opción 7: El jugador decide reiniciar la partida, borrando su inventario y reiniciando el tiempo.
Reiniciar Partida

    Ingrese su opción: 7
    Partida reiniciada.
    Inventario (0 kg, 0 pts):
    (Inventario vacío)
    Tiempo restante: 10
    Presione cualquier tecla para continuar...

### Opción 8: El jugador decide salir del juego.
Salir

    Ingrese su opción: 8
    Saliendo del juego...
    Fin de la partida.

No hubieron contribuciones el trabajo fue individual.
