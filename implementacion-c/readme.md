
# Documentación de Implementación: Sistema de Asignación de Mesas (Implementación Primitiva en C)

## 1. Introducción

Este documento describe la **Implementación 2** del Sistema de Asignación Inteligente de Mesas. A diferencia de la Implementación 1, esta versión se desarrolla utilizando únicamente **variables de tipo primitivo y estructuras de control básicas** en el lenguaje C, evitando deliberadamente el uso de estructuras de datos compuestas como `struct` (para agrupar atributos de entidades como colecciones), `arrays` (para representar colecciones de entidades dinámicas o de tamaño variable), ni cualquier otra abstracción de colección. El objetivo es demostrar las complejidades y limitaciones inherentes al resolver problemas con este nivel de restricción, en contraste con un enfoque que utiliza abstracciones de datos y estructuras de datos más avanzadas. La versión de 3 slots se realizó como una prueba inicial de esta lógica primitiva antes de escalarla al requisito de 15 slots.

## 2. Arquitectura y Representación de Datos

Dada la restricción de no usar `struct` ni `arrays` para colecciones, la arquitectura no sigue formalmente un patrón MVC. En cambio, el sistema se basa en:

*   **Variables Globales/Estáticas:** El estado completo del sistema (clientes en espera y mesas) se mantiene en conjuntos de variables globales o estáticas individuales y hardcodeadas. Cada atributo de cada cliente en espera (hasta 15 slots) y cada mesa (12 mesas) tiene su propia variable con nombre único (ej., `g_slot_cliente1_nombre`, `g_mesa5_disponible`).
*   **Funciones Procedurales:** La lógica del sistema se implementa en funciones que acceden y modifican directamente estas variables globales.
*   **CLI Manual:** La interfaz de usuario se implementa con lectura (`fgets`, `sscanf`) e impresión (`printf`) directas en consola, y un parseo manual básico de los comandos introducidos.

## 3. Representación Primitiva de Entidades

*   **Clientes en Espera (Simulados en Slots):** Se define un número fijo de **15 slots** de cliente en espera (`MAX_CLIENTES_EN_COLA_SIMULADA = 15`). Cada slot tiene su propio conjunto de variables globales para atributos como `g_slot_clienteX_ocupado`, `g_slot_clienteX_nombre`, `g_slot_clienteX_hora_llegada_minutos`, `g_slot_clienteX_numero_personas`, etc., donde X varía de 1 a 15. Un slot se considera "ocupado" si su variable `g_slot_clienteX_ocupado` es 1. El número total de clientes en espera (`g_clientes_en_cola_count`) se mantiene en una variable separada.
*   **Mesas:** Las 12 mesas (`NUM_MESAS = 12`) también se representan mediante conjuntos de variables globales individuales para sus atributos (ej., `g_mesaY_capacidad`, `g_mesaY_disponible`, `g_mesaY_cliente_asignado_id`), donde Y varía de 1 a 12.

## 4. Criterios de Prioridad Primitivos

La prioridad de un cliente se calcula en un solo valor numérico (`g_slot_clienteX_prioridad_calculada`). Los valores más bajos indican mayor prioridad. La función `calcular_prioridad_numerica` combina pesos para los criterios principales:

1.  **Reserva:** 0 si tiene, 200 si no tiene.
2.  **Espera Excedida (>= 15 mins):** 0 si excedió, 100 si no excedió.
3.  **Tipo de Cliente:** VIP (10), REGULAR (20), OCASIONAL (30).

La prioridad calculada es la suma de estos pesos. El desempate entre clientes con el mismo valor numérico de prioridad se realiza **explícitamente** en la lógica de búsqueda del cliente prioritario dentro de la función `intentar_asignar_mesa`, comparando sus tiempos de llegada (`g_slot_clienteX_hora_llegada_minutos`) (FIFO).

## 5. Funcionalidades y Operaciones

Las funcionalidades de la aplicación se implementan como funciones que manipulan directamente las variables globales:

