
# logic-of-a-robot-waiter

<!-- Una descripción concisa del proyecto. -->
> Implementación comparativa en Python y C de un sistema de asignación de mesas para un robot recepcionista, priorizando clientes usando una cola de prioridad.

---

## Descripción del Proyecto

Este proyecto desarrolla un sistema central para un robot recepcionista en un restaurante, encargado de gestionar la fila de espera y asignar mesas a los clientes. El núcleo del sistema es una lógica de priorización basada en múltiples criterios: hora de llegada, estado de reserva y tipo de cliente (VIP, regular, ocasional).

El proyecto tiene un objetivo de estudio comparativo fundamental: implementar la misma lógica de negocio utilizando **dos enfoques de programación radicalmente distintos**:

1.  **Implementación 1 (Python - Estructuras de Datos y MVC):** Utilizando estructuras de datos de alto nivel (clases, listas, diccionarios) y una implementación **propia** de una Cola de Prioridad basada en heap manual. Sigue un patrón MVC y presenta una CLI amigable.
2.  **Implementación 2 (C - Variables Primitivas y Procedural):** Restringiéndose estrictamente al uso de variables de tipo primitivo y estructuras de control básicas, simulando las entidades y la lógica sin colecciones dinámicas ni abstracciones de datos complejas.

Ambas implementaciones buscan demostrar el impacto en la complejidad del código, la escalabilidad, la legibilidad y la mantenibilidad al abordar el mismo problema con diferentes herramientas de abstracción de datos.

## Configuración del Restaurante Simulado

Ambas implementaciones operan bajo la misma configuración y reglas:

*   **Mesas:** 12 mesas con capacidades fijas: 4x (6 personas), 6x (4 personas), 2x (2 personas).
*   **Tamaño de Grupos:** Se aceptan grupos de 1 a 6 personas. Grupos más grandes no son admitidos.
*   **Capacidad de la Cola de Espera:** Máximo 15 grupos en espera. Grupos adicionales son rechazados.
*   **Tiempo de Espera Crítico:** 15 minutos. Exceder este tiempo altera la prioridad y la lógica de asignación de mesa.
*   **Avance del Tiempo:** El tiempo de simulación avanza en incrementos (por defecto, 5 minutos por ciertas operaciones o comando `time`/`advance`).

## Arquitectura y Representación de Datos

### Implementación 1 (Python con Estructuras)

*   **Arquitectura:** Sigue el patrón Modelo-Vista-Controlador (MVC).
    *   **Modelo:** Clases (`Cliente`, `Mesa`, `SolicitudAsignacion`), Cola de Prioridad propia (`ColaPrioridadPropia` basada en heap manual), y la lógica principal (`GestorRestaurante`).
    *   **Vista:** Interacción con la Consola (`VistaConsola`), incluyendo formateo mejorado (tablas, arte ASCII).
    *   **Controlador:** Gestiona el flujo y la interacción con la CLI personalizada (`ControladorAplicacion`).
*   **Representación:**
    *   Entidades (`Cliente`, `Mesa`, `SolicitudAsignacion`) son representadas por **objetos de clase**.
    *   Colecciones (cola de espera, lista de mesas) son gestionadas con **listas y la estructura `ColaPrioridadPropia` (heap manual)**.
    *   El estado del sistema se mantiene en atributos de la clase `GestorRestaurante`.

### Implementación 2 (C con Primitivas)

*   **Arquitectura:** No sigue un patrón formal (Procedural). Toda la lógica opera directamente sobre variables de estado globales.
*   **Representación:**
    *   El estado completo del sistema (15 slots de cliente, 12 mesas) se mantiene utilizando únicamente **variables globales o estáticas de tipo primitivo** (int, float, char[], bool simulado con int).
    *   Cada atributo de cada slot de cliente y cada mesa tiene su propia variable hardcodeada (ej. `g_slot_cliente1_nombre`, `g_mesa5_disponible`).
    *   La "cola de prioridad" y la "gestión de mesas" son simuladas a través de la lógica de las funciones que acceden y manipulan estas variables fijas. No hay estructuras de datos de colección dinámicas.

## Estructura del Proyecto

El repositorio se organiza para separar las dos implementaciones y otros archivos del proyecto:

```
logic-of-a-robot-waiter/
├── implementacion-c/            # Código fuente de la Implementación 2 (C con Primitivas)
│   └── restaurante-15slots.c    # Implementación principal en C
│   └── readme.md                # Documentación específica de la implementación en C (Adjunta en chat)
│   └── 3slots-try.c             # (Opcional) Archivo de prueba o versión preliminar
├── implementacion-compleja/     # Código fuente de la Implementación 1 (Python con Estructuras)
│   ├── src/                     # Directorio principal del código Python
│   │   ├── __init__.py
│   │   ├── cliente.py
│   │   ├── mesa.py
│   │   ├── solicitud_asignacion.py
│   │   ├── cola_prioridad_propia.py  # <-- Implementación manual del Heap
│   │   ├── gestor_restaurante.py
│   │   ├── vista_consola.py
│   │   └── controlador_aplicacion.py
│   └── main.py                     # <-- Punto de entrada para la Implementación 1
│   └── readme.md                   # Documentación específica de la implementación en Python (Este documento)
├── pre-proyecto/                # Archivos del anteproyecto
│   └── pre-proyecto.md
│   └── pre-proyecto.pdf
├── LICENSE                      # Información de la licencia (MIT)
└── README.md                    # Este archivo
```
*   **Ignorados:** Archivos de Git (`.git/`, `.gitattributes`) y cachés de Python (`__pycache__`).

