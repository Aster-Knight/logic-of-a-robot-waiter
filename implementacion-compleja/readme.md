
# Documentación de Diseño Complejo: Implementación de Asignación de Mesas con Estructuras de Datos

## 1. Introducción

Esta documentación detalla el diseño de la **Implementación 1** del Sistema de Asignación Inteligente de Mesas. Esta implementación aprovechará estructuras de datos estándar de Python, objetos de clase personalizados, y una implementación propia de una Cola de Prioridad para gestionar las solicitudes de los clientes. La arquitectura de la aplicación seguirá el patrón Modelo-Vista-Controlador (MVC) para separar las responsabilidades y mejorar la modularidad y mantenibilidad. La interacción con el usuario se realizará a través de una interfaz de línea de comandos (CLI) personalizada, construida sin dependencias de librerías externas para facilitar su reutilización en la implementación primitiva.

## 2. Arquitectura del Sistema (Patrón MVC)

La aplicación se estructurará en los siguientes componentes:

*   **Modelo (Model):** Contendrá la lógica de negocio, los datos y las reglas del sistema. Esto incluye:
    *   Clases para representar entidades: `Cliente`, `Mesa`, `SolicitudAsignacion`.
    *   La clase `ColaPrioridadPropia` para gestionar la cola de espera.
    *   La clase `GestorRestaurante` (o similar) que encapsula el estado de las mesas, la cola de prioridad, y la lógica de asignación, registro y liberación.
    *   Lógica para calcular prioridades, verificar tiempos de espera y aplicar reglas de negocio.
*   **Vista (View):** Será responsable de presentar la información al usuario y de recibir su entrada. En esta implementación, la Vista será la **consola**.
    *   Funciones para mostrar el menú de opciones.
    *   Funciones para solicitar datos del usuario (ej. detalles de un nuevo grupo, ID de mesa a liberar).
    *   Funciones para mostrar el estado del sistema (cola de espera, estado de mesas, mensajes de confirmación o error).
*   **Controlador (Controller):** Actuará como intermediario entre el Modelo y la Vista.
    *   Recibirá las acciones del usuario desde la Vista.
    *   Invocará los métodos apropiados en el Modelo para procesar esas acciones.
    *   Tomará los resultados del Modelo y los pasará a la Vista para su presentación.
    *   Gestionará el flujo general de la aplicación y la interacción con el usuario a través de la CLI personalizada.

## 3. Diseño Detallado de los Componentes

### 3.1. Modelo (Model)

#### 3.1.1. Clase `Cliente`

*   **Atributos:**
    *   `id_cliente` (int): Identificador único del cliente/grupo.
    *   `nombre_grupo` (str): Nombre o identificador del grupo.
    *   `hora_llegada` (datetime.datetime): Timestamp exacto de llegada.
    *   `tiene_reserva` (bool): Indica si el grupo tiene una reserva.
    *   `tipo_cliente` (str): Categoría del cliente (ej: "VIP", "REGULAR", "OCASIONAL").
    *   `numero_personas` (int): Cantidad de personas en el grupo (1-6).
    *   `tiempo_espera_actual` (datetime.timedelta): Calculado dinámicamente.

#### 3.1.2. Clase `Mesa`

*   **Atributos:**
    *   `id_mesa` (int o str): Identificador único de la mesa.
    *   `capacidad` (int): Número máximo de personas que puede albergar (2, 4 o 6).
    *   `disponible` (bool): Estado de la mesa.
    *   `cliente_asignado_id` (int | None): ID del cliente/grupo asignado si está ocupada.
    *   `hora_asignacion` (datetime.datetime | None): Hora en que se asignó la mesa.

#### 3.1.3. Clase `SolicitudAsignacion`

*   **Atributos:**
    *   `cliente` (Cliente): La instancia del cliente/grupo asociado.
    *   `prioridad` (tuple): Tupla calculada utilizada para el ordenamiento en la cola de prioridad. Ejemplo: `(valor_reserva, valor_tiempo_espera_excedido, valor_tipo_cliente, valor_hora_llegada)`. *Valores menores indican mayor prioridad.*

#### 3.1.4. Clase `ColaPrioridadPropia`

*   **Implementación:** Se basará en un **heap binario (min-heap) implementado manualmente** utilizando una lista de Python. Las operaciones de inserción y extracción mantendrán la propiedad del heap mediante los algoritmos `_sift_up` y `_sift_down` implementados directamente en la clase.
*   **Atributos:**
    *   `_heap` (list[SolicitudAsignacion]): Lista interna que almacena los objetos `SolicitudAsignacion` que representan los elementos de la cola.
