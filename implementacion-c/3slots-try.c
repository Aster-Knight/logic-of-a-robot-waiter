
#include <stdio.h>
#include <string.h> // 
#include <stdlib.h> // 
#include <time.h>   // Para inicializar el tiempo

// --- Definiciones y Constantes ---
#define MAX_NOMBRE_GRUPO 50
#define MAX_TIPO_CLIENTE 10
#define MAX_CLIENTES_EN_COLA_SIMULADA 3 // Límite de slots de clientes
#define NUM_MESAS 12
#define TIEMPO_ESPERA_MAX_MINS 15
#define MINUTOS_AVANCE_POR_DEFECTO 5

// --- Variables Globales para el Estado del Sistema (Simulación Primitiva) ---

// Estado del Tiempo de Simulación
int g_tiempo_simulacion_actual_minutos = 0;
int g_proximo_id_cliente_simulado = 1;
int g_clientes_en_cola_count = 0;

// --- Slots para Clientes en Espera (Máximo 3) ---
// Slot 1
int g_slot_cliente1_ocupado = 0; // 0 = libre, 1 = ocupado
char g_slot_cliente1_nombre[MAX_NOMBRE_GRUPO];
int g_slot_cliente1_hora_llegada_minutos;
int g_slot_cliente1_tiene_reserva; // 0 o 1
char g_slot_cliente1_tipo_cliente[MAX_TIPO_CLIENTE];
int g_slot_cliente1_numero_personas;
int g_slot_cliente1_id_simulado;
int g_slot_cliente1_prioridad_calculada;

// Slot 2
int g_slot_cliente2_ocupado = 0;
char g_slot_cliente2_nombre[MAX_NOMBRE_GRUPO];
int g_slot_cliente2_hora_llegada_minutos;
int g_slot_cliente2_tiene_reserva;
char g_slot_cliente2_tipo_cliente[MAX_TIPO_CLIENTE];
int g_slot_cliente2_numero_personas;
int g_slot_cliente2_id_simulado;
int g_slot_cliente2_prioridad_calculada;

// Slot 3
int g_slot_cliente3_ocupado = 0;
char g_slot_cliente3_nombre[MAX_NOMBRE_GRUPO];
int g_slot_cliente3_hora_llegada_minutos;
int g_slot_cliente3_tiene_reserva;
char g_slot_cliente3_tipo_cliente[MAX_TIPO_CLIENTE];
int g_slot_cliente3_numero_personas;
int g_slot_cliente3_id_simulado;
int g_slot_cliente3_prioridad_calculada;

// --- Estado de las Mesas (12 mesas) ---
// Mesas 1-4 (capacidad 6)
int g_mesa1_capacidad = 6; int g_mesa1_disponible = 1; int g_mesa1_cliente_asignado_id = -1; int g_mesa1_hora_asignacion_minutos = -1;
int g_mesa2_capacidad = 6; int g_mesa2_disponible = 1; int g_mesa2_cliente_asignado_id = -1; int g_mesa2_hora_asignacion_minutos = -1;
int g_mesa3_capacidad = 6; int g_mesa3_disponible = 1; int g_mesa3_cliente_asignado_id = -1; int g_mesa3_hora_asignacion_minutos = -1;
int g_mesa4_capacidad = 6; int g_mesa4_disponible = 1; int g_mesa4_cliente_asignado_id = -1; int g_mesa4_hora_asignacion_minutos = -1;

// Mesas 5-10 (capacidad 4)
int g_mesa5_capacidad = 4; int g_mesa5_disponible = 1; int g_mesa5_cliente_asignado_id = -1; int g_mesa5_hora_asignacion_minutos = -1;
int g_mesa6_capacidad = 4; int g_mesa6_disponible = 1; int g_mesa6_cliente_asignado_id = -1; int g_mesa6_hora_asignacion_minutos = -1;
int g_mesa7_capacidad = 4; int g_mesa7_disponible = 1; int g_mesa7_cliente_asignado_id = -1; int g_mesa7_hora_asignacion_minutos = -1;
int g_mesa8_capacidad = 4; int g_mesa8_disponible = 1; int g_mesa8_cliente_asignado_id = -1; int g_mesa8_hora_asignacion_minutos = -1;
int g_mesa9_capacidad = 4; int g_mesa9_disponible = 1; int g_mesa9_cliente_asignado_id = -1; int g_mesa9_hora_asignacion_minutos = -1;
int g_mesa10_capacidad = 4; int g_mesa10_disponible = 1; int g_mesa10_cliente_asignado_id = -1; int g_mesa10_hora_asignacion_minutos = -1;

