#include <stdio.h>
#include <string.h> // Para strcpy, strcmp, strlen
#include <stdlib.h> // Para atoi (si es necesario)
#include <time.h>   // Para inicializar el tiempo (opcional)

// --- Definiciones y Constantes ---
#define MAX_NOMBRE_GRUPO 50
#define MAX_TIPO_CLIENTE 10
#define MAX_CLIENTES_EN_COLA_SIMULADA 15 // Límite de slots de clientes
#define NUM_MESAS 12
#define TIEMPO_ESPERA_MAX_MINS 15
#define MINUTOS_AVANCE_POR_DEFECTO 5

// --- Variables Globales para el Estado del Sistema (Simulación Primitiva) ---

// Estado del Tiempo de Simulación
int g_tiempo_simulacion_actual_minutos = 0;
int g_proximo_id_cliente_simulado = 1;
int g_clientes_en_cola_count = 0;

// --- Slots para Clientes en Espera (Máximo 15) ---
// Slot 1
int g_slot_cliente1_ocupado = 0; char g_slot_cliente1_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente1_hora_llegada_minutos; int g_slot_cliente1_tiene_reserva; char g_slot_cliente1_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente1_numero_personas; int g_slot_cliente1_id_simulado; int g_slot_cliente1_prioridad_calculada;
// Slot 2
int g_slot_cliente2_ocupado = 0; char g_slot_cliente2_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente2_hora_llegada_minutos; int g_slot_cliente2_tiene_reserva; char g_slot_cliente2_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente2_numero_personas; int g_slot_cliente2_id_simulado; int g_slot_cliente2_prioridad_calculada;
// Slot 3
int g_slot_cliente3_ocupado = 0; char g_slot_cliente3_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente3_hora_llegada_minutos; int g_slot_cliente3_tiene_reserva; char g_slot_cliente3_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente3_numero_personas; int g_slot_cliente3_id_simulado; int g_slot_cliente3_prioridad_calculada;
// Slot 4
int g_slot_cliente4_ocupado = 0; char g_slot_cliente4_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente4_hora_llegada_minutos; int g_slot_cliente4_tiene_reserva; char g_slot_cliente4_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente4_numero_personas; int g_slot_cliente4_id_simulado; int g_slot_cliente4_prioridad_calculada;
// Slot 5
int g_slot_cliente5_ocupado = 0; char g_slot_cliente5_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente5_hora_llegada_minutos; int g_slot_cliente5_tiene_reserva; char g_slot_cliente5_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente5_numero_personas; int g_slot_cliente5_id_simulado; int g_slot_cliente5_prioridad_calculada;
// Slot 6
int g_slot_cliente6_ocupado = 0; char g_slot_cliente6_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente6_hora_llegada_minutos; int g_slot_cliente6_tiene_reserva; char g_slot_cliente6_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente6_numero_personas; int g_slot_cliente6_id_simulado; int g_slot_cliente6_prioridad_calculada;
// Slot 7
int g_slot_cliente7_ocupado = 0; char g_slot_cliente7_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente7_hora_llegada_minutos; int g_slot_cliente7_tiene_reserva; char g_slot_cliente7_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente7_numero_personas; int g_slot_cliente7_id_simulado; int g_slot_cliente7_prioridad_calculada;
// Slot 8
int g_slot_cliente8_ocupado = 0; char g_slot_cliente8_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente8_hora_llegada_minutos; int g_slot_cliente8_tiene_reserva; char g_slot_cliente8_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente8_numero_personas; int g_slot_cliente8_id_simulado; int g_slot_cliente8_prioridad_calculada;
// Slot 9
int g_slot_cliente9_ocupado = 0; char g_slot_cliente9_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente9_hora_llegada_minutos; int g_slot_cliente9_tiene_reserva; char g_slot_cliente9_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente9_numero_personas; int g_slot_cliente9_id_simulado; int g_slot_cliente9_prioridad_calculada;
// Slot 10
int g_slot_cliente10_ocupado = 0; char g_slot_cliente10_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente10_hora_llegada_minutos; int g_slot_cliente10_tiene_reserva; char g_slot_cliente10_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente10_numero_personas; int g_slot_cliente10_id_simulado; int g_slot_cliente10_prioridad_calculada;
// Slot 11
int g_slot_cliente11_ocupado = 0; char g_slot_cliente11_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente11_hora_llegada_minutos; int g_slot_cliente11_tiene_reserva; char g_slot_cliente11_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente11_numero_personas; int g_slot_cliente11_id_simulado; int g_slot_cliente11_prioridad_calculada;
// Slot 12
int g_slot_cliente12_ocupado = 0; char g_slot_cliente12_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente12_hora_llegada_minutos; int g_slot_cliente12_tiene_reserva; char g_slot_cliente12_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente12_numero_personas; int g_slot_cliente12_id_simulado; int g_slot_cliente12_prioridad_calculada;
// Slot 13
int g_slot_cliente13_ocupado = 0; char g_slot_cliente13_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente13_hora_llegada_minutos; int g_slot_cliente13_tiene_reserva; char g_slot_cliente13_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente13_numero_personas; int g_slot_cliente13_id_simulado; int g_slot_cliente13_prioridad_calculada;
// Slot 14
int g_slot_cliente14_ocupado = 0; char g_slot_cliente14_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente14_hora_llegada_minutos; int g_slot_cliente14_tiene_reserva; char g_slot_cliente14_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente14_numero_personas; int g_slot_cliente14_id_simulado; int g_slot_cliente14_prioridad_calculada;
// Slot 15
int g_slot_cliente15_ocupado = 0; char g_slot_cliente15_nombre[MAX_NOMBRE_GRUPO]; int g_slot_cliente15_hora_llegada_minutos; int g_slot_cliente15_tiene_reserva; char g_slot_cliente15_tipo_cliente[MAX_TIPO_CLIENTE]; int g_slot_cliente15_numero_personas; int g_slot_cliente15_id_simulado; int g_slot_cliente15_prioridad_calculada;


// --- Estado de las Mesas (12 mesas) ---
int g_mesa1_capacidad = 6; int g_mesa1_disponible = 1; int g_mesa1_cliente_asignado_id = -1; int g_mesa1_hora_asignacion_minutos = -1;
int g_mesa2_capacidad = 6; int g_mesa2_disponible = 1; int g_mesa2_cliente_asignado_id = -1; int g_mesa2_hora_asignacion_minutos = -1;
int g_mesa3_capacidad = 6; int g_mesa3_disponible = 1; int g_mesa3_cliente_asignado_id = -1; int g_mesa3_hora_asignacion_minutos = -1;
int g_mesa4_capacidad = 6; int g_mesa4_disponible = 1; int g_mesa4_cliente_asignado_id = -1; int g_mesa4_hora_asignacion_minutos = -1;
int g_mesa5_capacidad = 4; int g_mesa5_disponible = 1; int g_mesa5_cliente_asignado_id = -1; int g_mesa5_hora_asignacion_minutos = -1;
int g_mesa6_capacidad = 4; int g_mesa6_disponible = 1; int g_mesa6_cliente_asignado_id = -1; int g_mesa6_hora_asignacion_minutos = -1;
int g_mesa7_capacidad = 4; int g_mesa7_disponible = 1; int g_mesa7_cliente_asignado_id = -1; int g_mesa7_hora_asignacion_minutos = -1;
int g_mesa8_capacidad = 4; int g_mesa8_disponible = 1; int g_mesa8_cliente_asignado_id = -1; int g_mesa8_hora_asignacion_minutos = -1;
int g_mesa9_capacidad = 4; int g_mesa9_disponible = 1; int g_mesa9_cliente_asignado_id = -1; int g_mesa9_hora_asignacion_minutos = -1;
int g_mesa10_capacidad = 4; int g_mesa10_disponible = 1; int g_mesa10_cliente_asignado_id = -1; int g_mesa10_hora_asignacion_minutos = -1;
int g_mesa11_capacidad = 2; int g_mesa11_disponible = 1; int g_mesa11_cliente_asignado_id = -1; int g_mesa11_hora_asignacion_minutos = -1;
int g_mesa12_capacidad = 2; int g_mesa12_disponible = 1; int g_mesa12_cliente_asignado_id = -1; int g_mesa12_hora_asignacion_minutos = -1;