*   **`inicializar_sistema()`:** Pone todas las variables de `g_slot_clienteX_ocupado` a 0 y reinicia los contadores.
*   **`registrar_llegada_cliente(nombre, reserva_flag, tipo, personas)`:** Busca el primer slot libre (`g_slot_clienteX_ocupado == 0`) de 1 a 15. Si encuentra uno y el límite de 15 grupos no se excede, copia los datos a las variables correspondientes de ese slot, asigna un ID, calcula y guarda la prioridad numérica inicial, y incrementa `g_clientes_en_cola_count`.
*   **`liberar_mesa(id_mesa)`:** Utiliza una cadena `if/else if` o `switch` para identificar la mesa por `id_mesa` (1 a 12) y marca su variable `g_mesaY_disponible` como 1. Luego llama a `intentar_asignar_mesa()`.
*   **`avanzar_tiempo(minutos, intentar_asignacion_despues)`:** Incrementa `g_tiempo_simulacion_actual_minutos` y opcionalmente llama a `intentar_asignar_mesa()`.
*   **`intentar_asignar_mesa()`:**
    *   Actualiza la `prioridad_calculada` para cada slot de cliente ocupado, considerando si su tiempo de espera actual excede los 15 minutos.
    *   **Simulación de `Peek` de Cola de Prioridad:** Itera a través de los 15 slots de cliente (si están ocupados) para encontrar el slot con el menor valor de `prioridad_calculada`. Si hay empates en `prioridad_calculada`, compara `hora_llegada_minutos` para seleccionar al que llegó antes.
    *   Busca una mesa disponible: Itera a través de las 12 mesas utilizando `if/else if`, verificando `g_mesaY_disponible` y `g_mesaY_capacidad` según las reglas de asignación (capacidad exacta primero, luego mayor si espera excedida).
    *   Si encuentra una mesa: Asigna el ID del cliente al slot de la mesa, marca la mesa como no disponible, y **"elimina" el cliente de la cola marcando su `g_slot_clienteX_ocupado` como 0** y decrementando `g_clientes_en_cola_count`. Esto simula la operación `Pop`.
*   **`revisar_cola_espera()`:** Actualiza la `prioridad_calculada` para cada slot ocupado y luego itera a través de los 15 slots imprimiendo los detalles de los que están ocupados. La salida no está formalmente ordenada por prioridad, pero muestra la prioridad calculada para que el usuario la interprete.
*   **`revisar_estado_mesas()`:** Itera a través de las 12 mesas imprimiendo sus variables de estado.
*   **`mostrar_reglas_prioridad()`:** Imprime una descripción de las reglas de prioridad y los pesos utilizados.

## 6. Estructura de Archivos del Proyecto

La implementación primitiva reside en un único archivo fuente de C:

```
implementacion-c/
└── restaurante-15slots.c # Contiene todas las variables globales y funciones.
```

## 7. Instrucciones de Implementación y Uso

### 7.1. Requisitos del Sistema

*   **Compilador C:** Se necesita un compilador de C (como GCC o Clang) instalado en el sistema.
*   **Sistema Operativo:** Compatible con operaciones básicas de consola (entrada/salida estándar).
*   **Módulos/Librerías:** Utiliza únicamente las librerías estándar de C (`stdio.h`, `string.h`, `stdlib.h`, `time.h`).

### 7.2. Pasos para la Configuración y Ejecución

1.  **Obtener el Código Fuente:** Guarda el código completo proporcionado como `restaurante-15slots.c ` en una carpeta de tu elección.
2.  **Abrir Terminal:** Abre una terminal o línea de comandos en el directorio donde guardaste el archivo `.c`.
3.  **Compilar el Código:** Usa tu compilador C para compilar el archivo fuente. Por ejemplo, con GCC:
    ```bash
    gcc restaurante-15slots.c  -o robot_mesero_primitivo
    ```
    Esto creará un archivo ejecutable llamado `robot_mesero_primitivo` (o `robot_mesero_primitivo.exe` en Windows).
4.  **Ejecutar la Aplicación:** Ejecuta el archivo compilado desde la terminal:
    ```bash
    ./robot_mesero_primitivo
    ```
    O en Windows:
    ```bash
    robot_mesero_primitivo.exe
    ```

### 7.3. Uso de la Interfaz de Consola

La aplicación iniciará un bucle de comandos simple con el prompt `PrimitivoCLI >`.

*   **Comandos:** register, free, advance, queue, tables, rules, assign, exit
*   **`register <NombreGrupo> <TieneReserva(0/1)> <TipoCliente(VIP/REGULAR/OCASIONAL)> <NumPersonas>`:** Registra un grupo. **Nota: `NombreGrupo` y `TipoCliente` NO deben contener espacios.**
    *   Ejemplo: `register FamiliaGarcia 1 VIP 4`
    *   Ejemplo: `register Juan 0 REGULAR 2`
*   **`free <ID_Mesa>`:** Libera una mesa por su ID (1 a 12). Esto intenta asignar automáticamente al cliente prioritario.
    *   Ejemplo: `free 5`
*   **`advance [minutos]`:** Avanza el tiempo de simulación. Por defecto son 5 minutos si no se especifica un número. Intenta asignar automáticamente.
    *   Ejemplo: `advance 10`
    *   Ejemplo: `advance`