## Cómo Ejecutar las Implementaciones

### Implementación 1 (Python con Estructuras)

*   **Requisitos:** Python 3.7+ (usa solo módulos estándar).
*   **Ejecución:**
    1.  Navega al directorio de la implementación en Python: `cd implementacion-compleja`
    2.  Ejecuta el script principal: `python main.py`
*   **Uso de la CLI (Python):**
    Verás el prompt `RestauranteCLI >`. Escribe `help` para ver los comandos. Los comandos y alias principales son:
    *   `add <nombre> -p <num> [-r] [-t <tipo>]`: Registra un grupo. Alias: `ad`, `add-group`. Ejemplo: `add "Familia Lopez" -p 4 -r -t VIP`
    *   `free <id_mesa>`: Libera una mesa. Alias: `fr`, `free-table`. Ejemplo: `free 7`
    *   `assign`: Intenta asignar mesa al prioritario. Alias: `as`, `assign-table`.
    *   `time [<minutos>]`: Avanza el tiempo. Alias: `ti`, `advance`, `advance-time`. Ejemplo: `time 10`
    *   `queue`: Muestra cola (tabla). Alias: `qu`, `view-queue`.
    *   `tables`: Muestra mesas (tabla). Alias: `ta`, `view-tables`.
    *   `rules`: Muestra reglas. Alias: `ru`, `view-rules`.
    *   `status`: Muestra resumen. Alias: `st`.
    *   `base-case`: Configura un caso de prueba. Alias: `bc`, `setup-base`.
    *   `help`: Muestra ayuda.
    *   `exit`: Sale.
    *   **Nota:** Encierra nombres o rutas con espacios entre comillas (`"`).

### Implementación 2 (C con Primitivas)

*   **Requisitos:** Compilador C (GCC, Clang, etc.), Sistema Operativo compatible con E/S de consola estándar. Usa solo librerías estándar de C.
*   **Ejecución:**
    1.  Navega al directorio de la implementación en C: `cd implementacion-c`
    2.  Compila el código (ej. con GCC): `gcc restaurante-15slots.c -o robot_mesero_primitivo`
    3.  Ejecuta el archivo compilado: `./robot_mesero_primitivo` (o `robot_mesero_primitivo.exe` en Windows)
*   **Uso de la CLI (C):**
    Verás el prompt `PrimitivoCLI >`. Los comandos son:
    *   `register <NombreGrupo> <TieneReserva(0/1)> <TipoCliente(VIP/REGULAR/OCASIONAL)> <NumPersonas>`: Registra un grupo. Ejemplo: `register FamiliaGarcia 1 VIP 4`
        *   **Nota Crítica:** `NombreGrupo` y `TipoCliente` **NO deben contener espacios** debido al parseo simple.
    *   `free <ID_Mesa>`: Libera una mesa. Ejemplo: `free 5`
    *   `advance [minutos]`: Avanza el tiempo. Ejemplo: `advance 10`
    *   `queue`: Muestra estado de slots.
    *   `tables`: Muestra estado de mesas.
    *   `rules`: Muestra reglas de prioridad.
    *   `assign`: Intenta asignar mesa al prioritario.
    *   `exit`: Sale.

## Comparación de Implementaciones

Se presenta una comparación detallada de las características implementadas y la complejidad observada entre la Implementación 1 (Python) y la Implementación 2 (C), reflejando las limitaciones y capacidades de cada enfoque.

## Comparación de Funcionalidades y Requisitos Cumplidos

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

## Análisis y Comparación de Complejidad

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


**Análisis Comparativo de Complejidad:**

Para el tamaño fijo y pequeño de 15 clientes y 12 mesas, ambas implementaciones pueden parecer rápidas en tiempo absoluto. Sin embargo, el análisis algorítmico revela que la Implementación 1 en Python, al utilizar una Cola de Prioridad basada en Heap (O(log N)), es fundamentalmente más escalable y eficiente para un número grande (N) de clientes en espera que el enfoque O(N) de la Implementación 2 en C para operaciones clave como Registrar o Intentar Asignar.

**Complejidad del Código:** La Implementación 2 en C es drásticamente más compleja, larga y propensa a errores de implementación que la Implementación 1 en Python, debido a la manipulación directa de variables primitivas y la falta de abstracciones.

## Licencia

Este proyecto está distribuido bajo la Licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

## Contacto

Jonatan Aguilar (Aster-Knight) - josue.aguilar@ufm.edu

---
---