*   **Métodos Principales:**
    *   `add(solicitud: SolicitudAsignacion)`: Añade una solicitud a la cola, la coloca al final de la lista interna y luego llama a `_sift_up` para restaurar la propiedad del heap.
    *   `pop()` -> `SolicitudAsignacion | None`: Remueve y retorna la solicitud con mayor prioridad (menor valor de prioridad, que se encuentra en la raíz del heap). Mueve el último elemento a la raíz y llama a `_sift_down` para restaurar la propiedad del heap. Retorna `None` si la cola está vacía.
    *   `peek()` -> `SolicitudAsignacion | None`: Retorna la solicitud con mayor prioridad sin removerla (el elemento en la raíz del heap). Retorna `None` si la cola está vacía.
    *   `is_empty()` -> `bool`: Verifica si la cola está vacía.
    *   `size()` -> `int`: Retorna el número de elementos en la cola.
    *   `get_all_items()` -> `list[SolicitudAsignacion]`: Retorna una lista de todas las solicitudes en la cola, ordenadas por prioridad (para fines de visualización).

#### 3.1.5. Clase `GestorRestaurante`

*   **Atributos:**
    *   `mesas` (list[Mesa]): Lista de todas las mesas del restaurante.
    *   `cola_espera` (ColaPrioridadPropia): Instancia de la cola de prioridad para los clientes en espera.
    *   `clientes_atendidos` (list[Cliente]): (Opcional) Para estadísticas.
    *   `tiempo_simulacion_actual` (datetime.datetime): Hora actual de la simulación.
    *   `proximo_id_cliente` (int): Contador para IDs de cliente.
    *   `limite_grupos_cola` (int): Constante (15).
    *   `limite_tiempo_espera_mins` (int): Constante (15).
*   **Métodos Principales (Lógica de Negocio):**
    *   `inicializar_mesas()`: Crea las 12 instancias de `Mesa` con sus capacidades.
    *   `registrar_llegada(nombre_grupo: str, tiene_reserva: bool, tipo_cliente: str, numero_personas: int)` -> `str` (mensaje de resultado):
        *   Verifica el tamaño del grupo (1-6).
        *   Verifica si la `cola_espera` está llena (<= `limite_grupos_cola`).
        *   Crea un objeto `Cliente`.
        *   Calcula la prioridad inicial para el cliente (considerando reserva, tipo, hora llegada).
        *   Crea `SolicitudAsignacion` y la añade a `cola_espera.add()`.
        *   Retorna un mensaje de éxito o error.
    *   `liberar_mesa(id_mesa: int)` -> `str`:
        *   Encuentra la mesa por ID.
        *   La marca como `disponible` y `cliente_asignado_id = None`.
        *   Retorna un mensaje.
    *   `intentar_asignar_mesa()` -> `str` (mensaje de asignación o de no disponibilidad):
        *   Si `cola_espera` no está vacía, obtiene la `SolicitudAsignacion` de mayor prioridad con `cola_espera.peek()`.
        *   Calcula el tiempo de espera del cliente: `tiempo_simulacion_actual - solicitud.cliente.hora_llegada`.
        *   Determina si el tiempo de espera excede los 15 minutos.
        *   Busca una mesa adecuada:
            *   Primero intenta con capacidad exacta o ligeramente superior.
            *   Si el tiempo de espera es >= 15 mins, considera mesas más grandes.
        *   Si encuentra mesa:
            *   Remueve la solicitud de la cola con `cola_espera.pop()`.
            *   Actualiza el estado de la mesa (`disponible = False`, `cliente_asignado_id`).
            *   Registra `hora_asignacion` en la mesa.
            *   Retorna un mensaje detallando la asignación.
        *   Si no encuentra mesa, retorna un mensaje indicándolo.
    *   `avanzar_tiempo_simulacion(minutos: int = 5)`: Incrementa `tiempo_simulacion_actual`.
    *   `get_cola_espera_formateada()` -> `list[str]`: Retorna una lista de strings representando la cola de espera para la Vista.
    *   `get_estado_mesas_formateado()` -> `list[str]`: Retorna una lista de strings representando el estado de las mesas para la Vista.
    *   `_calcular_prioridad(cliente: Cliente, tiempo_espera_excedido: bool)` -> `tuple`: Función helper privada para calcular la tupla de prioridad.

