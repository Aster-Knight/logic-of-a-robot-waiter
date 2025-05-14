# src/cliente.py

import datetime

class Cliente:
    """
    Representa un grupo de clientes esperando asignación de mesa.
    """
    _next_id = 1 # Class variable para generar IDs únicos

    def __init__(self, nombre_grupo: str, tiene_reserva: bool, tipo_cliente: str, numero_personas: int):
        """
        Inicializa un nuevo cliente/grupo.

        Args:
            nombre_grupo (str): Nombre o identificador del grupo.
            tiene_reserva (bool): True si el grupo tiene una reserva.
            tipo_cliente (str): Categoría del cliente (ej: "VIP", "REGULAR", "OCASIONAL").
            numero_personas (int): Número de personas en el grupo (1-6).
        """
        self.id_cliente: int = Cliente._next_id
        Cliente._next_id += 1

        self.nombre_grupo: str = nombre_grupo
        self.hora_llegada: datetime.datetime = datetime.datetime.now() # Se asigna al crear
        self.tiene_reserva: bool = tiene_reserva
        self.tipo_cliente: str = tipo_cliente.upper() # Estandarizar a mayúsculas
        self.numero_personas: int = numero_personas
        # El tiempo de espera se calculará dinámicamente en el GestorRestaurante

    def __repr__(self):
        return (f"Cliente(ID={self.id_cliente}, Nombre='{self.nombre_grupo}', Personas={self.numero_personas}, "
                f"Reserva={self.tiene_reserva}, Tipo='{self.tipo_cliente}', Llegada='{self.hora_llegada.strftime('%H:%M:%S')}')")

    def __str__(self):
        reserva_str = "Con Reserva" if self.tiene_reserva else "Sin Reserva"
        return (f"[{self.id_cliente}] Grupo '{self.nombre_grupo}' ({self.numero_personas}p, {self.tipo_cliente}, {reserva_str}, "
                f"Llegó: {self.hora_llegada.strftime('%H:%M:%S')})")
    