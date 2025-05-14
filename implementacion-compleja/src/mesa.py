# src/mesa.py

import datetime

class Mesa:
    """
    Representa una mesa en el restaurante.
    """
    def __init__(self, id_mesa: int, capacidad: int):
        """
        Inicializa una nueva mesa.

        Args:
            id_mesa (int): Identificador único de la mesa.
            capacidad (int): Número máximo de personas que puede albergar (2, 4 o 6).
        """
        self.id_mesa: int = id_mesa
        self.capacidad: int = capacidad
        self.disponible: bool = True
        self.cliente_asignado_id: int | None = None
        self.hora_asignacion: datetime.datetime | None = None

    def asignar_cliente(self, cliente_id: int):
        """Marca la mesa como ocupada y asigna un cliente."""
        if self.disponible:
            self.disponible = False
            self.cliente_asignado_id = cliente_id
            self.hora_asignacion = datetime.datetime.now() # Hora actual de asignación
        else:
            # Idealmente, esto no debería suceder si la lógica de asignación es correcta
            print(f"Advertencia: Intentando asignar cliente a mesa {self.id_mesa} ya ocupada.")

    def liberar_mesa(self):
        """Marca la mesa como disponible."""
        self.disponible = True
        self.cliente_asignado_id = None
        self.hora_asignacion = None

    def __repr__(self):
        estado = "Disponible" if self.disponible else f"Ocupada por Cliente ID: {self.cliente_asignado_id}"
        return f"Mesa(ID={self.id_mesa}, Capacidad={self.capacidad}, Estado='{estado}')"

    def __str__(self):
        estado_str = "Disponible" if self.disponible else f"Ocupada (Cliente ID: {self.cliente_asignado_id or 'N/A'})"
        return f"Mesa {self.id_mesa} (Cap: {self.capacidad}p) - {estado_str}"
    