// Mesas 11-12 (capacidad 2)
int g_mesa11_capacidad = 2; int g_mesa11_disponible = 1; int g_mesa11_cliente_asignado_id = -1; int g_mesa11_hora_asignacion_minutos = -1;
int g_mesa12_capacidad = 2; int g_mesa12_disponible = 1; int g_mesa12_cliente_asignado_id = -1; int g_mesa12_hora_asignacion_minutos = -1;


// --- Funciones Auxiliares ---
int calcular_prioridad_numerica(int tiene_reserva, int espera_excedida, const char* tipo_cliente, int hora_llegada) {
    // la variable hora de llegada no se usa directamente en la prioridad, pero si al momento de tener un empate de prioridades.
    // Inlcuirla genera una advertencia de compilacion, pero removerla implica refactorizar el código.
    int prioridad_res = tiene_reserva ? 0 : 100; // Con reserva es mejor (menor valor)
    int prioridad_esp = espera_excedida ? 0 : 50;  // Espera excedida es mejor
    int prioridad_tipo_val = 99;

    if (strcmp(tipo_cliente, "VIP") == 0) {
        prioridad_tipo_val = 10;
    } else if (strcmp(tipo_cliente, "REGULAR") == 0) {
        prioridad_tipo_val = 20;
    } else if (strcmp(tipo_cliente, "OCASIONAL") == 0) {
        prioridad_tipo_val = 30;
    }
    // La hora de llegada actúa como tie-breaker, pero el valor de prioridad debe ser un solo número.
    // Podríamos sumar la hora de llegada (o una función de ella) a la prioridad total.
    // Para simplificar, combinaremos los factores. Menor es mejor.
    // Ejemplo muy simple de combinación:
    return prioridad_res + prioridad_esp + prioridad_tipo_val;

}

// --- Funciones Principales de la Aplicación ---

void inicializar_sistema() {
    // Mesas ya están inicializadas con valores por defecto en sus declaraciones globales.
    // Inicializar slots de clientes
    g_slot_cliente1_ocupado = 0;
    g_slot_cliente2_ocupado = 0;
    g_slot_cliente3_ocupado = 0;

    g_tiempo_simulacion_actual_minutos = 0;
    g_proximo_id_cliente_simulado = 1;
    g_clientes_en_cola_count = 0;
    printf("Sistema de restaurante inicializado. Hora: %d mins.\n", g_tiempo_simulacion_actual_minutos);
}

void avanzar_tiempo(int minutos_a_avanzar, int intentar_asignacion_despues); // Declaración adelantada

