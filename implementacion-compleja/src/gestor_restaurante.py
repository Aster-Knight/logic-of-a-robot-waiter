# src/gestor_restaurante.py

import datetime
from typing import List, Tuple, Dict, Any

from .cliente import Cliente
from .mesa import Mesa
from .solicitud_asignacion import SolicitudAsignacion
from .cola_prioridad_propia import ColaPrioridadPropia # Asegúrate de que esta es tu clase manual

# Definir pesos para tipos de cliente (menor valor = mayor prioridad)
TIPO_CLIENTE_PESOS: Dict[str, int] = {
    "VIP": 1,
    "REGULAR": 2,
    "OCASIONAL": 3,
    "DEFAULT": 99
}

class GestorRestaurante:
    """
    Encapsula la lógica de negocio para la gestión de mesas y clientes.
    Componente central del Modelo en MVC.
    """
    def __init__(self):
        self.mesas: List[Mesa] = []
        self.cola_espera: ColaPrioridadPropia = ColaPrioridadPropia()
        #self.tiempo_simulacion_actual: datetime.datetime = datetime.datetime.now() # Hora de inicio real o simulada
        self.tiempo_simulacion_actual: datetime.datetime = datetime.datetime(2024, 1, 1, 18, 0, 0) # Hora de inicio simulada (ej. 18:00)

        self.proximo_id_cliente: int = 1 # Contador para IDs de cliente (usado internamente)

        self.limite_grupos_cola: int = 15
        self.limite_tiempo_espera_mins: int = 15
        self.minutos_por_operacion: int = 5 # Tiempo que avanza con ciertas operaciones

        self._inicializar_mesas()
        # Cliente._next_id = 1 # No resetear aquí si queremos IDs persistentes a través de reset/base case?
        # Mejor que el gestor gestione los IDs.

    def _get_next_client_id(self) -> int:
        """Obtiene el próximo ID de cliente único."""
        next_id = self.proximo_id_cliente
        self.proximo_id_cliente += 1
        return next_id

    def _inicializar_mesas(self):
        """Crea las 12 instancias de Mesa con sus capacidades."""
        self.mesas = []
        for i in range(1, 5): self.mesas.append(Mesa(id_mesa=i, capacidad=6))
        for i in range(5, 11): self.mesas.append(Mesa(id_mesa=i, capacidad=4))
        for i in range(11, 13): self.mesas.append(Mesa(id_mesa=i, capacidad=2))

    def _calcular_prioridad_tupla(self, cliente: Cliente, hora_actual: datetime.datetime) -> Tuple[int, int, int, datetime.datetime]:
        """
        Calcula la tupla de prioridad para un cliente en un momento dado.
        Menores valores indican mayor prioridad.
        Formato: (reserva, espera_excedida, tipo_cliente_val, hora_llegada)
        """
        tiempo_esperando_delta = hora_actual - cliente.hora_llegada
        tiempo_esperando_mins = tiempo_esperando_delta.total_seconds() / 60
        espera_excedida = tiempo_esperando_mins >= self.limite_tiempo_espera_mins

        prioridad_reserva = 0 if cliente.tiene_reserva else 1
        prioridad_espera = 0 if espera_excedida else 1
        prioridad_tipo = TIPO_CLIENTE_PESOS.get(cliente.tipo_cliente.upper(), TIPO_CLIENTE_PESOS["DEFAULT"])

        # Hora de llegada se usa directamente para el desempate FIFO
        return (prioridad_reserva, prioridad_espera, prioridad_tipo, cliente.hora_llegada)


    # Modificar para usar el ID interno del gestor
    def registrar_llegada(self, nombre_grupo: str, tiene_reserva: bool, tipo_cliente: str, numero_personas: int) -> str:
        """
        Registra la llegada de un nuevo grupo de clientes.
        Avanza el tiempo de simulación por defecto.
        """
        # Primero avanzar tiempo para reflejar el momento de la llegada en la simulación
        self.avanzar_tiempo_simulacion(self.minutos_por_operacion, False) # Avanzar tiempo SIN intentar asignar inmediatamente

        if not (1 <= numero_personas <= 6):
            return f"Error: Tamaño de grupo inválido ({numero_personas}). Debe ser entre 1 y 6 personas."

        if self.cola_espera.size() >= self.limite_grupos_cola:
            return "Error: Lo sentimos, la lista de espera está llena en este momento."

        # Crear cliente usando el ID del gestor y la hora actual de la simulación
        nuevo_cliente = Cliente(nombre_grupo, tiene_reserva, tipo_cliente, numero_personas)
        nuevo_cliente.id_cliente = self._get_next_client_id() # Usar el ID del gestor
        nuevo_cliente.hora_llegada = self.tiempo_simulacion_actual # Hora actual de la simulación

        # La prioridad inicial se calcula basada en la hora de llegada (que ya es self.tiempo_simulacion_actual)
        # y asumiendo que el tiempo de espera NO ha excedido *en este instante de llegada*.
        # La tupla de prioridad almacenada con la solicitud no necesita recalcularse,
        # solo se recalcula 'espera_excedida' al momento de intentar asignar.
        prioridad_inicial = self._calcular_prioridad_tupla(nuevo_cliente, self.tiempo_simulacion_actual) # Calcular con hora actual
        solicitud = SolicitudAsignacion(nuevo_cliente, prioridad_inicial)

        self.cola_espera.add(solicitud)
        return f"Grupo '{nombre_grupo}' (ID: {nuevo_cliente.id_cliente}) añadido a la cola de espera."


    def liberar_mesa(self, id_mesa: int) -> str:
        """
        Marca una mesa como disponible y avanza el tiempo.
        Intenta asignar una nueva mesa después de liberar.
        """
        mesa_encontrada = None
        for mesa in self.mesas:
            if mesa.id_mesa == id_mesa:
                mesa_encontrada = mesa
                break

        if not mesa_encontrada:
            return f"Error: Mesa ID {id_mesa} no encontrada."
        if mesa_encontrada.disponible:
            return f"Error: Mesa ID {id_mesa} ya está disponible."

        cliente_id_anterior = mesa_encontrada.cliente_asignado_id
        mesa_encontrada.liberar_mesa()
        self.avanzar_tiempo_simulacion(self.minutos_por_operacion, False) # Avanzar tiempo SIN intentar asignar

        # Intentar asignar una mesa después de liberar
        mensaje_asignacion = self.intentar_asignar_mesa(False) # No avanzar tiempo de nuevo
        return (f"Mesa {id_mesa} liberada (antes ocupada por Cliente ID: {cliente_id_anterior}).\n"
                f"-> Resultado intento de asignación: {mensaje_asignacion}") # Mejorar mensaje


    def intentar_asignar_mesa(self, avanzar_tiempo_primero: bool = True) -> str:
        """
        Intenta asignar la mesa más adecuada al cliente con mayor prioridad en la cola.
        Si avanzar_tiempo_primero es True, primero simula el paso de tiempo antes de intentar.
        """
        if avanzar_tiempo_primero:
             # Si se llama con avanzar_tiempo_primero=True, simular el paso de tiempo
             # antes de evaluar prioridades y buscar mesa.
            self.avanzar_tiempo_simulacion(self.minutos_por_operacion, False) # Avanzar tiempo SIN intentar asignar de nuevo


        if self.cola_espera.is_empty():
            return "No hay clientes en la cola de espera para asignar."

        # Obtener el cliente de mayor prioridad SIN removerlo aún
        solicitud_prioritaria = self.cola_espera.peek()
        if not solicitud_prioritaria: return "Error interno: No se pudo obtener cliente de la cola."

        cliente_a_asignar = solicitud_prioritaria.cliente
        tiempo_esperando_delta = self.tiempo_simulacion_actual - cliente_a_asignar.hora_llegada
        tiempo_esperando_mins = tiempo_esperando_delta.total_seconds() / 60
        espera_excedida = tiempo_esperando_mins >= self.limite_tiempo_espera_mins

        # **Nota:** La prioridad efectiva (si excedió espera) se usa AQUÍ para la lógica de búsqueda de mesa,
        # no para reordenar la cola antes de peek. La cola está ordenada por la prioridad de inserción.

        mesa_asignada = None
        # Ordenar mesas disponibles por capacidad para asignar la más pequeña adecuada primero
        mesas_disponibles_ordenadas = sorted([m for m in self.mesas if m.disponible], key=lambda m: m.capacidad)

        # Lógica de búsqueda de mesa basada en prioridad y reglas:
        # 1. Buscar mesa con capacidad exacta
        for mesa in mesas_disponibles_ordenadas:
            if mesa.capacidad == cliente_a_asignar.numero_personas:
                mesa_asignada = mesa
                break

        # 2. Si no hay exacta y NO ha excedido el tiempo de espera, buscar la más pequeña >= capacidad_cliente
        #    con una diferencia de capacidad 'ligera'.
        if not mesa_asignada and not espera_excedida:
             for mesa in mesas_disponibles_ordenadas:
                 if mesa.capacidad >= cliente_a_asignar.numero_personas:
                     # Definir "ligera": Capacidad de mesa <= Capacidad cliente + 2 personas como ejemplo
                     if (mesa.capacidad - cliente_a_asignar.numero_personas) <= 2:
                          mesa_asignada = mesa
                          break
                     # Si la diferencia es mayor a 2, seguir buscando una mesa más pequeña


        # 3. Si no hay mesa asignada todavía (puede ser porque no hay exacta, o no hay "ligeramente" más grande para no-espera-excedida)
        #    y SI ha excedido el tiempo de espera, buscar CUALQUIER mesa >= capacidad_cliente
        if not mesa_asignada and espera_excedida:
             for mesa in mesas_disponibles_ordenadas:
                 if mesa.capacidad >= cliente_a_asignar.numero_personas:
                     mesa_asignada = mesa # Asignar a la primera mesa disponible que quepa
                     break

        # 4. Si no ha excedido el tiempo de espera y no se encontró exacta o "ligera", reintentar
        #    con CUALQUIER mesa >= capacidad_cliente (última opción para no exceder espera)
        #    Esto podría ser redundante si la búsqueda 2 ya cubrió todos los casos > exacto.
        #    Simplifiquemos: si no se encontró en 1 o 2 y NO excedió espera, no asignamos una mesa muy grande.
        #    Si se encontró en 3 (porque excedió espera), usamos esa.


        if mesa_asignada:
            # Remover de la cola la solicitud que se va a atender
            self.cola_espera.pop()
            mesa_asignada.asignar_cliente(cliente_a_asignar.id_cliente)
            mesa_asignada.hora_asignacion = self.tiempo_simulacion_actual # Registrar hora de asignación real
            return (f"Cliente ID {cliente_a_asignar.id_cliente} ('{cliente_a_asignar.nombre_grupo}') "
                    f"asignado a Mesa {mesa_asignada.id_mesa} (Cap: {mesa_asignada.capacidad}p). "
                    f"Esperó {tiempo_esperando_mins:.1f} mins.")
        else:
            # Si espera excedida, indicar que se intentó con mesas más grandes
            if espera_excedida:
                 return (f"Cliente ID {cliente_a_asignar.id_cliente} ('{cliente_a_asignar.nombre_grupo}') "
                         f"no pudo ser asignado. Tiempo de espera {tiempo_esperando_mins:.1f} mins. "
                         f"Se consideraron mesas más grandes. No hay mesas adecuadas disponibles.")
            else:
                 return (f"Cliente ID {cliente_a_asignar.id_cliente} ('{cliente_a_asignar.nombre_grupo}') "
                         f"no pudo ser asignado. No hay mesas adecuadas disponibles en este momento. "
                         f"Tiempo de espera actual: {tiempo_esperando_mins:.1f} mins.")


    def avanzar_tiempo_simulacion(self, minutos: int = 5, intentar_asignacion: bool = True):
        """
        Avanza el tiempo de la simulación.
        Si intentar_asignacion es True, intenta asignar mesas después.
        """
        if minutos < 0:
            # Manejar minutos negativos si es necesario, o simplemente no avanzar
            # Por ahora, simplemente no hacemos nada o mostramos un error
            return "Error: No se puede avanzar el tiempo con minutos negativos."

        if minutos > 0:
            self.tiempo_simulacion_actual += datetime.timedelta(minutes=minutos)
            print(f"Tiempo avanzado. Hora actual de simulación: {self.tiempo_simulacion_actual.strftime('%Y-%m-%d %H:%M:%S')}")
        else:
             print(f"Hora actual de simulación: {self.tiempo_simulacion_actual.strftime('%Y-%m-%d %H:%M:%S')}")


        if intentar_asignacion:
            # Después de avanzar el tiempo, verificar si se pueden asignar mesas
            print("Intentando asignar mesas después de avanzar el tiempo...")
            # Bucle para asignar mientras haya clientes en cola y mesas potencialmente disponibles
            # y el intento de asignacion tenga éxito (no retorne un mensaje de 'no asignado')
            while not self.cola_espera.is_empty() and any(m.disponible for m in self.mesas):
                # No avanzar tiempo de nuevo dentro del bucle, solo intentar asignar
                resultado_asignacion = self.intentar_asignar_mesa(False)
                # Si la asignación fue exitosa, el mensaje contiene "asignado a Mesa"
                if "asignado a Mesa" in resultado_asignacion:
                    print(resultado_asignacion)
                else:
                    # Si el intento no resultó en una asignación, salir del bucle de reintento
                    print(resultado_asignacion) # Mostrar por qué no se asignó
                    break # Salir del bucle de reintento


    def get_cola_espera_formateada(self) -> List[SolicitudAsignacion]:
        """Retorna la lista de solicitudes de la cola, ordenada por prioridad, para la Vista."""
        # La vista ahora manejará el formateo a string/tabla
        return self.cola_espera.get_all_items()

    def get_estado_mesas_formateado(self) -> List[Mesa]:
        """Retorna la lista de mesas para la Vista."""
        # La vista ahora manejará el formateo a string/tabla
        return self.mesas # La vista puede ordenarlas si es necesario


    def get_reglas_prioridad_formateadas(self) -> List[str]:
        # ... (igual que antes, no cambia) ...
        return [
            "--- Reglas de Prioridad (Menor valor de tupla = Mayor Prioridad) ---",
            "La prioridad se determina por la tupla: (Reserva, Espera_Excedida, Tipo_Cliente, Hora_Llegada)",
            "1. Reserva: Con Reserva (0) > Sin Reserva (1)",
            "2. Espera Excedida (>=15 min): Sí (0) > No (1)",
            "3. Tipo de Cliente: VIP (1) > REGULAR (2) > OCASIONAL (3)",
            "4. Hora de Llegada: Más temprano es mejor (timestamp)",
            "Si un grupo espera >= 15 minutos, se considerarán mesas más grandes si es necesario.",
            "-------------------------------------------------------------------"
        ]

    # --- Método de Resumen (Nuevo) ---
    def get_status_summary(self) -> Dict[str, Any]:
        """Retorna un diccionario con datos para el resumen del sistema."""
        available_tables = sum(1 for mesa in self.mesas if mesa.disponible)
        total_tables = len(self.mesas)
        queue_size = self.cola_espera.size()

        return {
            "current_time": self.tiempo_simulacion_actual,
            "queue_size": queue_size,
            "available_tables": available_tables,
            "total_tables": total_tables
        }

    # --- Método de Caso Base (Nuevo) ---
    def setup_base_case(self):
        """Configura un estado inicial predefinido para demostración."""
        print("\n--- Configurando Caso Base ---")
        # Resetear el estado del gestor
        self._inicializar_mesas()
        self.cola_espera = ColaPrioridadPropia() # Crear una nueva cola vacía
        self.proximo_id_cliente = 1 # Resetear contador de IDs
        # Establecer una hora de inicio clara para el caso base
        self.tiempo_simulacion_actual = datetime.datetime(2024, 1, 1, 18, 0, 0) # 6:00 PM

        # Simular llegadas en diferentes momentos y con diferentes atributos
        # Crear clientes y solicitudes manualmente para controlar la hora de llegada
        # y el estado de reserva/tipo, etc.

        # Grupo 1: Llegó temprano, reserva VIP, 4p
        c1 = Cliente("Grupo A (VIP R)", True, "VIP", 4)
        c1.id_cliente = self._get_next_client_id()
        c1.hora_llegada = self.tiempo_simulacion_actual - datetime.timedelta(minutes=10) # Llegó hace 10 min
        self.cola_espera.add(SolicitudAsignacion(c1, self._calcular_prioridad_tupla(c1, self.tiempo_simulacion_actual)))

        # Grupo 2: Llegó más temprano, sin reserva regular, 2p
        c2 = Cliente("Grupo B (SinR Reg)", False, "REGULAR", 2)
        c2.id_cliente = self._get_next_client_id()
        c2.hora_llegada = self.tiempo_simulacion_actual - datetime.timedelta(minutes=12) # Llegó hace 12 min
        self.cola_espera.add(SolicitudAsignacion(c2, self._calcular_prioridad_tupla(c2, self.tiempo_simulacion_actual)))

        # Grupo 3: Llegó hace mucho, sin reserva ocasional, 6p (Espera Excedida si >15min)
        c3 = Cliente("Grupo C (SinR Ocas)", False, "OCASIONAL", 6)
        c3.id_cliente = self._get_next_client_id()
        c3.hora_llegada = self.tiempo_simulacion_actual - datetime.timedelta(minutes=20) # Llegó hace 20 min (EXCEDIDO)
        self.cola_espera.add(SolicitudAsignacion(c3, self._calcular_prioridad_tupla(c3, self.tiempo_simulacion_actual))) # Prioridad calculada ahora

        # Grupo 4: Llegó hace poco, reserva regular, 4p
        c4 = Cliente("Grupo D (Reserva Reg)", True, "REGULAR", 4)
        c4.id_cliente = self._get_next_client_id()
        c4.hora_llegada = self.tiempo_simulacion_actual - datetime.timedelta(minutes=5) # Llegó hace 5 min
        self.cola_espera.add(SolicitudAsignacion(c4, self._calcular_prioridad_tupla(c4, self.tiempo_simulacion_actual)))

         # Grupo 5: Llegó ahora, sin reserva VIP, 2p
        c5 = Cliente("Grupo E (SinR VIP)", False, "VIP", 2)
        c5.id_cliente = self._get_next_client_id()
        c5.hora_llegada = self.tiempo_simulacion_actual # Llegó Justo Ahora
        self.cola_espera.add(SolicitudAsignacion(c5, self._calcular_prioridad_tupla(c5, self.tiempo_simulacion_actual)))

        # Grupo 6: Llegó hace un tiempo, sin reserva regular, 3p
        c6 = Cliente("Grupo F (SinR Reg)", False, "REGULAR", 3)
        c6.id_cliente = self._get_next_client_id()
        c6.hora_llegada = self.tiempo_simulacion_actual - datetime.timedelta(minutes=8) # Llegó hace 8 min
        self.cola_espera.add(SolicitudAsignacion(c6, self._calcular_prioridad_tupla(c6, self.tiempo_simulacion_actual)))

        # Asignar algunas mesas para simular ocupación
        # Mesas 1-4 (Cap 6), 5-10 (Cap 4), 11-12 (Cap 2)
        # Ocupar 6 mesas (mitad)
        self.mesas[0].asignar_cliente(-1) # Mesa 1 (Cap 6) - Ocupada (dummy client)
        self.mesas[1].asignar_cliente(-2) # Mesa 2 (Cap 6) - Ocupada (dummy client)
        self.mesas[4].asignar_cliente(-3) # Mesa 5 (Cap 4) - Ocupada (dummy client)
        self.mesas[5].asignar_cliente(-4) # Mesa 6 (Cap 4) - Ocupada (dummy client)
        self.mesas[10].asignar_cliente(-5) # Mesa 11 (Cap 2) - Ocupada (dummy client)
        self.mesas[11].asignar_cliente(-6) # Mesa 12 (Cap 2) - Ocupada (dummy client)

        # Nota: La hora de asignación de estas mesas ocupadas manualmente no es crucial
        # para la lógica de asignación, solo su estado `disponible = False`.

        print("Caso base configurado.")
        self.avanzar_tiempo_simulacion(0, False) # Mostrar hora actual sin avanzar ni intentar asignar
        # Opcional: Mostrar estado inicial después del caso base
        # print(self.get_status_summary())
        # self.vista.mostrar_cola_espera(self.get_cola_espera_formateada(), self.tiempo_simulacion_actual)
        # self.vista.mostrar_estado_mesas(self.get_estado_mesas_formateado())


    # --- Métodos de Cancelación y Ajuste de Hora (No implementados según solicitud) ---
    # def cancel_group(self, cliente_id: int) -> str: ...
    # def set_arrival_time(self, cliente_id: int, new_time_str: str) -> str: ...
    # ... (resto de la clase igual) ...  