// --- Funciones Auxiliares ---
// Calcula un valor numérico de prioridad basado en el estado de la reserva, tiempo de espera excedido y tipo de cliente.
// Un valor numérico menor indica mayor prioridad.
int calcular_prioridad_numerica(int tiene_reserva, int espera_excedida, const char* tipo_cliente) {
    int prioridad_res = tiene_reserva ? 0 : 200; // Reserva tiene mayor influencia (0 vs 200)
    int prioridad_esp = espera_excedida ? 0 : 100; // Tiempo de espera excedido es la segunda mayor influencia (0 vs 100)
    int prioridad_tipo_val = 999; // Valor base alto (baja prioridad)

    // --- CAMBIO: VIP con el menor valor numérico (mayor prioridad) ---
    if (strcmp(tipo_cliente, "VIP") == 0) {
        prioridad_tipo_val = 10; // VIP es el tipo más importante
    } else if (strcmp(tipo_cliente, "REGULAR") == 0) {
        prioridad_tipo_val = 20;
    } else if (strcmp(tipo_cliente, "OCASIONAL") == 0) {
        prioridad_tipo_val = 30;
    }
    return prioridad_res + prioridad_esp + prioridad_tipo_val;
}

// --- Funciones Principales de la Aplicación ---

// Inicializa el estado del sistema (slots de clientes y variables de estado).
void inicializar_sistema() {
    // --- CAMBIO: Inicializar 15 slots de clientes ---
    g_slot_cliente1_ocupado = 0; g_slot_cliente2_ocupado = 0; g_slot_cliente3_ocupado = 0;
    g_slot_cliente4_ocupado = 0; g_slot_cliente5_ocupado = 0; g_slot_cliente6_ocupado = 0;
    g_slot_cliente7_ocupado = 0; g_slot_cliente8_ocupado = 0; g_slot_cliente9_ocupado = 0;
    g_slot_cliente10_ocupado = 0; g_slot_cliente11_ocupado = 0; g_slot_cliente12_ocupado = 0;
    g_slot_cliente13_ocupado = 0; g_slot_cliente14_ocupado = 0; g_slot_cliente15_ocupado = 0;

    // Las mesas ya están inicializadas con valores por defecto en sus declaraciones globales.

    g_tiempo_simulacion_actual_minutos = 0;
    g_proximo_id_cliente_simulado = 1;
    g_clientes_en_cola_count = 0;
    printf("Sistema de restaurante inicializado. Hora: %d mins.\n", g_tiempo_simulacion_actual_minutos);
}

void avanzar_tiempo(int minutos_a_avanzar, int intentar_asignacion_despues); // Declaración adelantada
void intentar_asignar_mesa(); // Declaración adelantada