### 3.2. Vista (View)

*   Módulo `vista_consola.py` (o similar).
*   **Funciones:**
    *   `mostrar_menu_principal()`: Imprime las opciones de comandos disponibles.
    *   `solicitar_input(prompt: str)` -> `str`: Wrapper seguro para `input()`.
    *   `mostrar_mensaje(mensaje: str)`: Imprime mensajes de información, éxito o error.
    *   `solicitar_datos_nuevo_grupo()` -> `dict`: Pide al usuario los datos de un nuevo grupo y los retorna en un diccionario.
    *   `solicitar_id_mesa(prompt: str)` -> `int`: Pide y valida un ID de mesa.
    *   `mostrar_cola_espera(cola_formateada: list[str])`: Imprime la cola de espera.
    *   `mostrar_estado_mesas(mesas_formateadas: list[str])`: Imprime el estado de las mesas.
    *   `mostrar_reglas_prioridad()`: Imprime las reglas de prioridad.

### 3.3. Controlador (Controller)

*   Módulo `controlador_aplicacion.py` (o similar), con una clase `ControladorAplicacion`.
*   **Atributos:**
    *   `gestor_restaurante` (GestorRestaurante): Instancia del Modelo.
    *   `vista` (módulo/objeto de la Vista): Referencia a las funciones de la Vista.
*   **Métodos Principales (Lógica de la CLI personalizada):**
    *   `iniciar_aplicacion()`: Contiene el bucle principal de la CLI (similar al de `main.py` anterior).
        *   Llama a `vista.mostrar_menu_principal()`.
        *   Llama a `vista.solicitar_input()` para obtener el comando del usuario.
        *   Usa `shlex.split()` para parsear la línea de comando.
        *   Usa `argparse` (con parsers específicos por comando) para validar y extraer argumentos.
    *   Métodos handler para cada comando (ej: `_manejar_registrar_grupo(args)`, `_manejar_liberar_mesa(args)`, etc.):
        *   Estos métodos toman los `parsed_args` de `argparse`.
        *   Llaman a los métodos correspondientes en `gestor_restaurante`.
        *   Toman el resultado del gestor y llaman a los métodos de `vista` para mostrarlo.
        *   Manejan la lógica de avanzar el tiempo después de ciertas operaciones.

## 4. Flujo de Datos y Operaciones

1.  **Inicio:** `main.py` instancia `GestorRestaurante` (Modelo), `VistaConsola` (Vista, implícitamente a través de sus funciones) y `ControladorAplicacion`. Llama a `controlador.iniciar_aplicacion()`.
2.  **Bucle de Comandos:** El Controlador muestra el prompt y espera input.
3.  **Entrada de Comando:** El usuario introduce un comando (ej: `register_group "Familia Perez" --reserva --tipo VIP --personas 4`).
4.  **Parseo:** El Controlador usa `shlex` y luego `argparse` para validar y extraer el comando y sus argumentos.
5.  **Ejecución:**
    *   El Controlador llama al método handler correspondiente (ej: `_manejar_registrar_grupo`).
    *   El handler llama al método apropiado en `gestor_restaurante` (Modelo) (ej: `gestor.registrar_llegada(...)`).
    *   El `gestor_restaurante` actualiza su estado (ej: añade a `ColaPrioridadPropia`, modifica una `Mesa`).
    *   El `gestor_restaurante` retorna un resultado (ej: mensaje de confirmación).
    *   El handler del Controlador pasa este resultado a la `vista` (ej: `vista.mostrar_mensaje(...)`).
6.  **Avanzar Tiempo:** Después de ciertas operaciones, el Controlador podría llamar a `gestor_restaurante.avanzar_tiempo_simulacion()`.
7.  **Repetir:** El bucle continúa hasta que el usuario introduce `exit`.

## 5. Implementación de la Cola de Prioridad Propia

*   **Estructura Interna:** Una lista estándar de Python (`list`) actuará como el array subyacente para la estructura de datos del heap.
*   **Lógica del Heap (Min-Heap):**
    *   Las operaciones de la cola de prioridad se implementarán para mantener la propiedad de un min-heap, donde el nodo padre siempre tiene una prioridad menor o igual (es decir, es "más importante") que sus nodos hijos.
    *   La comparación entre `SolicitudAsignacion` se basa en el método `__lt__` definido en dicha clase, que compara primero la tupla de prioridad y luego el contador de inserción como *tie-breaker*.