*   **`queue`:** Muestra el estado de los slots de clientes en espera.
*   **`tables`:** Muestra el estado de las mesas.
*   **`rules`:** Muestra las reglas de prioridad.
*   **`assign`:** Intenta asignar una mesa al cliente prioritario sin avanzar el tiempo.
*   **`exit`:** Sale de la aplicación.

## 8. Comparación de Funcionalidades y Requisitos Cumplidos

| Característica / Requisito             | Implementación 1 (Python con Estructuras)                                 | Implementación 2 (C con Primitivas)                                                                 | Estado en Implementación C                                                                                                |
| :------------------------------------- | :------------------------------------------------------------------------ | :-------------------------------------------------------------------------------------------------- | :------------------------------------------------------------------------------------------------------------------------ |
| **Nº Mesas (12, capacidades fijas)**   | Cumplido (Objetos `Mesa` en lista)                                        | Cumplido (Variables globales `g_mesaY_...`)                                                         | **Cumplido**                                                                                                              |
| **Tamaño Grupos (1-6)**                | Cumplido (Validación en `registrar_llegada`)                              | Cumplido (Validación en `registrar_llegada_cliente`)                                                | **Cumplido**                                                                                                              |
| **Límite Grupos en Cola (15)**         | Cumplido (Verificación `cola_espera.size() <= 15`)                        | Limitado a 15 slots hardcodeados                                                                    | **Cumplido** (por el límite físico de variables, no por escalabilidad)                                                    |
| **Tiempo Espera Máx (15 mins)**        | Cumplido (Lógica en `GestorRestaurante`)                                  | Cumplido (Lógica en `intentar_asignar_mesa`)                                                        | **Cumplido**                                                                                                              |
| **Regla Espera Excedida (Mesa Mayor)** | Cumplido (Lógica en `GestorRestaurante`)                                  | Cumplido (Lógica en `intentar_asignar_mesa`)                                                        | **Cumplido**                                                                                                              |
| **Prioridad por Reserva**              | Cumplido (Parte de la tupla de prioridad)                                 | Cumplido (Peso en `calcular_prioridad_numerica`)                                                    | **Cumplido**                                                                                                              |
| **Prioridad por Tiempo Espera Excedida** | Cumplido (Parte de la tupla de prioridad)                                 | Cumplido (Peso en `calcular_prioridad_numerica`)                                                    | **Cumplido**                                                                                                              |
| **Prioridad por Tipo Cliente (VIP)**   | Cumplido (Parte de la tupla de prioridad)                                 | Cumplido (Peso en `calcular_prioridad_numerica`)                                                    | **Cumplido** (VIP tiene el menor peso numérico)                                                                           |
| **Desempate FIFO (Hora Llegada)**      | Cumplido (Último elemento de la tupla de prioridad)                         | Cumplido (Comparación explícita en `intentar_asignar_mesa` si `prioridad_calculada` es igual)       | **Cumplido**                                                                                                              |
| **Registrar Nuevo Grupo**              | Cumplido (`GestorRestaurante.registrar_llegada`)                          | Cumplido (`registrar_llegada_cliente`)                                                              | **Cumplido**                                                                                                              |
| **Reportar Mesa Vacía**                | Cumplido (`GestorRestaurante.liberar_mesa`)                               | Cumplido (`liberar_mesa`)                                                                           | **Cumplido**                                                                                                              |
| **Intentar Asignar Mesa**              | Cumplido (`GestorRestaurante.intentar_asignar_mesa`)                      | Cumplido (`intentar_asignar_mesa`)                                                                  | **Cumplido**                                                                                                              |
| **Avanzar Tiempo**                     | Cumplido (`GestorRestaurante.avanzar_tiempo_simulacion`)                  | Cumplido (`avanzar_tiempo`)                                                                         | **Cumplido**                                                                                                              |
| **Revisar Cola de Espera**             | Cumplido (`GestorRestaurante.get_cola_espera_formateada`)                 | Cumplido (`revisar_cola_espera`)                                                                    | **Cumplido** (Muestra slots ocupados con info, no necesariamente *en orden* de prioridad mostrada)                       |
| **Revisar Estado de Mesas**            | Cumplido (`GestorRestaurante.get_estado_mesas_formateado`)                | Cumplido (`revisar_estado_mesas`)                                                                   | **Cumplido**                                                                                                              |
| **Mostrar Reglas Prioridad**           | Cumplido (`GestorRestaurante.get_reglas_prioridad_formateadas`)           | Cumplido (`mostrar_reglas_prioridad`)                                                               | **Cumplido**                                                                                                              |
| **Implementación Cola Prioridad Heap** | Cumplido (Clase `ColaPrioridadPropia` con heap manual)                    | Simulación por búsqueda lineal entre slots                                                          | **Simulado** (No es una estructura de heap real, cumple la *función* de encontrar al prioritario)                        |
| **Uso de Objetos/Clases**              | Extensivo (`Cliente`, `Mesa`, `SolicitudAsignacion`, `GestorRestaurante`) | No para colecciones, sí para agrupamiento conceptual (omitido en C primitiva por restricción)       | **Omitido** (por la estricta restricción de "primitivas", no se usaron `struct` ni arrays)                               |
| **Uso de Listas/Diccionarios**         | Extensivo (Para colecciones de mesas, indexación, etc.)                   | Ninguno (Manejo con variables individuales)                                                         | **Omitido** (por la estricta restricción)                                                                                 |
| **CLI Personalizada (Sin libs extra)** | Cumplido (Con `input`, `shlex`, `argparse`)                               | Cumplido (Con `fgets`, `sscanf`, `strcmp`)                                                          | **Cumplido**                                                                                                              |
| **Manejo de Argumentos CLI Robustos**  | Mejor (Con `shlex`, `argparse`)                                           | Básico (Con `sscanf`, frágil con espacios/formato)                                                | **Limitado** (No maneja espacios en nombres/tipos, depende de formato exacto)                                             |