// Registra la llegada de un nuevo grupo de clientes y los añade a un slot disponible.
void registrar_llegada_cliente(const char* nombre, int reserva_flag, const char* tipo, int personas) {
    // Avanzar tiempo antes de procesar la llegada (según diseño)
    avanzar_tiempo(MINUTOS_AVANCE_POR_DEFECTO, 0); // Avanzar tiempo, no intentar asignar aún

    if (personas < 1 || personas > 6) {
        printf("Error: Tamaño de grupo inválido (%d). Debe ser entre 1 y 6.\n", personas);
        return;
    }
    if (g_clientes_en_cola_count >= MAX_CLIENTES_EN_COLA_SIMULADA) {
        printf("Error: Lo sentimos, la lista de espera (%d slots) está llena.\n", MAX_CLIENTES_EN_COLA_SIMULADA);
        return;
    }

    // --- CAMBIO: Buscar un slot libre entre 15 slots ---
    int slot_asignado = -1;
    if (!g_slot_cliente1_ocupado) slot_asignado = 1;
    else if (!g_slot_cliente2_ocupado) slot_asignado = 2;
    else if (!g_slot_cliente3_ocupado) slot_asignado = 3;
    else if (!g_slot_cliente4_ocupado) slot_asignado = 4;
    else if (!g_slot_cliente5_ocupado) slot_asignado = 5;
    else if (!g_slot_cliente6_ocupado) slot_asignado = 6;
    else if (!g_slot_cliente7_ocupado) slot_asignado = 7;
    else if (!g_slot_cliente8_ocupado) slot_asignado = 8;
    else if (!g_slot_cliente9_ocupado) slot_asignado = 9;
    else if (!g_slot_cliente10_ocupado) slot_asignado = 10;
    else if (!g_slot_cliente11_ocupado) slot_asignado = 11;
    else if (!g_slot_cliente12_ocupado) slot_asignado = 12;
    else if (!g_slot_cliente13_ocupado) slot_asignado = 13;
    else if (!g_slot_cliente14_ocupado) slot_asignado = 14;
    else if (!g_slot_cliente15_ocupado) slot_asignado = 15;


    if (slot_asignado != -1) {
        int cliente_id = g_proximo_id_cliente_simulado++;
        int hora_llegada = g_tiempo_simulacion_actual_minutos; // Usar tiempo actual de simulación
        // Calcular prioridad inicial (tiempo de espera no excedido al llegar)
        int prioridad = calcular_prioridad_numerica(reserva_flag, 0, tipo);

        // --- CAMBIO: Asignar datos al slot correspondiente (1 a 15) ---
        if (slot_asignado == 1) {
            g_slot_cliente1_ocupado = 1; strcpy(g_slot_cliente1_nombre, nombre); g_slot_cliente1_hora_llegada_minutos = hora_llegada; g_slot_cliente1_tiene_reserva = reserva_flag; strcpy(g_slot_cliente1_tipo_cliente, tipo); g_slot_cliente1_numero_personas = personas; g_slot_cliente1_id_simulado = cliente_id; g_slot_cliente1_prioridad_calculada = prioridad;
        } else if (slot_asignado == 2) {
            g_slot_cliente2_ocupado = 1; strcpy(g_slot_cliente2_nombre, nombre); g_slot_cliente2_hora_llegada_minutos = hora_llegada; g_slot_cliente2_tiene_reserva = reserva_flag; strcpy(g_slot_cliente2_tipo_cliente, tipo); g_slot_cliente2_numero_personas = personas; g_slot_cliente2_id_simulado = cliente_id; g_slot_cliente2_prioridad_calculada = prioridad;
        } else if (slot_asignado == 3) {
            g_slot_cliente3_ocupado = 1; strcpy(g_slot_cliente3_nombre, nombre); g_slot_cliente3_hora_llegada_minutos = hora_llegada; g_slot_cliente3_tiene_reserva = reserva_flag; strcpy(g_slot_cliente3_tipo_cliente, tipo); g_slot_cliente3_numero_personas = personas; g_slot_cliente3_id_simulado = cliente_id; g_slot_cliente3_prioridad_calculada = prioridad;
        } else if (slot_asignado == 4) {
            g_slot_cliente4_ocupado = 1; strcpy(g_slot_cliente4_nombre, nombre); g_slot_cliente4_hora_llegada_minutos = hora_llegada; g_slot_cliente4_tiene_reserva = reserva_flag; strcpy(g_slot_cliente4_tipo_cliente, tipo); g_slot_cliente4_numero_personas = personas; g_slot_cliente4_id_simulado = cliente_id; g_slot_cliente4_prioridad_calculada = prioridad;
        } else if (slot_asignado == 5) {
            g_slot_cliente5_ocupado = 1; strcpy(g_slot_cliente5_nombre, nombre); g_slot_cliente5_hora_llegada_minutos = hora_llegada; g_slot_cliente5_tiene_reserva = reserva_flag; strcpy(g_slot_cliente5_tipo_cliente, tipo); g_slot_cliente5_numero_personas = personas; g_slot_cliente5_id_simulado = cliente_id; g_slot_cliente5_prioridad_calculada = prioridad;
        } else if (slot_asignado == 6) {
            g_slot_cliente6_ocupado = 1; strcpy(g_slot_cliente6_nombre, nombre); g_slot_cliente6_hora_llegada_minutos = hora_llegada; g_slot_cliente6_tiene_reserva = reserva_flag; strcpy(g_slot_cliente6_tipo_cliente, tipo); g_slot_cliente6_numero_personas = personas; g_slot_cliente6_id_simulado = cliente_id; g_slot_cliente6_prioridad_calculada = prioridad;
        } else if (slot_asignado == 7) {
            g_slot_cliente7_ocupado = 1; strcpy(g_slot_cliente7_nombre, nombre); g_slot_cliente7_hora_llegada_minutos = hora_llegada; g_slot_cliente7_tiene_reserva = reserva_flag; strcpy(g_slot_cliente7_tipo_cliente, tipo); g_slot_cliente7_numero_personas = personas; g_slot_cliente7_id_simulado = cliente_id; g_slot_cliente7_prioridad_calculada = prioridad;
        } else if (slot_asignado == 8) {
            g_slot_cliente8_ocupado = 1; strcpy(g_slot_cliente8_nombre, nombre); g_slot_cliente8_hora_llegada_minutos = hora_llegada; g_slot_cliente8_tiene_reserva = reserva_flag; strcpy(g_slot_cliente8_tipo_cliente, tipo); g_slot_cliente8_numero_personas = personas; g_slot_cliente8_id_simulado = cliente_id; g_slot_cliente8_prioridad_calculada = prioridad;
        } else if (slot_asignado == 9) {
            g_slot_cliente9_ocupado = 1; strcpy(g_slot_cliente9_nombre, nombre); g_slot_cliente9_hora_llegada_minutos = hora_llegada; g_slot_cliente9_tiene_reserva = reserva_flag; strcpy(g_slot_cliente9_tipo_cliente, tipo); g_slot_cliente9_numero_personas = personas; g_slot_cliente9_id_simulado = cliente_id; g_slot_cliente9_prioridad_calculada = prioridad;
        } else if (slot_asignado == 10) {
            g_slot_cliente10_ocupado = 1; strcpy(g_slot_cliente10_nombre, nombre); g_slot_cliente10_hora_llegada_minutos = hora_llegada; g_slot_cliente10_tiene_reserva = reserva_flag; strcpy(g_slot_cliente10_tipo_cliente, tipo); g_slot_cliente10_numero_personas = personas; g_slot_cliente10_id_simulado = cliente_id; g_slot_cliente10_prioridad_calculada = prioridad;
        } else if (slot_asignado == 11) {
            g_slot_cliente11_ocupado = 1; strcpy(g_slot_cliente11_nombre, nombre); g_slot_cliente11_hora_llegada_minutos = hora_llegada; g_slot_cliente11_tiene_reserva = reserva_flag; strcpy(g_slot_cliente11_tipo_cliente, tipo); g_slot_cliente11_numero_personas = personas; g_slot_cliente11_id_simulado = cliente_id; g_slot_cliente11_prioridad_calculada = prioridad;
        } else if (slot_asignado == 12) {
            g_slot_cliente12_ocupado = 1; strcpy(g_slot_cliente12_nombre, nombre); g_slot_cliente12_hora_llegada_minutos = hora_llegada; g_slot_cliente12_tiene_reserva = reserva_flag; strcpy(g_slot_cliente12_tipo_cliente, tipo); g_slot_cliente12_numero_personas = personas; g_slot_cliente12_id_simulado = cliente_id; g_slot_cliente12_prioridad_calculada = prioridad;
        } else if (slot_asignado == 13) {
            g_slot_cliente13_ocupado = 1; strcpy(g_slot_cliente13_nombre, nombre); g_slot_cliente13_hora_llegada_minutos = hora_llegada; g_slot_cliente13_tiene_reserva = reserva_flag; strcpy(g_slot_cliente13_tipo_cliente, tipo); g_slot_cliente13_numero_personas = personas; g_slot_cliente13_id_simulado = cliente_id; g_slot_cliente13_prioridad_calculada = prioridad;
        } else if (slot_asignado == 14) {
            g_slot_cliente14_ocupado = 1; strcpy(g_slot_cliente14_nombre, nombre); g_slot_cliente14_hora_llegada_minutos = hora_llegada; g_slot_cliente14_tiene_reserva = reserva_flag; strcpy(g_slot_cliente14_tipo_cliente, tipo); g_slot_cliente14_numero_personas = personas; g_slot_cliente14_id_simulado = cliente_id; g_slot_cliente14_prioridad_calculada = prioridad;
        } else if (slot_asignado == 15) {
            g_slot_cliente15_ocupado = 1; strcpy(g_slot_cliente15_nombre, nombre); g_slot_cliente15_hora_llegada_minutos = hora_llegada; g_slot_cliente15_tiene_reserva = reserva_flag; strcpy(g_slot_cliente15_tipo_cliente, tipo); g_slot_cliente15_numero_personas = personas; g_slot_cliente15_id_simulado = cliente_id; g_slot_cliente15_prioridad_calculada = prioridad;
        }


        g_clientes_en_cola_count++;
        printf("Grupo '%s' (ID: %d) añadido a la cola en slot %d. Prioridad inicial: %d.\n", nombre, cliente_id, slot_asignado, prioridad);
    } else {
        // Esto no debería ser alcanzado si g_clientes_en_cola_count < MAX_CLIENTES_EN_COLA_SIMULADA
        printf("Error interno: No se encontró slot libre aunque la cuenta indicaba espacio.\n");
    }
}


// Libera una mesa por su ID.
void liberar_mesa(int id_mesa) {
    int mesa_liberada_flag = 0; // Bandera para saber si una mesa válida fue liberada
    // --- CAMBIO: Liberar la mesa correspondiente (1 a 12) ---
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
        //avanzar_tiempo(MINUTOS_AVANCE_POR_DEFECTO, 0); // Avanzar tiempo se hace en main loop o por otras ops
        intentar_asignar_mesa(); // Intentar asignar después de liberar
    }
}

// Avanza el tiempo de simulación.
void avanzar_tiempo(int minutos_a_avanzar, int intentar_asignacion_despues) {
    if (minutos_a_avanzar > 0) {
        g_tiempo_simulacion_actual_minutos += minutos_a_avanzar;
        printf("Tiempo avanzado. Hora actual de simulación: %d mins.\n", g_tiempo_simulacion_actual_minutos);
    }
    if (intentar_asignacion_despues) {
        intentar_asignar_mesa();
    }
}