*   **Operaciones Clave Implementadas Manualmente:**
    *   **`_sift_up(index)`:** Después de añadir un elemento al final del heap, esta función lo "hace flotar" hacia arriba, intercambiándolo con su padre mientras sea de menor valor (mayor prioridad) que su padre, hasta que se restaure la propiedad del heap o alcance la raíz.
    *   **`_sift_down(index)`:** Después de reemplazar la raíz con el último elemento del heap (durante una operación `pop`), esta función "hunde" el elemento hacia abajo, intercambiándolo con el menor de sus hijos (de mayor prioridad) mientras sea de mayor valor que alguno de sus hijos, hasta que se restaure la propiedad del heap o se convierta en una hoja.
    *   **`add(solicitud)`:** Añade el elemento al final de la lista interna y luego llama a `_sift_up` en el índice del nuevo elemento.
    *   **`pop()`:** Extrae el elemento de la raíz (el de mayor prioridad). Mueve el último elemento de la lista a la posición de la raíz. Si el heap no queda vacío, llama a `_sift_down` en la raíz.

## 6. Estructura de Archivos del Proyecto

El proyecto se organiza en la siguiente estructura de directorios y archivos para mantener la modularidad y la separación de responsabilidades según el patrón MVC:

```
robot_mesero_mvc/
├── src/                     # Directorio principal del código fuente (Modelo, Vista, Controlador)
│   ├── __init__.py          # Hace que 'src' sea reconocido como un paquete Python. Puede estar vacío.
│   ├── cliente.py           # Modelo: Define la clase `Cliente`.
│   ├── mesa.py              # Modelo: Define la clase `Mesa`.
│   ├── solicitud_asignacion.py # Modelo: Define la clase `SolicitudAsignacion` para la cola de prioridad.
│   ├── cola_prioridad_propia.py # Modelo: Implementación de la `ColaPrioridadPropia`.
│   ├── gestor_restaurante.py  # Modelo: Clase `GestorRestaurante` con la lógica de negocio principal.
│   └── vista_consola.py     # Vista: Clase `VistaConsola` para la interacción con el usuario.
│   └── controlador_aplicacion.py # Controlador: Clase `ControladorAplicacion` que gestiona el flujo.
└── main.py                  # Punto de entrada de la aplicación. Instancia y ejecuta el Controlador.
```

**Descripción de los Componentes Clave:**

*   **`src/`**: Contiene todos los módulos de la lógica principal de la aplicación.
    *   **`__init__.py`**: Permite tratar al directorio `src` como un paquete, facilitando las importaciones relativas entre sus módulos.
    *   **Módulos del Modelo (`cliente.py`, `mesa.py`, `solicitud_asignacion.py`, `cola_prioridad_propia.py`, `gestor_restaurante.py`):** Encapsulan las entidades de datos, la estructura de la cola de prioridad personalizada y la lógica de negocio del sistema de asignación.
    *   **`vista_consola.py`**: Contiene la clase `VistaConsola`, responsable de toda la presentación de información y la captura de entradas del usuario a través de la consola.
    *   **`controlador_aplicacion.py`**: Contiene la clase `ControladorAplicacion`, que actúa como el orquestador entre la Vista y el Modelo, procesando los comandos del usuario.
*   **`main.py`**: Script simple que se ejecuta para iniciar la aplicación. Su única responsabilidad es crear una instancia del `ControladorAplicacion` y llamar a su método para comenzar la interacción.

## 7. Instrucciones de Implementación y Uso

Esta sección detalla los requisitos y los pasos necesarios para configurar y ejecutar la aplicación.

### 7.1. Requisitos del Sistema

*   **Python:** Versión 3.7 o superior.
*   **Módulos Estándar de Python:** La aplicación utiliza únicamente módulos incluidos en la biblioteca estándar de Python. Los módulos utilizados son:
    *   `datetime` (para manejo de fechas y horas)
    *   `argparse` (para el parseo de argumentos de los comandos)
    *   `shlex` (para dividir la entrada de comandos respetando comillas)
    *   `os`, `sys`, `stat`, `pathlib` (para interacción con el sistema de archivos y rutas)
    *   `typing` (para anotaciones de tipo)
    *   *Nota: El módulo `heapq` ya no se utiliza directamente para la lógica fundamental de la cola de prioridad, ya que esta se implementa manualmente.*

### 7.2. Pasos para la Configuración y Ejecución