## 9. Análisis y Comparación de Complejidad

Analicemos la complejidad temporal de las operaciones clave en el **peor caso** para ambas implementaciones, considerando N como el número de clientes en la cola de espera simulada y M como el número total de mesas.

| Operación                  | Implementación 1 (Python con Estructuras)         | Implementación 2 (C con Primitivas - 15 Slots)                      | Notas                                                                                                                               |
| :------------------------- | :------------------------------------------------ | :------------------------------------------------------------------ | :---------------------------------------------------------------------------------------------------------------------------------- |
| **Registrar Llegada**      | O(log N) (Inserción en el heap)                   | O(N) (Buscar el primer slot libre de 1 a 15)                        | Python: Eficiente gracias al heap. C: Ineficiente, busca linealmente un hueco. Para N=15, ambas son ~O(1) en la práctica, pero el algoritmo C es peor. |
| **Intentar Asignar Mesa**  | O(log N + M) (Pop del heap + Búsqueda lineal de mesa) | O(N + M) (Buscar prioritario entre 15 slots + Búsqueda lineal de mesa) | Python: `Peek`/`Pop` son logarítmicos. C: `Peek`/`Pop` simulados son O(N) buscando al prioritario. La búsqueda de mesa es O(M) en ambos. |
| **Revisar Cola (Visualizar)** | O(N log N) (Copiar y ordenar el heap)           | O(N) (Iterar y mostrar slots ocupados)                              | Python: Ordena para mostrar (costoso). C: Iteración simple, no garantiza ordenación mostrada (depende de los IFs). C es más rápido *si no se requiere la salida ordenada*. |
| **Revisar Mesas**          | O(M log M) (Ordenar lista de mesas)              | O(M) (Iterar y mostrar mesas)                                       | Python: Ordena las mesas por ID (costo de ordenación). C: Itera y muestra mesas en orden hardcodeado (constante).                   |

**Análisis Comparativo:**

1.  **Escalabilidad Algorítmica:**
    *   La Implementación 1 en Python exhibe una complejidad algorítmica (`log N`) para las operaciones de la cola de prioridad que escala mucho mejor a medida que el número de clientes en espera (N) crece. La búsqueda de mesa es O(M), pero M (número de mesas) es fijo y relativamente pequeño.
    *   La Implementación 2 en C, al no usar un heap, tiene operaciones clave (`Registrar`, `Intentar Asignar`) con complejidad O(N). Esto significa que si intentáramos (hipotéticamente, si el código no fuera tan largo) manejar 100 o 1000 clientes, el tiempo para estas operaciones escalaría linealmente con N, volviéndose mucho más lento que el crecimiento logarítmico de Python.
2.  **Complejidad Constante para N=15, M=12:** Para el tamaño fijo actual (N=15, M=12), los valores de `log N` y `N` son pequeños (`log2(15)` es aproximadamente 3.9). En la práctica, la diferencia de tiempo de ejecución entre O(log N) y O(N) para N=15 puede ser mínima o dominada por factores constantes (costo de copiar strings en C, overhead de objetos en Python). **Para este tamaño específico, ambas implementaciones podrían parecer "rápidas" o "constantes" en tiempo absoluto.** Sin embargo, el *algoritmo* en C es fundamentalmente menos eficiente para N grande.
3.  **Complejidad del Código:** La Implementación 2 en C es drásticamente más compleja, larga y propensa a errores de implementación que la Implementación 1 en Python debido a la necesidad de repetir lógica para cada variable individual y la falta de abstracciones.

---
