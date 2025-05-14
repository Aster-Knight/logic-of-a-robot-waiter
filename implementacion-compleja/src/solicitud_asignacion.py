# src/solicitud_asignacion.py

from .cliente import Cliente # Importar la clase Cliente

class SolicitudAsignacion:
    """
    Encapsula un cliente y su prioridad calculada para la cola de prioridad.
    """
    # Contador para el tie-breaker en la cola de prioridad
    _contador_insercion = 0

    def __init__(self, cliente: Cliente, prioridad_tupla: tuple):
        """
        Inicializa una nueva solicitud de asignación.

        Args:
            cliente (Cliente): La instancia del cliente asociado.
            prioridad_tupla (tuple): La tupla de prioridad calculada.
                                     Menor valor indica mayor prioridad.
                                     Ej: (reserva, espera_excedida, tipo_cliente_val, hora_llegada_ts)
        """
        self.cliente: Cliente = cliente
        self.prioridad_calculada: tuple = prioridad_tupla # La tupla que se usa para comparar
        self.orden_insercion: int = SolicitudAsignacion._contador_insercion
        SolicitudAsignacion._contador_insercion += 1

    # Métodos de comparación para que heapq pueda ordenar directamente las SolicitudAsignacion
    # si las usamos como elementos del heap. Comparará primero prioridad_calculada, luego orden_insercion.
    def __lt__(self, other: 'SolicitudAsignacion') -> bool:
        if self.prioridad_calculada != other.prioridad_calculada:
            return self.prioridad_calculada < other.prioridad_calculada
        return self.orden_insercion < other.orden_insercion # Tie-breaker FIFO

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, SolicitudAsignacion):
            return NotImplemented
        return (self.prioridad_calculada == other.prioridad_calculada and
                self.orden_insercion == other.orden_insercion)

    def __repr__(self):
        return f"Solicitud(Prioridad={self.prioridad_calculada}, ClienteID={self.cliente.id_cliente})"