1.  **Obtener el Código Fuente:**
    *   Si el proyecto está en un repositorio Git, clónalo:
        ```bash
        git clone [URL_DEL_REPOSITORIO_GIT]
        cd [NOMBRE_DEL_DIRECTORIO_DEL_PROYECTO]
        ```
    
2.  **Verificar la Versión de Python:**
    Abre una terminal o línea de comandos y ejecuta:
    ```bash
    python --version
    # o
    python3 --version
    ```
    Asegúrate de que la versión sea 3.7 o superior.

3.  **Navegar al Directorio Raíz:**
    En tu terminal, navega hasta el directorio raíz del proyecto (la carpeta que contiene `main.py` y la subcarpeta `src/`). Por ejemplo, si clonaste en `robot_mesero_mvc`, navega a `robot_mesero_mvc`.

4.  **Ejecutar la Aplicación:**
    Desde el directorio raíz del proyecto, ejecuta el script `main.py`:
    ```bash
    python main.py
    ```
    Esto iniciará la interfaz de línea de comandos interactiva de la aplicación.

### 7.3. Uso de la Interfaz de Consola

Una vez que la aplicación esté en ejecución, verás un prompt similar a `RestauranteCLI >`.

1.  **Obtener Ayuda:**
    Escribe `help` y presiona Enter para ver la lista de comandos principales disponibles.
    ```
    > help
    ```
    Para obtener ayuda específica sobre un comando y sus opciones, escribe `help <nombre_del_comando>`:
    ```
    > help register_group
    > help print
    ```

2.  **Ejecutar Comandos:**
    Introduce los comandos seguidos de sus argumentos y opciones. Recuerda:
    *   **Encerrar rutas o nombres con espacios entre comillas dobles (`"`)**.
    *   Las opciones suelen comenzar con `--` (ej: `--depth 3`, `--reserva`).

    **Ejemplos de Comandos:**

    *   Registrar un nuevo grupo:
        ```
        > register_group "Familia García" --personas 4 --reserva --tipo VIP
        > register_group Juan --personas 2 --tipo REGULAR
        ```
        También puedes llamar a `register_group` sin argumentos y la aplicación te pedirá los datos interactivamente.

    *   Liberar una mesa:
        ```
        > free_table 5
        ```
        O, sin argumentos, `free_table` te preguntará el ID.

    *   Avanzar el tiempo de simulación:
        ```
        > advance_time
        > advance_time 10
        ```

    *   Ver la cola de espera:
        ```
        > view_queue
        ```

    *   Ver el estado de las mesas:
        ```
        > view_tables
        ```

    *   Ver las reglas de prioridad:
        ```
        > view_rules
        ```

    *   Intentar asignar una mesa manualmente (aunque también se intenta después de `free_table` y `advance_time`):
        ```
        > assign_table
        ```

3.  **Salir de la Aplicación:**
    Escribe `exit` y presiona Enter.
    ```
    > exit
    ```
    También puedes usar `Ctrl+D` (en Linux/macOS) o `Ctrl+Z` seguido de Enter (en Windows) para enviar una señal EOF, o `Ctrl+C` para interrumpir la aplicación.

### 7.4. Consideraciones Adicionales

*   **Persistencia de Datos:** Esta implementación opera completamente en memoria. Al cerrar la aplicación, el estado actual de la cola de espera y las mesas asignadas **no se guarda**. Cada ejecución comienza con un restaurante vacío.
*   **Manejo de Errores:** La aplicación intenta manejar entradas incorrectas de comandos y errores básicos, pero una entrada muy malformada podría no ser capturada por los parsers.
*   **Tiempo de Simulación:** El tiempo es crucial para la lógica de prioridad. Usa `advance_time` para simular el paso del tiempo y ver cómo afecta la cola y las asignaciones.

## 8. Consideraciones Adicionales (Previamente Inciso 6)

*   Manejo de Tiempo: El módulo `datetime` será crucial para registrar `hora_llegada`, calcular `tiempo_espera_actual` y gestionar `tiempo_simulacion_actual`.
*   Constantes: Se definirán constantes para los límites (tamaño de grupo, capacidad de cola, tiempo de espera, ponderaciones de tipo de cliente) para fácil modificación.
*   Pruebas Unitarias: Aunque no es un requisito explícito del anteproyecto, se recomienda pensar en cómo se podrían probar unitariamente los componentes del Modelo, especialmente la `ColaPrioridadPropia` y la lógica de `GestorRestaurante`.
---