void registrar_llegada_cliente(const char* nombre, int reserva_flag, const char* tipo, int personas) {
    avanzar_tiempo(MINUTOS_AVANCE_POR_DEFECTO, 0); // Avanzar tiempo, no intentar asignar aún

    if (personas < 1 || personas > 6) {
        printf("Error: Tamaño de grupo inválido (%d). Debe ser entre 1 y 6.\n", personas);
        return;
    }
    if (g_clientes_en_cola_count >= MAX_CLIENTES_EN_COLA_SIMULADA) {
        printf("Error: Lo sentimos, la lista de espera (slots) está llena.\n");
        return;
    }

    int slot_asignado = -1;
    if (!g_slot_cliente1_ocupado) slot_asignado = 1;
    else if (!g_slot_cliente2_ocupado) slot_asignado = 2;
    else if (!g_slot_cliente3_ocupado) slot_asignado = 3;

    if (slot_asignado != -1) {
        int cliente_id = g_proximo_id_cliente_simulado++;
        int hora_llegada = g_tiempo_simulacion_actual_minutos; // Usar tiempo actual de simulación
        int prioridad = calcular_prioridad_numerica(reserva_flag, 0, tipo, hora_llegada); // Espera no excedida al llegar

        if (slot_asignado == 1) {
            g_slot_cliente1_ocupado = 1;
            strcpy(g_slot_cliente1_nombre, nombre);
            g_slot_cliente1_hora_llegada_minutos = hora_llegada;
            g_slot_cliente1_tiene_reserva = reserva_flag;
            strcpy(g_slot_cliente1_tipo_cliente, tipo);
            g_slot_cliente1_numero_personas = personas;
            g_slot_cliente1_id_simulado = cliente_id;
            g_slot_cliente1_prioridad_calculada = prioridad;
        } else if (slot_asignado == 2) {
            g_slot_cliente2_ocupado = 1;
            strcpy(g_slot_cliente2_nombre, nombre);
            g_slot_cliente2_hora_llegada_minutos = hora_llegada;
            g_slot_cliente2_tiene_reserva = reserva_flag;
            strcpy(g_slot_cliente2_tipo_cliente, tipo);
            g_slot_cliente2_numero_personas = personas;
            g_slot_cliente2_id_simulado = cliente_id;
            g_slot_cliente2_prioridad_calculada = prioridad;
        } else if (slot_asignado == 3) {
            g_slot_cliente3_ocupado = 1;
            strcpy(g_slot_cliente3_nombre, nombre);
            g_slot_cliente3_hora_llegada_minutos = hora_llegada;
            g_slot_cliente3_tiene_reserva = reserva_flag;
            strcpy(g_slot_cliente3_tipo_cliente, tipo);
            g_slot_cliente3_numero_personas = personas;
            g_slot_cliente3_id_simulado = cliente_id;
            g_slot_cliente3_prioridad_calculada = prioridad;
        }
        g_clientes_en_cola_count++;
        printf("Grupo '%s' (ID: %d) añadido a la cola en slot %d. Prioridad inicial: %d.\n", nombre, cliente_id, slot_asignado, prioridad);
    } else {
        // Esto no debería pasar si g_clientes_en_cola_count < MAX_CLIENTES_EN_COLA_SIMULADA
        printf("Error interno: No se encontró slot libre aunque la cuenta indicaba espacio.\n");
    }
}

void intentar_asignar_mesa(); // Declaración adelantada

