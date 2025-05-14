# src/gestor_restaurante.py

import datetime
from typing import List, Tuple, Dict, Optional # Para type hints más claros

from .cliente import Cliente
from .mesa import Mesa
from .solicitud_asignacion import SolicitudAsignacion
from .cola_prioridad_propia import ColaPrioridadPropia

# Definir pesos para tipos de cliente (menor valor = mayor prioridad)
TIPO_CLIENTE_PESOS: Dict[str, int] = {
    "VIP": 1,
    "REGULAR": 2,
    "OCASIONAL": 3,
    "DEFAULT": 99 # Por si se añade un tipo no reconocido
}

class GestorRestaurante:
    """
    Encapsula la lógica de negocio para la gestión de mesas y clientes.
    Componente central del Modelo en MVC.
    """
    def __init__(self):
        self.mesas: List[Mesa] = []
        self.cola_espera: ColaPrioridadPropia = ColaPrioridadPropia()
        self.tiempo_simulacion_actual: datetime.datetime = datetime.datetime.now() # Hora de inicio
        self.proximo_id_cliente_manual: int = 1 # Para la simulación, reemplaza Cliente._next_id si es necesario
        self.limite_grupos_cola: int = 15
        self.limite_tiempo_espera_mins: int = 15
        self.minutos_por_operacion: int = 5 # Tiempo que avanza con ciertas operaciones

        self._inicializar_mesas()
        Cliente._next_id = 1 # Resetear el contador de IDs de cliente al iniciar el gestor

    def _inicializar_mesas(self):
        """Crea las 12 instancias de Mesa con sus capacidades."""
        self.mesas = []
        # Mesas 1 a 4: 6 personas
        for i in range(1, 5):
            self.mesas.append(Mesa(id_mesa=i, capacidad=6))
        # Mesas 5 a 10: 4 personas
        for i in range(5, 11):
            self.mesas.append(Mesa(id_mesa=i, capacidad=4))
        # Mesas 11 y 12: 2 personas
        for i in range(11, 13):
            self.mesas.append(Mesa(id_mesa=i, capacidad=2))

    def _calcular_prioridad_tupla(self, cliente: Cliente, tiempo_espera_excedido: bool) -> Tuple[int, int, int, datetime.datetime]:
        """
        Calcula la tupla de prioridad para un cliente.
        Menores valores indican mayor prioridad.
        Formato: (reserva, espera_excedida, tipo_cliente_val, hora_llegada)
        """
        # Prioridad por reserva (0 si tiene, 1 si no tiene)
        prioridad_reserva = 0 if cliente.tiene_reserva else 1
        # Prioridad por tiempo de espera excedido (0 si excedió, 1 si no)
        prioridad_espera = 0 if tiempo_espera_excedido else 1
        # Prioridad por tipo de cliente
        prioridad_tipo = TIPO_CLIENTE_PESOS.get(cliente.tipo_cliente.upper(), TIPO_CLIENTE_PESOS["DEFAULT"])

        # Hora de llegada se usa directamente (timestamps más tempranos son mejores como tie-breaker)
        return (prioridad_reserva, prioridad_espera, prioridad_tipo, cliente.hora_llegada)

    def registrar_llegada(self, nombre_grupo: str, tiene_reserva: bool, tipo_cliente: str, numero_personas: int) -> str:
        """
        Registra la llegada de un nuevo grupo de clientes.
        Avanza el tiempo de simulación por defecto.
        """
        self.avanzar_tiempo_simulacion(self.minutos_por_operacion, False) # Avanzar tiempo SIN intentar asignar

        if not (1 <= numero_personas <= 6):
            return f"Error: Tamaño de grupo inválido ({numero_personas}). Debe ser entre 1 y 6 personas."

        if self.cola_espera.size() >= self.limite_grupos_cola:
            return "Error: Lo sentimos, la lista de espera está llena en este momento."

        nuevo_cliente = Cliente(nombre_grupo, tiene_reserva, tipo_cliente, numero_personas)
        nuevo_cliente.hora_llegada = self.tiempo_simulacion_actual # Asegurar que la hora de llegada es la actual de la simulación

        # Para la prioridad inicial, el tiempo de espera aún no ha excedido
        prioridad_inicial = self._calcular_prioridad_tupla(nuevo_cliente, False)
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
        mensaje_liberacion = f"Mesa {id_mesa} liberada (antes ocupada por Cliente ID: {cliente_id_anterior})."

        # Intentar asignar una mesa después de liberar
        mensaje_asignacion = self.intentar_asignar_mesa(False) # No avanzar tiempo de nuevo
        return f"{mensaje_liberacion}\n{mensaje_asignacion}"


    def intentar_asignar_mesa(self, avanzar_tiempo_primero: bool = True) -> str:
        """
        Intenta asignar la mesa más adecuada al cliente con mayor prioridad en la cola.
        """
        if avanzar_tiempo_primero:
            self.avanzar_tiempo_simulacion(0, False) # Actualizar tiempo de espera sin avanzar reloj

        if self.cola_espera.is_empty():
            return "No hay clientes en la cola de espera."

        # Obtener el cliente de mayor prioridad SIN removerlo aún
        solicitud_prioritaria = self.cola_espera.peek()
        if not solicitud_prioritaria: return "Error interno: No se pudo obtener cliente de la cola." # Debería ser redundante con is_empty

        cliente_a_asignar = solicitud_prioritaria.cliente
        tiempo_esperando_delta = self.tiempo_simulacion_actual - cliente_a_asignar.hora_llegada
        tiempo_esperando_mins = tiempo_esperando_delta.total_seconds() / 60
        espera_excedida = tiempo_esperando_mins >= self.limite_tiempo_espera_mins

        # Si el tiempo de espera ha cambiado el estado de "espera_excedida", la prioridad
        # necesita ser re-evaluada para esta solicitud particular antes de la búsqueda de mesa.
        # No es necesario reconstruir toda la cola, solo considerar este estado para la búsqueda.

        mesa_asignada = None
        mesas_disponibles_ordenadas = sorted([m for m in self.mesas if m.disponible], key=lambda m: m.capacidad)

        # Buscar mesa
        # 1. Buscar mesa con capacidad exacta
        for mesa in mesas_disponibles_ordenadas:
            if mesa.capacidad == cliente_a_asignar.numero_personas:
                mesa_asignada = mesa
                break
        # 2. Si no hay exacta, buscar la más pequeña que sea >= capacidad_cliente
        if not mesa_asignada:
            for mesa in mesas_disponibles_ordenadas:
                if mesa.capacidad >= cliente_a_asignar.numero_personas:
                    # Lógica para "ligeramente superior" (podría ser más compleja)
                    # Por ahora, tomamos la primera que cumpla ser >=
                    # Si la espera NO ha sido excedida, podríamos ser más restrictivos
                    if not espera_excedida and (mesa.capacidad - cliente_a_asignar.numero_personas) <= 2: # Ejemplo: no más de 2 asientos extra
                        mesa_asignada = mesa
                        break
                    elif espera_excedida: # Si la espera fue excedida, cualquier mesa que quepa es válida
                        mesa_asignada = mesa
                        break
                    # Si no excede espera y la mesa es muy grande, seguir buscando

        # 3. Si la espera fue excedida y aún no hay mesa, reintentar con menos restricciones
        if not mesa_asignada and espera_excedida:
             for mesa in mesas_disponibles_ordenadas:
                if mesa.capacidad >= cliente_a_asignar.numero_personas:
                    mesa_asignada = mesa # Asignar a la primera mesa disponible más grande
                    break


        if mesa_asignada:
            # Remover de la cola la solicitud que se va a atender (ahora sí la sacamos)
            self.cola_espera.pop()
            mesa_asignada.asignar_cliente(cliente_a_asignar.id_cliente)
            mesa_asignada.hora_asignacion = self.tiempo_simulacion_actual # Registrar hora de asignación real
            return (f"Cliente ID {cliente_a_asignar.id_cliente} ('{cliente_a_asignar.nombre_grupo}') "
                    f"asignado a Mesa {mesa_asignada.id_mesa}. Esperó {tiempo_esperando_mins:.1f} mins.")
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
        """Avanza el tiempo de la simulación e intenta asignar mesas."""
        if minutos > 0:
            self.tiempo_simulacion_actual += datetime.timedelta(minutes=minutos)
            print(f"Tiempo avanzado. Hora actual de simulación: {self.tiempo_simulacion_actual.strftime('%Y-%m-%d %H:%M:%S')}")

        if intentar_asignacion:
            # Después de avanzar el tiempo, verificar si se pueden asignar mesas
            print("Intentando asignar mesas después de avanzar el tiempo...")
            # Bucle para asignar mientras haya clientes en cola y mesas potencialmente disponibles
            # Esto es un poco simplista, podría haber una lógica más compleja de "eventos"
            while not self.cola_espera.is_empty() and any(m.disponible for m in self.mesas):
                resultado_asignacion = self.intentar_asignar_mesa(False) # No avanzar tiempo de nuevo dentro del bucle
                if "asignado a Mesa" in resultado_asignacion:
                    print(resultado_asignacion)
                else:
                    # Si no se pudo asignar al primero de la cola, probablemente no se pueda a otros
                    # a menos que la lógica de búsqueda cambie.
                    print(resultado_asignacion) # Mostrar por qué no se asignó
                    break # Salir del bucle de reintento si el primero no se pudo asignar

    def get_cola_espera_formateada(self) -> List[str]:
        """Prepara una lista de strings para mostrar la cola de espera."""
        if self.cola_espera.is_empty():
            return ["La cola de espera está vacía."]

        lineas_cola = ["--- Cola de Espera (Mayor Prioridad Primero) ---"]
        # Usar el método de la cola que devuelve los items ordenados
        solicitudes_ordenadas = self.cola_espera.get_all_items()

        for solicitud in solicitudes_ordenadas:
            cliente = solicitud.cliente
            tiempo_esperando_delta = self.tiempo_simulacion_actual - cliente.hora_llegada
            tiempo_esperando_mins = tiempo_esperando_delta.total_seconds() / 60
            espera_excedida = tiempo_esperando_mins >= self.limite_tiempo_espera_mins

            # Recalcular la tupla de prioridad actual para mostrarla (no para reordenar la cola aquí)
            # La tupla en SolicitudAsignacion es la de inserción.
            # Aquí mostramos una "prioridad efectiva" basada en el estado actual.
            prioridad_efectiva_tupla = self._calcular_prioridad_tupla(cliente, espera_excedida)

            lineas_cola.append(
                f"ID Cliente: {cliente.id_cliente}, Grupo: '{cliente.nombre_grupo}', Personas: {cliente.numero_personas}, "
                f"Reserva: {'Sí' if cliente.tiene_reserva else 'No'}, Tipo: {cliente.tipo_cliente}, "
                f"Llegó: {cliente.hora_llegada.strftime('%H:%M:%S')}, Esperando: {tiempo_esperando_mins:.1f} mins, "
                f"Prioridad: {prioridad_efectiva_tupla} {'(Espera Excedida!)' if espera_excedida else ''}"
            )
        lineas_cola.append("------------------------------------------")
        return lineas_cola

    def get_estado_mesas_formateado(self) -> List[str]:
        """Prepara una lista de strings para mostrar el estado de todas las mesas."""
        lineas_mesas = ["--- Estado de las Mesas ---"]
        for mesa in sorted(self.mesas, key=lambda m: m.id_mesa): # Ordenar por ID de mesa
            estado_str = "Disponible"
            info_adicional = ""
            if not mesa.disponible:
                estado_str = f"Ocupada por Cliente ID: {mesa.cliente_asignado_id or 'N/A'}"
                if mesa.hora_asignacion:
                    info_adicional = f" (Desde: {mesa.hora_asignacion.strftime('%H:%M:%S')})"
            lineas_mesas.append(f"Mesa {mesa.id_mesa} (Cap: {mesa.capacidad}p): {estado_str}{info_adicional}")
        lineas_mesas.append("--------------------------")
        return lineas_mesas

    def get_reglas_prioridad_formateadas(self) -> List[str]:
        """Retorna una descripción de las reglas de prioridad."""
        return [
            "--- Reglas de Prioridad (Menor valor de tupla = Mayor Prioridad) ---",
            "La prioridad se determina por la tupla: (Reserva, Espera_Excedida, Tipo_Cliente, Hora_Llegada)",
            "1. Reserva: Con Reserva (0) > Sin Reserva (1)",
            "2. Espera Excedida (>=15 min): Sí (0) > No (1)",
            "3. Tipo de Cliente: VIP (1) > REGULAR (2) > OCASIONAL (3)",
            "4. Hora de Llegada: Más temprano es mejor (timestamp como desempate)",
            "Si un grupo espera >= 15 minutos, se considerarán mesas más grandes si es necesario.",
            "-------------------------------------------------------------------"
        ]