// Intenta asignar la mesa más adecuada al cliente con mayor prioridad en la cola.
void intentar_asignar_mesa() {
    if (g_clientes_en_cola_count == 0) {
        printf("No hay clientes en espera para asignar mesa.\n");
        return;
    }

    // --- CAMBIO: Actualizar prioridades para los 15 slots basándose en el tiempo actual ---
    if(g_slot_cliente1_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente1_hora_llegada_minutos; g_slot_cliente1_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente1_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente1_tipo_cliente); }
    else g_slot_cliente1_prioridad_calculada = 999999; // Marcar como muy baja prioridad si no está ocupado
    if(g_slot_cliente2_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente2_hora_llegada_minutos; g_slot_cliente2_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente2_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente2_tipo_cliente); }
    else g_slot_cliente2_prioridad_calculada = 999999;
    if(g_slot_cliente3_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente3_hora_llegada_minutos; g_slot_cliente3_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente3_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente3_tipo_cliente); }
    else g_slot_cliente3_prioridad_calculada = 999999;
    if(g_slot_cliente4_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente4_hora_llegada_minutos; g_slot_cliente4_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente4_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente4_tipo_cliente); }
    else g_slot_cliente4_prioridad_calculada = 999999;
    if(g_slot_cliente5_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente5_hora_llegada_minutos; g_slot_cliente5_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente5_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente5_tipo_cliente); }
    else g_slot_cliente5_prioridad_calculada = 999999;
    if(g_slot_cliente6_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente6_hora_llegada_minutos; g_slot_cliente6_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente6_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente6_tipo_cliente); }
    else g_slot_cliente6_prioridad_calculada = 999999;
    if(g_slot_cliente7_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente7_hora_llegada_minutos; g_slot_cliente7_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente7_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente7_tipo_cliente); }
    else g_slot_cliente7_prioridad_calculada = 999999;
    if(g_slot_cliente8_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente8_hora_llegada_minutos; g_slot_cliente8_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente8_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente8_tipo_cliente); }
    else g_slot_cliente8_prioridad_calculada = 999999;
    if(g_slot_cliente9_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente9_hora_llegada_minutos; g_slot_cliente9_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente9_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente9_tipo_cliente); }
    else g_slot_cliente9_prioridad_calculada = 999999;
    if(g_slot_cliente10_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente10_hora_llegada_minutos; g_slot_cliente10_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente10_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente10_tipo_cliente); }
    else g_slot_cliente10_prioridad_calculada = 999999;
    if(g_slot_cliente11_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente11_hora_llegada_minutos; g_slot_cliente11_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente11_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente11_tipo_cliente); }
    else g_slot_cliente11_prioridad_calculada = 999999;
    if(g_slot_cliente12_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente12_hora_llegada_minutos; g_slot_cliente12_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente12_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente12_tipo_cliente); }
    else g_slot_cliente12_prioridad_calculada = 999999;
    if(g_slot_cliente13_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente13_hora_llegada_minutos; g_slot_cliente13_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente13_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente13_tipo_cliente); }
    else g_slot_cliente13_prioridad_calculada = 999999;
    if(g_slot_cliente14_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente14_hora_llegada_minutos; g_slot_cliente14_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente14_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente14_tipo_cliente); }
    else g_slot_cliente14_prioridad_calculada = 999999;
    if(g_slot_cliente15_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente15_hora_llegada_minutos; g_slot_cliente15_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente15_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente15_tipo_cliente); }
    else g_slot_cliente15_prioridad_calculada = 999999;


    // --- CAMBIO: Encontrar el cliente con la mayor prioridad entre 15 slots, incluyendo desempate por hora de llegada ---
    // Esto simula encontrar el elemento con el menor valor de prioridad en la cola.
    int mejor_prioridad_valor = 999999; // Inicializar con un valor muy alto (menor prioridad)
    int slot_del_mejor_cliente = -1;
    int hora_llegada_del_mejor_cliente = 999999; // Para desempate FIFO (hora más temprana es mejor)

    // Iterar a través de los 15 slots para encontrar el cliente de mayor prioridad
    if (g_slot_cliente1_ocupado) {
        if (g_slot_cliente1_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente1_prioridad_calculada;
            slot_del_mejor_cliente = 1;
            hora_llegada_del_mejor_cliente = g_slot_cliente1_hora_llegada_minutos;
        } else if (g_slot_cliente1_prioridad_calculada == mejor_prioridad_valor) {
            // Tie-breaker: hora de llegada más temprana gana
            if (g_slot_cliente1_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 1;
                 hora_llegada_del_mejor_cliente = g_slot_cliente1_hora_llegada_minutos;
            }
        }
    }
    if (g_slot_cliente2_ocupado) {
        if (g_slot_cliente2_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente2_prioridad_calculada;
            slot_del_mejor_cliente = 2;
            hora_llegada_del_mejor_cliente = g_slot_cliente2_hora_llegada_minutos;
        } else if (g_slot_cliente2_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente2_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 2;
                 hora_llegada_del_mejor_cliente = g_slot_cliente2_hora_llegada_minutos;
            }
        }
    }
    if (g_slot_cliente3_ocupado) {
        if (g_slot_cliente3_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente3_prioridad_calculada;
            slot_del_mejor_cliente = 3;
            hora_llegada_del_mejor_cliente = g_slot_cliente3_hora_llegada_minutos;
        } else if (g_slot_cliente3_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente3_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 3;
                 hora_llegada_del_mejor_cliente = g_slot_cliente3_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente4_ocupado) {
        if (g_slot_cliente4_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente4_prioridad_calculada;
            slot_del_mejor_cliente = 4;
            hora_llegada_del_mejor_cliente = g_slot_cliente4_hora_llegada_minutos;
        } else if (g_slot_cliente4_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente4_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 4;
                 hora_llegada_del_mejor_cliente = g_slot_cliente4_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente5_ocupado) {
        if (g_slot_cliente5_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente5_prioridad_calculada;
            slot_del_mejor_cliente = 5;
            hora_llegada_del_mejor_cliente = g_slot_cliente5_hora_llegada_minutos;
        } else if (g_slot_cliente5_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente5_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 5;
                 hora_llegada_del_mejor_cliente = g_slot_cliente5_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente6_ocupado) {
        if (g_slot_cliente6_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente6_prioridad_calculada;
            slot_del_mejor_cliente = 6;
            hora_llegada_del_mejor_cliente = g_slot_cliente6_hora_llegada_minutos;
        } else if (g_slot_cliente6_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente6_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 6;
                 hora_llegada_del_mejor_cliente = g_slot_cliente6_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente7_ocupado) {
        if (g_slot_cliente7_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente7_prioridad_calculada;
            slot_del_mejor_cliente = 7;
            hora_llegada_del_mejor_cliente = g_slot_cliente7_hora_llegada_minutos;
        } else if (g_slot_cliente7_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente7_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 7;
                 hora_llegada_del_mejor_cliente = g_slot_cliente7_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente8_ocupado) {
        if (g_slot_cliente8_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente8_prioridad_calculada;
            slot_del_mejor_cliente = 8;
            hora_llegada_del_mejor_cliente = g_slot_cliente8_hora_llegada_minutos;
        } else if (g_slot_cliente8_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente8_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 8;
                 hora_llegada_del_mejor_cliente = g_slot_cliente8_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente9_ocupado) {
        if (g_slot_cliente9_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente9_prioridad_calculada;
            slot_del_mejor_cliente = 9;
            hora_llegada_del_mejor_cliente = g_slot_cliente9_hora_llegada_minutos;
        } else if (g_slot_cliente9_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente9_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 9;
                 hora_llegada_del_mejor_cliente = g_slot_cliente9_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente10_ocupado) {
        if (g_slot_cliente10_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente10_prioridad_calculada;
            slot_del_mejor_cliente = 10;
            hora_llegada_del_mejor_cliente = g_slot_cliente10_hora_llegada_minutos;
        } else if (g_slot_cliente10_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente10_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 10;
                 hora_llegada_del_mejor_cliente = g_slot_cliente10_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente11_ocupado) {
        if (g_slot_cliente11_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente11_prioridad_calculada;
            slot_del_mejor_cliente = 11;
            hora_llegada_del_mejor_cliente = g_slot_cliente11_hora_llegada_minutos;
        } else if (g_slot_cliente11_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente11_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 11;
                 hora_llegada_del_mejor_cliente = g_slot_cliente11_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente12_ocupado) {
        if (g_slot_cliente12_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente12_prioridad_calculada;
            slot_del_mejor_cliente = 12;
            hora_llegada_del_mejor_cliente = g_slot_cliente12_hora_llegada_minutos;
        } else if (g_slot_cliente12_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente12_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 12;
                 hora_llegada_del_mejor_cliente = g_slot_cliente12_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente13_ocupado) {
        if (g_slot_cliente13_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente13_prioridad_calculada;
            slot_del_mejor_cliente = 13;
            hora_llegada_del_mejor_cliente = g_slot_cliente13_hora_llegada_minutos;
        } else if (g_slot_cliente13_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente13_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 13;
                 hora_llegada_del_mejor_cliente = g_slot_cliente13_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente14_ocupado) {
        if (g_slot_cliente14_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente14_prioridad_calculada;
            slot_del_mejor_cliente = 14;
            hora_llegada_del_mejor_cliente = g_slot_cliente14_hora_llegada_minutos;
        } else if (g_slot_cliente14_prioridad_calculada == mejor_prioridad_valor) {
            if (g_slot_cliente14_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 14;
                 hora_llegada_del_mejor_cliente = g_slot_cliente14_hora_llegada_minutos;
            }
        }
    }
     if (g_slot_cliente15_ocupado) {
        if (g_slot_cliente15_prioridad_calculada < mejor_prioridad_valor) {
            mejor_prioridad_valor = g_slot_cliente15_prioridad_calculada;
            slot_del_mejor_cliente = 15;
            hora_llegada_del_mejor_cliente = g_slot_cliente15_hora_llegada_minutos;
        } else if (g_slot_cliente15_prioridad_calculada== mejor_prioridad_valor) {
            if (g_slot_cliente15_hora_llegada_minutos < hora_llegada_del_mejor_cliente) {
                 slot_del_mejor_cliente = 15;
                 hora_llegada_del_mejor_cliente = g_slot_cliente15_hora_llegada_minutos;
            }
        }
    }


    // Si no se encontró ningún cliente ocupado (no debería pasar si g_clientes_en_cola_count > 0)
    if (slot_del_mejor_cliente == -1) {
        printf("Error interno: No se pudo seleccionar un cliente prioritario de los slots ocupados.\n");
        return;
    }

    // Extraer datos del cliente seleccionado basándose en el número de slot identificado
    int cliente_id, personas, hora_llegada_cliente_sel;
    char nombre_cliente[MAX_NOMBRE_GRUPO];
    // --- CAMBIO: Extraer datos del slot correspondiente (1 a 15) ---
    if (slot_del_mejor_cliente == 1) { cliente_id = g_slot_cliente1_id_simulado; personas = g_slot_cliente1_numero_personas; strcpy(nombre_cliente, g_slot_cliente1_nombre); hora_llegada_cliente_sel = g_slot_cliente1_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 2) { cliente_id = g_slot_cliente2_id_simulado; personas = g_slot_cliente2_numero_personas; strcpy(nombre_cliente, g_slot_cliente2_nombre); hora_llegada_cliente_sel = g_slot_cliente2_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 3) { cliente_id = g_slot_cliente3_id_simulado; personas = g_slot_cliente3_numero_personas; strcpy(nombre_cliente, g_slot_cliente3_nombre); hora_llegada_cliente_sel = g_slot_cliente3_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 4) { cliente_id = g_slot_cliente4_id_simulado; personas = g_slot_cliente4_numero_personas; strcpy(nombre_cliente, g_slot_cliente4_nombre); hora_llegada_cliente_sel = g_slot_cliente4_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 5) { cliente_id = g_slot_cliente5_id_simulado; personas = g_slot_cliente5_numero_personas; strcpy(nombre_cliente, g_slot_cliente5_nombre); hora_llegada_cliente_sel = g_slot_cliente5_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 6) { cliente_id = g_slot_cliente6_id_simulado; personas = g_slot_cliente6_numero_personas; strcpy(nombre_cliente, g_slot_cliente6_nombre); hora_llegada_cliente_sel = g_slot_cliente6_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 7) { cliente_id = g_slot_cliente7_id_simulado; personas = g_slot_cliente7_numero_personas; strcpy(nombre_cliente, g_slot_cliente7_nombre); hora_llegada_cliente_sel = g_slot_cliente7_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 8) { cliente_id = g_slot_cliente8_id_simulado; personas = g_slot_cliente8_numero_personas; strcpy(nombre_cliente, g_slot_cliente8_nombre); hora_llegada_cliente_sel = g_slot_cliente8_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 9) { cliente_id = g_slot_cliente9_id_simulado; personas = g_slot_cliente9_numero_personas; strcpy(nombre_cliente, g_slot_cliente9_nombre); hora_llegada_cliente_sel = g_slot_cliente9_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 10) { cliente_id = g_slot_cliente10_id_simulado; personas = g_slot_cliente10_numero_personas; strcpy(nombre_cliente, g_slot_cliente10_nombre); hora_llegada_cliente_sel = g_slot_cliente10_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 11) { cliente_id = g_slot_cliente11_id_simulado; personas = g_slot_cliente11_numero_personas; strcpy(nombre_cliente, g_slot_cliente11_nombre); hora_llegada_cliente_sel = g_slot_cliente11_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 12) { cliente_id = g_slot_cliente12_id_simulado; personas = g_slot_cliente12_numero_personas; strcpy(nombre_cliente, g_slot_cliente12_nombre); hora_llegada_cliente_sel = g_slot_cliente12_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 13) { cliente_id = g_slot_cliente13_id_simulado; personas = g_slot_cliente13_numero_personas; strcpy(nombre_cliente, g_slot_cliente13_nombre); hora_llegada_cliente_sel = g_slot_cliente13_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 14) { cliente_id = g_slot_cliente14_id_simulado; personas = g_slot_cliente14_numero_personas; strcpy(nombre_cliente, g_slot_cliente14_nombre); hora_llegada_cliente_sel = g_slot_cliente14_hora_llegada_minutos; }
    else if (slot_del_mejor_cliente == 15) { cliente_id = g_slot_cliente15_id_simulado; personas = g_slot_cliente15_numero_personas; strcpy(nombre_cliente, g_slot_cliente15_nombre); hora_llegada_cliente_sel = g_slot_cliente15_hora_llegada_minutos; }


    int tiempo_esperando_actual_mins = g_tiempo_simulacion_actual_minutos - hora_llegada_cliente_sel;
    int espera_excedida_actual = (tiempo_esperando_actual_mins >= TIEMPO_ESPERA_MAX_MINS);

    // --- Buscar Mesa (Lógica sin cambios, itera sobre las 12 mesas) ---
    int mesa_asignada_id = -1;
    // Prioridad 1: Capacidad exacta
    if (g_mesa1_disponible && g_mesa1_capacidad == personas) mesa_asignada_id = 1;
    else if (g_mesa2_disponible && g_mesa2_capacidad == personas) mesa_asignada_id = 2;
    else if (g_mesa3_disponible && g_mesa3_capacidad == personas) mesa_asignada_id = 3;
    else if (g_mesa4_disponible && g_mesa4_capacidad == personas) mesa_asignada_id = 4;
    else if (g_mesa5_disponible && g_mesa5_capacidad == personas) mesa_asignada_id = 5;
    else if (g_mesa6_disponible && g_mesa6_capacidad == personas) mesa_asignada_id = 6;
    else if (g_mesa7_disponible && g_mesa7_capacidad == personas) mesa_asignada_id = 7;
    else if (g_mesa8_disponible && g_mesa8_capacidad == personas) mesa_asignada_id = 8;
    else if (g_mesa9_disponible && g_mesa9_capacidad == personas) mesa_asignada_id = 9;
    else if (g_mesa10_disponible && g_mesa10_capacidad == personas) mesa_asignada_id = 10;
    else if (g_mesa11_disponible && g_mesa11_capacidad == personas) mesa_asignada_id = 11;
    else if (g_mesa12_disponible && g_mesa12_capacidad == personas) mesa_asignada_id = 12;

    if (mesa_asignada_id == -1) { // Si no se encontró capacidad exacta, buscar mayor o igual capacidad
        // Lógica simplificada para mesas más grandes (se puede refinar como antes)
        int best_fit_mesa_id = -1;
        int min_cap_diff = 999; // Para encontrar la mesa más ajustada si no es exacta

        // Mesas de 6
        if (personas <= 6) {
             if (g_mesa1_disponible && g_mesa1_capacidad >= personas && (espera_excedida_actual || (g_mesa1_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa1_capacidad < min_cap_diff) {best_fit_mesa_id = 1; min_cap_diff = g_mesa1_capacidad;} }
             if (g_mesa2_disponible && g_mesa2_capacidad >= personas && (espera_excedida_actual || (g_mesa2_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa2_capacidad < min_cap_diff) {best_fit_mesa_id = 2; min_cap_diff = g_mesa2_capacidad;} }
             if (g_mesa3_disponible && g_mesa3_capacidad >= personas && (espera_excedida_actual || (g_mesa3_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa3_capacidad < min_cap_diff) {best_fit_mesa_id = 3; min_cap_diff = g_mesa3_capacidad;} }
             if (g_mesa4_disponible && g_mesa4_capacidad >= personas && (espera_excedida_actual || (g_mesa4_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa4_capacidad < min_cap_diff) {best_fit_mesa_id = 4; min_cap_diff = g_mesa4_capacidad;} }
         }
          // Mesas de 4
         if (personas <= 4 ) {
             if (g_mesa5_disponible && g_mesa5_capacidad >= personas && (espera_excedida_actual || (g_mesa5_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa5_capacidad < min_cap_diff) {best_fit_mesa_id = 5; min_cap_diff = g_mesa5_capacidad;} }
             if (g_mesa6_disponible && g_mesa6_capacidad >= personas && (espera_excedida_actual || (g_mesa6_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa6_capacidad < min_cap_diff) {best_fit_mesa_id = 6; min_cap_diff = g_mesa6_capacidad;} }
             if (g_mesa7_disponible && g_mesa7_capacidad >= personas && (espera_excedida_actual || (g_mesa7_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa7_capacidad < min_cap_diff) {best_fit_mesa_id = 7; min_cap_diff = g_mesa7_capacidad;} }
             if (g_mesa8_disponible && g_mesa8_capacidad >= personas && (espera_excedida_actual || (g_mesa8_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa8_capacidad < min_cap_diff) {best_fit_mesa_id = 8; min_cap_diff = g_mesa8_capacidad;} }
             if (g_mesa9_disponible && g_mesa9_capacidad >= personas && (espera_excedida_actual || (g_mesa9_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa9_capacidad < min_cap_diff) {best_fit_mesa_id = 9; min_cap_diff = g_mesa9_capacidad;} }
             if (g_mesa10_disponible && g_mesa10_capacidad >= personas && (espera_excedida_actual || (g_mesa10_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa10_capacidad < min_cap_diff) {best_fit_mesa_id = 10; min_cap_diff = g_mesa10_capacidad;} }
         }
         // Mesas de 2
        if (personas <= 2) {
             if (g_mesa11_disponible && g_mesa11_capacidad >= personas && (espera_excedida_actual || (g_mesa11_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa11_capacidad < min_cap_diff) {best_fit_mesa_id = 11; min_cap_diff = g_mesa11_capacidad;} }
             if (g_mesa12_disponible && g_mesa12_capacidad >= personas && (espera_excedida_actual || (g_mesa12_capacidad - personas <= 2))) { if(best_fit_mesa_id == -1 || g_mesa12_capacidad < min_cap_diff) {best_fit_mesa_id = 12; min_cap_diff = g_mesa12_capacidad;} }
         }
        mesa_asignada_id = best_fit_mesa_id;
    }


    if (mesa_asignada_id != -1) {
        printf("Cliente ID %d ('%s') asignado a Mesa %d. Esperó %d mins.\n",
               cliente_id, nombre_cliente, mesa_asignada_id, tiempo_esperando_actual_mins);

        // --- CAMBIO: Marcar la mesa asignada como ocupada (1 a 12) ---
        if(mesa_asignada_id == 1) {g_mesa1_disponible = 0; g_mesa1_cliente_asignado_id = cliente_id; g_mesa1_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 2) {g_mesa2_disponible = 0; g_mesa2_cliente_asignado_id = cliente_id; g_mesa2_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 3) {g_mesa3_disponible = 0; g_mesa3_cliente_asignado_id = cliente_id; g_mesa3_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 4) {g_mesa4_disponible = 0; g_mesa4_cliente_asignado_id = cliente_id; g_mesa4_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 5) {g_mesa5_disponible = 0; g_mesa5_cliente_asignado_id = cliente_id; g_mesa5_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 6) {g_mesa6_disponible = 0; g_mesa6_cliente_asignado_id = cliente_id; g_mesa6_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 7) {g_mesa7_disponible = 0; g_mesa7_cliente_asignado_id = cliente_id; g_mesa7_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 8) {g_mesa8_disponible = 0; g_mesa8_cliente_asignado_id = cliente_id; g_mesa8_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 9) {g_mesa9_disponible = 0; g_mesa9_cliente_asignado_id = cliente_id; g_mesa9_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 10) {g_mesa10_disponible = 0; g_mesa10_cliente_asignado_id = cliente_id; g_mesa10_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 11) {g_mesa11_disponible = 0; g_mesa11_cliente_asignado_id = cliente_id; g_mesa11_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}
        else if(mesa_asignada_id == 12) {g_mesa12_disponible = 0; g_mesa12_cliente_asignado_id = cliente_id; g_mesa12_hora_asignacion_minutos = g_tiempo_simulacion_actual_minutos;}


        // --- CAMBIO: Liberar el slot del cliente (1 a 15) ---
        if (slot_del_mejor_cliente == 1) g_slot_cliente1_ocupado = 0;
        else if (slot_del_mejor_cliente == 2) g_slot_cliente2_ocupado = 0;
        else if (slot_del_mejor_cliente == 3) g_slot_cliente3_ocupado = 0;
        else if (slot_del_mejor_cliente == 4) g_slot_cliente4_ocupado = 0;
        else if (slot_del_mejor_cliente == 5) g_slot_cliente5_ocupado = 0;
        else if (slot_del_mejor_cliente == 6) g_slot_cliente6_ocupado = 0;
        else if (slot_del_mejor_cliente == 7) g_slot_cliente7_ocupado = 0;
        else if (slot_del_mejor_cliente == 8) g_slot_cliente8_ocupado = 0;
        else if (slot_del_mejor_cliente == 9) g_slot_cliente9_ocupado = 0;
        else if (slot_del_mejor_cliente == 10) g_slot_cliente10_ocupado = 0;
        else if (slot_del_mejor_cliente == 11) g_slot_cliente11_ocupado = 0;
        else if (slot_del_mejor_cliente == 12) g_slot_cliente12_ocupado = 0;
        else if (slot_del_mejor_cliente == 13) g_slot_cliente13_ocupado = 0;
        else if (slot_del_mejor_cliente == 14) g_slot_cliente14_ocupado = 0;
        else if (slot_del_mejor_cliente == 15) g_slot_cliente15_ocupado = 0;

        g_clientes_en_cola_count--; // Decrementar el contador de clientes en espera
    } else {
        // Mensaje si no se encontró una mesa adecuada
        printf("No hay mesas adecuadas disponibles para Cliente ID %d ('%s'). Esperó %d mins%s.\n",
               cliente_id, nombre_cliente, tiempo_esperando_actual_mins,
               espera_excedida_actual ? " (se consideraron mesas más grandes)" : "");
    }
}

// Revisa e imprime el estado actual de los slots de la cola de espera.
void revisar_cola_espera() {
    printf("\n--- Cola de Espera (Simulada, Prioridad Actual) ---\n");
    if (g_clientes_en_cola_count == 0) {
        printf("La cola de espera está vacía.\n");
        return;
    }

    // --- CAMBIO: Actualizar prioridades para los 15 slots para mostrar ---
    if(g_slot_cliente1_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente1_hora_llegada_minutos; g_slot_cliente1_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente1_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente1_tipo_cliente); }
    else g_slot_cliente1_prioridad_calculada = 999999;
    if(g_slot_cliente2_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente2_hora_llegada_minutos; g_slot_cliente2_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente2_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente2_tipo_cliente); }
    else g_slot_cliente2_prioridad_calculada = 999999;
    if(g_slot_cliente3_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente3_hora_llegada_minutos; g_slot_cliente3_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente3_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente3_tipo_cliente); }
    else g_slot_cliente3_prioridad_calculada = 999999;
    if(g_slot_cliente4_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente4_hora_llegada_minutos; g_slot_cliente4_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente4_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente4_tipo_cliente); }
    else g_slot_cliente4_prioridad_calculada = 999999;
    if(g_slot_cliente5_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente5_hora_llegada_minutos; g_slot_cliente5_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente5_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente5_tipo_cliente); }
    else g_slot_cliente5_prioridad_calculada = 999999;
    if(g_slot_cliente6_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente6_hora_llegada_minutos; g_slot_cliente6_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente6_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente6_tipo_cliente); }
    else g_slot_cliente6_prioridad_calculada = 999999;
    if(g_slot_cliente7_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente7_hora_llegada_minutos; g_slot_cliente7_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente7_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente7_tipo_cliente); }
    else g_slot_cliente7_prioridad_calculada = 999999;
    if(g_slot_cliente8_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente8_hora_llegada_minutos; g_slot_cliente8_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente8_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente8_tipo_cliente); }
    else g_slot_cliente8_prioridad_calculada = 999999;
    if(g_slot_cliente9_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente9_hora_llegada_minutos; g_slot_cliente9_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente9_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente9_tipo_cliente); }
    else g_slot_cliente9_prioridad_calculada = 999999;
    if(g_slot_cliente10_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente10_hora_llegada_minutos; g_slot_cliente10_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente10_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente10_tipo_cliente); }
    else g_slot_cliente10_prioridad_calculada = 999999;
    if(g_slot_cliente11_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente11_hora_llegada_minutos; g_slot_cliente11_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente11_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente11_tipo_cliente); }
    else g_slot_cliente11_prioridad_calculada = 999999;
    if(g_slot_cliente12_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente12_hora_llegada_minutos; g_slot_cliente12_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente12_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente12_tipo_cliente); }
    else g_slot_cliente12_prioridad_calculada = 999999;
    if(g_slot_cliente13_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente13_hora_llegada_minutos; g_slot_cliente13_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente13_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente13_tipo_cliente); }
    else g_slot_cliente13_prioridad_calculada = 999999;
    if(g_slot_cliente14_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente14_hora_llegada_minutos; g_slot_cliente14_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente14_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente14_tipo_cliente); }
    else g_slot_cliente14_prioridad_calculada = 999999;
    if(g_slot_cliente15_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente15_hora_llegada_minutos; g_slot_cliente15_prioridad_calculada = calcular_prioridad_numerica(g_slot_cliente15_tiene_reserva, (e >= TIEMPO_ESPERA_MAX_MINS), g_slot_cliente15_tipo_cliente); }
    else g_slot_cliente15_prioridad_calculada = 999999;


    printf("Slot | ID | Nombre        | Personas | Reserva | Tipo      | Llegada(m) | Espera(m) | Prioridad\n");
    printf("---------------------------------------------------------------------------------------------\n");
    // --- CAMBIO: Imprimir los 15 slots si están ocupados ---
    if (g_slot_cliente1_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente1_hora_llegada_minutos; printf("1    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente1_id_simulado, g_slot_cliente1_nombre, g_slot_cliente1_numero_personas, g_slot_cliente1_tiene_reserva ? "Si":"No", g_slot_cliente1_tipo_cliente, g_slot_cliente1_hora_llegada_minutos, e, g_slot_cliente1_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente2_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente2_hora_llegada_minutos; printf("2    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente2_id_simulado, g_slot_cliente2_nombre, g_slot_cliente2_numero_personas, g_slot_cliente2_tiene_reserva ? "Si":"No", g_slot_cliente2_tipo_cliente, g_slot_cliente2_hora_llegada_minutos, e, g_slot_cliente2_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente3_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente3_hora_llegada_minutos; printf("3    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente3_id_simulado, g_slot_cliente3_nombre, g_slot_cliente3_numero_personas, g_slot_cliente3_tiene_reserva ? "Si":"No", g_slot_cliente3_tipo_cliente, g_slot_cliente3_hora_llegada_minutos, e, g_slot_cliente3_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente4_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente4_hora_llegada_minutos; printf("4    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente4_id_simulado, g_slot_cliente4_nombre, g_slot_cliente4_numero_personas, g_slot_cliente4_tiene_reserva ? "Si":"No", g_slot_cliente4_tipo_cliente, g_slot_cliente4_hora_llegada_minutos, e, g_slot_cliente4_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente5_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente5_hora_llegada_minutos; printf("5    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente5_id_simulado, g_slot_cliente5_nombre, g_slot_cliente5_numero_personas, g_slot_cliente5_tiene_reserva ? "Si":"No", g_slot_cliente5_tipo_cliente, g_slot_cliente5_hora_llegada_minutos, e, g_slot_cliente5_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente6_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente6_hora_llegada_minutos; printf("6    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente6_id_simulado, g_slot_cliente6_nombre, g_slot_cliente6_numero_personas, g_slot_cliente6_tiene_reserva ? "Si":"No", g_slot_cliente6_tipo_cliente, g_slot_cliente6_hora_llegada_minutos, e, g_slot_cliente6_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente7_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente7_hora_llegada_minutos; printf("7    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente7_id_simulado, g_slot_cliente7_nombre, g_slot_cliente7_numero_personas, g_slot_cliente7_tiene_reserva ? "Si":"No", g_slot_cliente7_tipo_cliente, g_slot_cliente7_hora_llegada_minutos, e, g_slot_cliente7_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente8_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente8_hora_llegada_minutos; printf("8    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente8_id_simulado, g_slot_cliente8_nombre, g_slot_cliente8_numero_personas, g_slot_cliente8_tiene_reserva ? "Si":"No", g_slot_cliente8_tipo_cliente, g_slot_cliente8_hora_llegada_minutos, e, g_slot_cliente8_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente9_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente9_hora_llegada_minutos; printf("9    | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente9_id_simulado, g_slot_cliente9_nombre, g_slot_cliente9_numero_personas, g_slot_cliente9_tiene_reserva ? "Si":"No", g_slot_cliente9_tipo_cliente, g_slot_cliente9_hora_llegada_minutos, e, g_slot_cliente9_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente10_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente10_hora_llegada_minutos; printf("10   | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente10_id_simulado, g_slot_cliente10_nombre, g_slot_cliente10_numero_personas, g_slot_cliente10_tiene_reserva ? "Si":"No", g_slot_cliente10_tipo_cliente, g_slot_cliente10_hora_llegada_minutos, e, g_slot_cliente10_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente11_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente11_hora_llegada_minutos; printf("11   | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente11_id_simulado, g_slot_cliente11_nombre, g_slot_cliente11_numero_personas, g_slot_cliente11_tiene_reserva ? "Si":"No", g_slot_cliente11_tipo_cliente, g_slot_cliente11_hora_llegada_minutos, e, g_slot_cliente11_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente12_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente12_hora_llegada_minutos; printf("12   | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente12_id_simulado, g_slot_cliente12_nombre, g_slot_cliente12_numero_personas, g_slot_cliente12_tiene_reserva ? "Si":"No", g_slot_cliente12_tipo_cliente, g_slot_cliente12_hora_llegada_minutos, e, g_slot_cliente12_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente13_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente13_hora_llegada_minutos; printf("13   | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente13_id_simulado, g_slot_cliente13_nombre, g_slot_cliente13_numero_personas, g_slot_cliente13_tiene_reserva ? "Si":"No", g_slot_cliente13_tipo_cliente, g_slot_cliente13_hora_llegada_minutos, e, g_slot_cliente13_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente14_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente14_hora_llegada_minutos; printf("14   | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente14_id_simulado, g_slot_cliente14_nombre, g_slot_cliente14_numero_personas, g_slot_cliente14_tiene_reserva ? "Si":"No", g_slot_cliente14_tipo_cliente, g_slot_cliente14_hora_llegada_minutos, e, g_slot_cliente14_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}
    if (g_slot_cliente15_ocupado) { int e = g_tiempo_simulacion_actual_minutos - g_slot_cliente15_hora_llegada_minutos; printf("15   | %2d | %-13s | %8d | %7s | %-9s | %10d | %9d | %9d %s\n", g_slot_cliente15_id_simulado, g_slot_cliente15_nombre, g_slot_cliente15_numero_personas, g_slot_cliente15_tiene_reserva ? "Si":"No", g_slot_cliente15_tipo_cliente, g_slot_cliente15_hora_llegada_minutos, e, g_slot_cliente15_prioridad_calculada, (e>=TIEMPO_ESPERA_MAX_MINS)?"(EXC)":"");}


    printf("---------------------------------------------------------------------------------------------\n");
}

// Revisa e imprime el estado actual de las mesas.
void revisar_estado_mesas() {
    printf("\n--- Estado de las Mesas ---\n");
    printf("ID | Capacidad | Disponible | Cliente ID Asignado | Hora Asignación (mins)\n");
    printf("----------------------------------------------------------------------\n");
    // --- CAMBIO: Imprimir el estado de las 12 mesas ---
    printf("%2d | %9d | %10s | %19d | %20d\n", 1, g_mesa1_capacidad, g_mesa1_disponible?"Si":"No", g_mesa1_cliente_asignado_id, g_mesa1_hora_asignacion_minutos);
    printf("%2d | %9d | %10s | %19d | %20d\n", 2, g_mesa2_capacidad, g_mesa2_disponible?"Si":"No", g_mesa2_cliente_asignado_id, g_mesa2_hora_asignacion_minutos);
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

// Muestra las reglas de prioridad.
void mostrar_reglas_prioridad() {
    printf("\n--- Reglas de Prioridad (Menor valor numérico = Mayor Prioridad) ---\n");
    printf("1. Con Reserva (peso: 0) > Sin Reserva (peso: 200)\n");
    printf("2. Espera >= 15 mins (peso: 0) > Espera < 15 mins (peso: 100)\n");
    printf("3. Tipo Cliente: VIP (peso: 10) > REGULAR (peso: 20) > OCASIONAL (peso: 30)\n"); // VIP con menor peso
    printf("La prioridad total es la suma de estos pesos. La hora de llegada (FIFO) desempata si las prioridades calculadas son idénticas.\n");
    printf("Si un grupo espera >= 15 minutos, se considerarán mesas más grandes.\n");
    printf("----------------------------------------------------------------------\n");
}


// --- Bucle Principal de la CLI ---
int main() {
    char linea_comando[256];
    char comando[50];
    char arg1_str[MAX_NOMBRE_GRUPO]; // Para nombre de grupo o ID de mesa como string
    char arg2_str[MAX_TIPO_CLIENTE]; // Para tipo de cliente o flag de reserva como string
    int arg3_num; // Para número de personas
    int arg4_num; // Para minutos a avanzar

    inicializar_sistema();

    printf("\n--- Robot Mesero Primitivo CLI (15 Slots) ---\n");
    printf("Comandos: register, free, advance, queue, tables, rules, assign, exit\n");
    printf("Para 'register': register <NombreGrupo> <TieneReserva(0/1)> <TipoCliente(VIP/REGULAR/OCASIONAL)> <NumPersonas>\n");
    printf("  (Nota: Nombres y tipos SIN ESPACIOS para esta versión primitiva)\n");
    printf("Para 'free': free <ID_Mesa>\n");
    printf("Para 'advance': advance [minutos] (por defecto 5 si no se especifica)\n");
    printf("Para 'assign': assign (intenta asignar mesa al prioritario)\n");
    printf("Para 'queue': queue (revisa la cola de espera)\n");
    printf("Para 'tables': tables (revisa estado de mesas)\n");
    printf("Para 'rules': rules (muestra reglas de prioridad)\n");


    while (1) {
        printf("\nPrimitivoCLI > ");
        // Leer la línea completa
        if (fgets(linea_comando, sizeof(linea_comando), stdin) == NULL) {
            break; // Manejar EOF (Ctrl+D)
        }
        // Quitar carácter de nueva línea
        linea_comando[strcspn(linea_comando, "\n")] = 0;

        // Parseo muy básico del comando principal y los primeros 3 argumentos
        int num_args_leidos = sscanf(linea_comando, "%s %s %s %d %d", comando, arg1_str, arg2_str, &arg3_num, &arg4_num);

        if (strcmp(comando, "exit") == 0) {
            printf("Saliendo...\n");
            break;
        } else if (strcmp(comando, "register") == 0) {
            // Re-parsear específicamente para 'register', esperando 4 argumentos después del comando
            char nombre_temp[MAX_NOMBRE_GRUPO];
            int reserva_temp;
            char tipo_temp[MAX_TIPO_CLIENTE];
            int personas_temp;
            // Usar sscanf con %*s para saltar el primer string (el comando "register")
            if (sscanf(linea_comando, "%*s %s %d %s %d", nombre_temp, &reserva_temp, tipo_temp, &personas_temp) == 4) {
                 // Validar argumentos numéricos y de cadena
                if ((reserva_temp == 0 || reserva_temp == 1) &&
                    (strcmp(tipo_temp, "VIP") == 0 || strcmp(tipo_temp, "REGULAR") == 0 || strcmp(tipo_temp, "OCASIONAL") == 0) &&
                    (personas_temp >= 1 && personas_temp <= 6)) { // Validar rango de personas aquí también
                    registrar_llegada_cliente(nombre_temp, reserva_temp, tipo_temp, personas_temp);
                } else {
                    printf("Argumentos invalidos para 'register'. Verifique TieneReserva (0/1), TipoCliente (VIP/REGULAR/OCASIONAL) y NumPersonas (1-6).\n");
                }
            } else {
                printf("Uso: register <NombreGrupo> <TieneReserva(0/1)> <TipoCliente> <NumPersonas>\n");
            }
        } else if (strcmp(comando, "free") == 0) {
            int id_mesa_temp;
            // Espera: free <ID_Mesa>
            if (sscanf(linea_comando, "%*s %d", &id_mesa_temp) == 1) { // %*s salta "free"
                 // Validación básica del rango de ID de mesa
                if (id_mesa_temp >= 1 && id_mesa_temp <= NUM_MESAS) {
                     liberar_mesa(id_mesa_temp);
                } else {
                     printf("ID de mesa invalido. Debe ser entre 1 y %d.\n", NUM_MESAS);
                }
            } else {
                printf("Uso: free <ID_Mesa>\n");
            }
        } else if (strcmp(comando, "advance") == 0) {
            int minutos_adv = MINUTOS_AVANCE_POR_DEFECTO; // Valor por defecto
            // Espera: advance [minutos]
            int minutos_leidos;
            if (sscanf(linea_comando, "%*s %d", &minutos_leidos) == 1) { // %*s salta "advance"
                 // Si se leyó un número después del comando
                 minutos_adv = minutos_leidos;
            } else {
                 // Si sscanf no leyó un número, verificar si había *algún* argumento
                 char argumento_extra[50];
                 if (sscanf(linea_comando, "%*s %s", argumento_extra) == 1) {
                      // Había un argumento, pero no era un número
                      printf("Uso: advance [minutos] (los minutos deben ser un numero)\n");
                      continue; // Salir del manejo del comando actual
                 }
                 // Si no se leyó ni número ni otro argumento, usar el valor por defecto
            }

            if(minutos_adv < 0) {
                printf("Los minutos a avanzar deben ser no negativos.\n");
                continue;
            }
            avanzar_tiempo(minutos_adv, 1); // Siempre intentar asignar después de avanzar tiempo manualmente
        } else if (strcmp(comando, "queue") == 0) {
            // Espera: queue (sin argumentos)
             char argumento_extra[50];
             if (sscanf(linea_comando, "%*s %s", argumento_extra) == 1) {
                  printf("Uso: queue (sin argumentos)\n");
             } else {
                 revisar_cola_espera();
             }
        } else if (strcmp(comando, "tables") == 0) {
            // Espera: tables (sin argumentos)
             char argumento_extra[50];
             if (sscanf(linea_comando, "%*s %s", argumento_extra) == 1) {
                  printf("Uso: tables (sin argumentos)\n");
             } else {
                 revisar_estado_mesas();
             }
        } else if (strcmp(comando, "rules") == 0) {
            // Espera: rules (sin argumentos)
             char argumento_extra[50];
             if (sscanf(linea_comando, "%*s %s", argumento_extra) == 1) {
                  printf("Uso: rules (sin argumentos)\n");
             } else {
                 mostrar_reglas_prioridad();
             }
        } else if (strcmp(comando, "assign") == 0) {
            // Espera: assign (sin argumentos)
             char argumento_extra[50];
             if (sscanf(linea_comando, "%*s %s", argumento_extra) == 1) {
                  printf("Uso: assign (sin argumentos)\n");
             } else {
                 intentar_asignar_mesa(); // Intentar asignar manualmente
             }
        }
        else {
            printf("Comando desconocido: '%s'. Comandos validos: register, free, advance, queue, tables, rules, assign, exit\n", comando);
        }
    }

    return 0; // Salir exitosamente
}