void liberar_mesa(int id_mesa) {
    int mesa_liberada_flag = 0;
    if (id_mesa == 1 && !g_mesa1_disponible) { g_mesa1_disponible = 1; g_mesa1_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 2 && !g_mesa2_disponible) { g_mesa2_disponible = 1; g_mesa2_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 3 && !g_mesa3_disponible) { g_mesa3_disponible = 1; g_mesa3_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 4 && !g_mesa4_disponible) { g_mesa4_disponible = 1; g_mesa4_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 5 && !g_mesa5_disponible) { g_mesa5_disponible = 1; g_mesa5_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 6 && !g_mesa6_disponible) { g_mesa6_disponible = 1; g_mesa6_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 7 && !g_mesa7_disponible) { g_mesa7_disponible = 1; g_mesa7_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 8 && !g_mesa8_disponible) { g_mesa8_disponible = 1; g_mesa8_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 9 && !g_mesa9_disponible) { g_mesa9_disponible = 1; g_mesa9_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 10 && !g_mesa10_disponible) { g_mesa10_disponible = 1; g_mesa10_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 11 && !g_mesa11_disponible) { g_mesa11_disponible = 1; g_mesa11_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else if (id_mesa == 12 && !g_mesa12_disponible) { g_mesa12_disponible = 1; g_mesa12_cliente_asignado_id = -1; mesa_liberada_flag = 1;}
    else {
        printf("Error: Mesa ID %d no encontrada o ya está disponible.\n", id_mesa);
        return;
    }

    if(mesa_liberada_flag){
        printf("Mesa %d liberada.\n", id_mesa);
        avanzar_tiempo(MINUTOS_AVANCE_POR_DEFECTO, 0); // Avanzar tiempo, no intentar asignar aún
        intentar_asignar_mesa(); // Intentar asignar después de liberar
    }
}

void avanzar_tiempo(int minutos_a_avanzar, int intentar_asignacion_despues) {
    if (minutos_a_avanzar > 0) {
        g_tiempo_simulacion_actual_minutos += minutos_a_avanzar;
        printf("Tiempo avanzado. Hora actual de simulación: %d mins.\n", g_tiempo_simulacion_actual_minutos);
    }
    if (intentar_asignacion_despues) {
        intentar_asignar_mesa();
    }
}


void intentar_asignar_mesa() {
    if (g_clientes_en_cola_count == 0) {
        printf("No hay clientes en espera para asignar mesa.\n");
        return;
    }

    // Actualizar prioridades basadas en tiempo de espera actual ANTES de seleccionar
    if(g_slot_cliente1_ocupado) {
        int espera1 = g_tiempo_simulacion_actual_minutos - g_slot_cliente1_hora_llegada_minutos;
        g_slot_cliente1_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente1_tiene_reserva, (espera1 >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente1_tipo_cliente, g_slot_cliente1_hora_llegada_minutos);
    }
    if(g_slot_cliente2_ocupado) {
        int espera2 = g_tiempo_simulacion_actual_minutos - g_slot_cliente2_hora_llegada_minutos;
        g_slot_cliente2_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente2_tiene_reserva, (espera2 >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente2_tipo_cliente, g_slot_cliente2_hora_llegada_minutos);
    }
    if(g_slot_cliente3_ocupado) {
        int espera3 = g_tiempo_simulacion_actual_minutos - g_slot_cliente3_hora_llegada_minutos;
        g_slot_cliente3_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente3_tiene_reserva, (espera3 >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente3_tipo_cliente, g_slot_cliente3_hora_llegada_minutos);
    }


    // Encontrar el cliente con la mayor prioridad (menor valor de prioridad_calculada)
    int mejor_prioridad = 999999;
    int slot_cliente_a_asignar = -1;

    if (g_slot_cliente1_ocupado && g_slot_cliente1_prioridad_calculada < mejor_prioridad) {
        mejor_prioridad = g_slot_cliente1_prioridad_calculada;
        slot_cliente_a_asignar = 1;
    }
    // Si hay empate de prioridad, la hora de llegada (menor) desempata
    else if (g_slot_cliente1_ocupado && g_slot_cliente1_prioridad_calculada == mejor_prioridad) {
        if (slot_cliente_a_asignar == 2 && g_slot_cliente1_hora_llegada_minutos < g_slot_cliente2_hora_llegada_minutos) slot_cliente_a_asignar = 1;
        else if (slot_cliente_a_asignar == 3 && g_slot_cliente1_hora_llegada_minutos < g_slot_cliente3_hora_llegada_minutos) slot_cliente_a_asignar = 1;
        // Si slot_cliente_a_asignar era -1, este es el primero, así que se asigna.
    }


    if (g_slot_cliente2_ocupado && g_slot_cliente2_prioridad_calculada < mejor_prioridad) {
        mejor_prioridad = g_slot_cliente2_prioridad_calculada;
        slot_cliente_a_asignar = 2;
    }
    else if (g_slot_cliente2_ocupado && g_slot_cliente2_prioridad_calculada == mejor_prioridad) {
        // Comprobar hora de llegada si la prioridad es igual
        if (slot_cliente_a_asignar == 1 && g_slot_cliente2_hora_llegada_minutos < g_slot_cliente1_hora_llegada_minutos) slot_cliente_a_asignar = 2;
        else if (slot_cliente_a_asignar == 3 && g_slot_cliente2_hora_llegada_minutos < g_slot_cliente3_hora_llegada_minutos) slot_cliente_a_asignar = 2;
        else if (slot_cliente_a_asignar == -1) slot_cliente_a_asignar = 2; // Si era el primero con esta prioridad
    }


    if (g_slot_cliente3_ocupado && g_slot_cliente3_prioridad_calculada < mejor_prioridad) {
        mejor_prioridad = g_slot_cliente3_prioridad_calculada;
        slot_cliente_a_asignar = 3;
    }
     else if (g_slot_cliente3_ocupado && g_slot_cliente3_prioridad_calculada == mejor_prioridad) {
        if (slot_cliente_a_asignar == 1 && g_slot_cliente3_hora_llegada_minutos < g_slot_cliente1_hora_llegada_minutos) slot_cliente_a_asignar = 3;
        else if (slot_cliente_a_asignar == 2 && g_slot_cliente3_hora_llegada_minutos < g_slot_cliente2_hora_llegada_minutos) slot_cliente_a_asignar = 3;
        else if (slot_cliente_a_asignar == -1) slot_cliente_a_asignar = 3;
    }


    if (slot_cliente_a_asignar == -1) {
        printf("No se pudo seleccionar un cliente prioritario (estado inesperado).\n");
        return;
    }

    // Extraer datos del cliente seleccionado
    int cliente_id, personas, hora_llegada;
    char nombre_cliente[MAX_NOMBRE_GRUPO];

    if (slot_cliente_a_asignar == 1) {
        cliente_id = g_slot_cliente1_id_simulado; personas = g_slot_cliente1_numero_personas; strcpy(nombre_cliente, g_slot_cliente1_nombre); hora_llegada = g_slot_cliente1_hora_llegada_minutos;
    } else if (slot_cliente_a_asignar == 2) {
        cliente_id = g_slot_cliente2_id_simulado; personas = g_slot_cliente2_numero_personas; strcpy(nombre_cliente, g_slot_cliente2_nombre); hora_llegada = g_slot_cliente2_hora_llegada_minutos;
    } else { // slot 3
        cliente_id = g_slot_cliente3_id_simulado; personas = g_slot_cliente3_numero_personas; strcpy(nombre_cliente, g_slot_cliente3_nombre); hora_llegada = g_slot_cliente3_hora_llegada_minutos;
    }

    int tiempo_esperando_actual_mins = g_tiempo_simulacion_actual_minutos - hora_llegada;
    int espera_excedida_actual = (tiempo_esperando_actual_mins >= TIEMPO_ESPERA_MAX_MINS);

    // Buscar mesa
    int mesa_asignada_id = -1;
    // Lógica extremadamente verbosa para buscar mesa...
    // Prioridad 1: Capacidad exacta
    if (g_mesa1_disponible && g_mesa1_capacidad == personas) mesa_asignada_id = 1;
    else if (g_mesa2_disponible && g_mesa2_capacidad == personas) mesa_asignada_id = 2;
    // ... (repetir para todas las mesas) ...
    else if (g_mesa12_disponible && g_mesa12_capacidad == personas) mesa_asignada_id = 12;

    // Prioridad 2: Capacidad ligeramente superior O cualquier capacidad si espera excedida
    if (mesa_asignada_id == -1) {
        // Para 6 personas
        if (personas <= 6) {
            if (g_mesa1_disponible && g_mesa1_capacidad >= personas && (espera_excedida_actual || (g_mesa1_capacidad - personas <=2))) { if(mesa_asignada_id == -1) mesa_asignada_id = 1;}
            if (g_mesa2_disponible && g_mesa2_capacidad >= personas && (espera_excedida_actual || (g_mesa2_capacidad - personas <=2))) { if(mesa_asignada_id == -1) mesa_asignada_id = 2;}
            if (g_mesa3_disponible && g_mesa3_capacidad >= personas && (espera_excedida_actual || (g_mesa3_capacidad - personas <=2))) { if(mesa_asignada_id == -1) mesa_asignada_id = 3;}
            if (g_mesa4_disponible && g_mesa4_capacidad >= personas && (espera_excedida_actual || (g_mesa4_capacidad - personas <=2))) { if(mesa_asignada_id == -1) mesa_asignada_id = 4;}
        }
        // Para 4 personas
        if (personas <= 4 && mesa_asignada_id == -1) { // Solo si no se encontró mesa de 6 para grupo pequeño
            if (g_mesa5_disponible && g_mesa5_capacidad >= personas && (espera_excedida_actual || (g_mesa5_capacidad - personas <=2))) { if(mesa_asignada_id == -1) mesa_asignada_id = 5;}
            if (g_mesa6_disponible && g_mesa6_capacidad >= personas && (espera_excedida_actual || (g_mesa6_capacidad - personas <=2))) { if(mesa_asignada_id == -1) mesa_asignada_id = 6;}
            // ... (repetir para mesas de 4) ...
            if (g_mesa10_disponible && g_mesa10_capacidad >= personas && (espera_excedida_actual || (g_mesa10_capacidad - personas <=2))) { if(mesa_asignada_id == -1) mesa_asignada_id = 10;}
        }
        // Para 2 personas
        if (personas <= 2 && mesa_asignada_id == -1) { // Solo si no se encontró mesa de 4 o 6 para grupo pequeño
            if (g_mesa11_disponible && g_mesa11_capacidad >= personas && (espera_excedida_actual || (g_mesa11_capacidad - personas <=2))) { if(mesa_asignada_id == -1) mesa_asignada_id = 11;}
            if (g_mesa12_disponible && g_mesa12_capacidad >= personas && (espera_excedida_actual || (g_mesa12_capacidad - personas <=2))) { if(mesa_asignada_id == -1) mesa_asignada_id = 12;}
        }
    }


    if (mesa_asignada_id != -1) {
        printf("Cliente ID %d ('%s') asignado a Mesa %d. Esperó %d mins.\n",
               cliente_id, nombre_cliente, mesa_asignada_id, tiempo_esperando_actual_mins);

        // Marcar mesa como ocupada y asignar cliente
        if(mesa_asignada_id == 1) {g_mesa1_disponible = 0; g_mesa1_cliente_asignado_id = cliente_id; g_mesa1_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 2) {g_mesa2_disponible = 0; g_mesa2_cliente_asignado_id = cliente_id; g_mesa2_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        // ... (repetir para todas las mesas) ...
        else if(mesa_asignada_id == 12) {g_mesa12_disponible = 0; g_mesa12_cliente_asignado_id = cliente_id; g_mesa12_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}


        // Liberar el slot del cliente
        if (slot_cliente_a_asignar == 1) g_slot_cliente1_ocupado = 0;
        else if (slot_cliente_a_asignar == 2) g_slot_cliente2_ocupado = 0;
        else if (slot_cliente_a_asignar == 3) g_slot_cliente3_ocupado = 0;
        g_clientes_en_cola_count--;
    } else {
        printf("No hay mesas adecuadas disponibles para Cliente ID %d ('%s'). Espera %d mins%s.\n",
               cliente_id, nombre_cliente, tiempo_esperando_actual_mins,
               espera_excedida_actual ? " (se consideraron mesas más grandes)" : "");
    }
}

void revisar_cola_espera() {
    printf("\n--- Cola de Espera (Simulada, Prioridad Actual) ---\n");
    if (g_clientes_en_cola_count == 0) {
        printf("La cola de espera está vacía.\n");
        return;
    }

    // Para mostrar en orden, necesitamos "ordenar" temporalmente los slots ocupados
    // Esto es muy ineficiente y verboso sin arrays/structs para la colección
    // Haremos una impresión simple, y el usuario infiere el orden por prioridad numérica

    // Actualizar prioridades para la visualización
    if(g_slot_cliente1_ocupado) {
        int espera1 = g_tiempo_simulacion_actual_minutos - g_slot_cliente1_hora_llegada_minutos;
        g_slot_cliente1_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente1_tiene_reserva, (espera1 >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente1_tipo_cliente, g_slot_cliente1_hora_llegada_minutos);
    }
    // ... (repetir para slot 2 y 3) ...
     if(g_slot_cliente2_ocupado) {
        int espera2 = g_tiempo_simulacion_actual_minutos - g_slot_cliente2_hora_llegada_minutos;
        g_slot_cliente2_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente2_tiene_reserva, (espera2 >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente2_tipo_cliente, g_slot_cliente2_hora_llegada_minutos);
    }
    if(g_slot_cliente3_ocupado) {
        int espera3 = g_tiempo_simulacion_actual_minutos - g_slot_cliente3_hora_llegada_minutos;
        g_slot_cliente3_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente3_tiene_reserva, (espera3 >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente3_tipo_cliente, g_slot_cliente3_hora_llegada_minutos);
    }


    // Imprimir cada slot ocupado
    printf("Slot | ID | Nombre        | Personas | Reserva | Tipo      | Llegada(m) | Espera(m) | Prioridad\n");
    printf("---------------------------------------------------------------------------------------------\n");
    if (g_slot_cliente1_ocupado) {
        int espera1 = g_tiempo_simulacion_actual_minutos - g_slot_cliente1_hora_llegada_minutos;
        printf("1    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n",
               g_slot_cliente1_id_simulado, g_slot_cliente1_nombre, g_slot_cliente1_numero_personas,
               g_slot_cliente1_tiene_reserva ? "Si" : "No", g_slot_cliente1_tipo_cliente,
               g_slot_cliente1_hora_llegada_minutos, espera1, g_slot_cliente1_prioridad_calculada,
               (espera1 >= TIEMPO_ESPERA_MAX_MINS) ? "(EXC)" : "");
    }
    if (g_slot_cliente2_ocupado) {
        int espera2 = g_tiempo_simulacion_actual_minutos - g_slot_cliente2_hora_llegada_minutos;
        printf("2    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n",
               g_slot_cliente2_id_simulado, g_slot_cliente2_nombre, g_slot_cliente2_numero_personas,
               g_slot_cliente2_tiene_reserva ? "Si" : "No", g_slot_cliente2_tipo_cliente,
               g_slot_cliente2_hora_llegada_minutos, espera2, g_slot_cliente2_prioridad_calculada,
               (espera2 >= TIEMPO_ESPERA_MAX_MINS) ? "(EXC)" : "");
    }
    if (g_slot_cliente3_ocupado) {
        int espera3 = g_tiempo_simulacion_actual_minutos - g_slot_cliente3_hora_llegada_minutos;
        printf("3    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n",
               g_slot_cliente3_id_simulado, g_slot_cliente3_nombre, g_slot_cliente3_numero_personas,
               g_slot_cliente3_tiene_reserva ? "Si" : "No", g_slot_cliente3_tipo_cliente,
               g_slot_cliente3_hora_llegada_minutos, espera3, g_slot_cliente3_prioridad_calculada,
               (espera3 >= TIEMPO_ESPERA_MAX_MINS) ? "(EXC)" : "");
    }
    printf("---------------------------------------------------------------------------------------------\n");

}

void revisar_estado_mesas() {
    printf("\n--- Estado de las Mesas ---\n");
    printf("ID | Capacidad | Disponible | Cliente ID Asignado | Hora Asignación (mins)\n");
    printf("----------------------------------------------------------------------\n");
    printf("%2d | %9d | %10s | %19d | %20d\n", 1, g_mesa1_capacidad, g_mesa1_disponible?"Si":"No", g_mesa1_cliente_asignado_id, g_mesa1_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 2, g_mesa2_capacidad, g_mesa2_disponible?"Si":"No", g_mesa2_cliente_asignado_id, g_mesa2_hora_asignacion_minutos);
    // ... (repetir para todas las 12 mesas) ...
    printf("%2d | %9d | %10s | %19d | %20d\n", 3, g_mesa3_capacidad, g_mesa3_disponible?"Si":"No", g_mesa3_cliente_asignado_id, g_mesa3_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 4, g_mesa4_capacidad, g_mesa4_disponible?"Si":"No", g_mesa4_cliente_asignado_id, g_mesa4_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 5, g_mesa5_capacidad, g_mesa5_disponible?"Si":"No", g_mesa5_cliente_asignado_id, g_mesa5_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 6, g_mesa6_capacidad, g_mesa6_disponible?"Si":"No", g_mesa6_cliente_asignado_id, g_mesa6_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 7, g_mesa7_capacidad, g_mesa7_disponible?"Si":"No", g_mesa7_cliente_asignado_id, g_mesa7_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 8, g_mesa8_capacidad, g_mesa8_disponible?"Si":"No", g_mesa8_cliente_asignado_id, g_mesa8_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 9, g_mesa9_capacidad, g_mesa9_disponible?"Si":"No", g_mesa9_cliente_asignado_id, g_mesa9_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 10, g_mesa10_capacidad, g_mesa10_disponible?"Si":"No", g_mesa10_cliente_asignado_id, g_mesa10_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 11, g_mesa11_capacidad, g_mesa11_disponible?"Si":"No", g_mesa11_cliente_asignado_id, g_mesa11_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 12, g_mesa12_capacidad, g_mesa12_disponible?"Si":"No", g_mesa12_cliente_asignado_id, g_mesa12_hora_asignacion_minutos);

    printf("----------------------------------------------------------------------\n");
}

void mostrar_reglas_prioridad() {
    printf("\n--- Reglas de Prioridad (Menor valor numérico = Mayor Prioridad) ---\n");
    printf("1. Con Reserva (peso: 0) > Sin Reserva (peso: 100)\n");
    printf("2. Espera >= 15 mins (peso: 0) > Espera < 15 mins (peso: 50)\n");
    printf("3. Tipo Cliente: VIP (peso: 10) > REGULAR (peso: 20) > OCASIONAL (peso: 30)\n");
    printf("La prioridad total es la suma de estos pesos. La hora de llegada desempata (FIFO implícito al seleccionar).\n");
    printf("Si un grupo espera >= 15 minutos, se considerarán mesas más grandes.\n");
    printf("----------------------------------------------------------------------\n");
}


// --- Bucle Principal de la CLI ---
int main() {
    char linea_comando[256];
    char comando[50];
    char arg1[MAX_NOMBRE_GRUPO]; // Para nombre de grupo o ID de mesa
    char arg2[MAX_TIPO_CLIENTE]; // Para tipo de cliente o flag de reserva
    int arg_num1; // Para número de personas o minutos
    int arg_num2; // Para flag de reserva (0 o 1)

    inicializar_sistema();

    printf("\n--- Robot Mesero Primitivo CLI ---\n");
    printf("Comandos: register, free, advance, queue, tables, rules, exit\n");
    printf("Para 'register': register <NombreGrupo> <TieneReserva(0/1)> <TipoCliente(VIP/REGULAR/OCASIONAL)> <NumPersonas>\n");
    printf("  (Nota: Nombres y tipos SIN ESPACIOS para esta versión primitiva)\n");
    printf("Para 'free': free <ID_Mesa>\n");
    printf("Para 'advance': advance [minutos] (por defecto 5 si no se especifica)\n");


    while (1) {
        printf("\nPrimitivoCLI > ");
        if (fgets(linea_comando, sizeof(linea_comando), stdin) == NULL) {
            break; // EOF
        }
        linea_comando[strcspn(linea_comando, "\n")] = 0; // Quitar newline

        // Parseo muy básico de comandos
        int num_args_leidos = sscanf(linea_comando, "%s %s %s %d %d", comando, arg1, arg2, &arg_num1, &arg_num2);
        // sscanf es limitado. Para register se necesitan 4 args después del comando.

        if (strcmp(comando, "exit") == 0) {
            printf("Saliendo...\n");
            break;
        } else if (strcmp(comando, "register") == 0) {
            // Re-parsear específicamente para register, esperando 4 argumentos
            char nombre_temp[MAX_NOMBRE_GRUPO];
            int reserva_temp;
            char tipo_temp[MAX_TIPO_CLIENTE];
            int personas_temp;
            if (sscanf(linea_comando, "%*s %s %d %s %d", nombre_temp, &reserva_temp, tipo_temp, &personas_temp) == 4) {
                registrar_llegada_cliente(nombre_temp, reserva_temp, tipo_temp, personas_temp);
            } else {
                printf("Uso: register <NombreGrupo> <TieneReserva(0/1)> <TipoCliente> <NumPersonas>\n");
            }
        } else if (strcmp(comando, "free") == 0) {
            if (num_args_leidos >= 2 && sscanf(arg1, "%d", &arg_num1) == 1) { // arg1 es el ID de mesa
                liberar_mesa(arg_num1);
            } else {
                printf("Uso: free <ID_Mesa>\n");
            }
        } else if (strcmp(comando, "advance") == 0) {
            int minutos_adv = MINUTOS_AVANCE_POR_DEFECTO;
            if (num_args_leidos >= 2 && sscanf(arg1, "%d", &minutos_adv) == 1) { // arg1 son los minutos
                // minutos_adv ya tiene el valor
            } else if (num_args_leidos == 1) {
                // usa el valor por defecto
            } else {
                 printf("Uso: advance [minutos]\n");
                 continue;
            }
            if(minutos_adv < 0) {printf("Los minutos deben ser no negativos.\n"); continue;}
            avanzar_tiempo(minutos_adv, 1); // Siempre intentar asignar después de avanzar tiempo manualmente
        } else if (strcmp(comando, "queue") == 0) {
            revisar_cola_espera();
        } else if (strcmp(comando, "tables") == 0) {
            revisar_estado_mesas();
        } else if (strcmp(comando, "rules") == 0) {
            mostrar_reglas_prioridad();
        } else if (strcmp(comando, "assign") == 0) { // Comando manual para intentar asignar
             intentar_asignar_mesa();
        }
        else {
            printf("Comando desconocido: '%s'. Comandos válidos: register, free, advance, queue, tables, rules, assign, exit\n", comando);
        }
    }

    return 0;
}