# src/vista_consola.py

from typing import List, Dict, Any, Optional
import datetime # Para formatear fechas y horas en tablas

# Importar clases del modelo para type hinting en funciones de formateo
from .cliente import Cliente
from .mesa import Mesa
from .solicitud_asignacion import SolicitudAsignacion


class VistaConsola:
    """
    Responsable de toda la interacción de entrada/salida con el usuario en la consola.
    Incluye formateo para tablas y elementos visuales simples (ASCII art).
    """
    def mostrar_mensaje(self, mensaje: str, es_error: bool = False):
        """Muestra un mensaje al usuario."""
        prefijo = "Error: " if es_error else ""
        print(f"{prefijo}{mensaje}")

    def solicitar_input(self, prompt: str) -> str:
        """Solicita una entrada de texto al usuario."""
        return input(prompt).strip()

    def mostrar_menu_principal(self):
        """Muestra el menú de ayuda principal."""
        # Ahora la ayuda general se obtiene directamente de la funcion mostrar_ayuda_comandos
        self.mostrar_ayuda_comandos()


    def mostrar_ayuda_comandos(self): # Ya no necesita parsers como argumento si la ayuda es fija
        """Muestra la ayuda general de los comandos."""
        print("\n--- Comandos Disponibles ---")
        print("  add <nombre> --personas <num> [--reserva] [--tipo <VIP|REGULAR|OCASIONAL>]")
        print("    Registra la llegada de un nuevo grupo. Alias: ad, add-group")
        print("  free <id_mesa>")
        print("    Marca una mesa como desocupada. Alias: fr, free-table")
        print("  assign")
        print("    Intenta asignar la siguiente mesa según prioridad. Alias: as, assign-table")
        print("  time [<minutos>]")
        print("    Avanza el tiempo de la simulación (por defecto 5). Alias: ti, advance, advance-time")
        print("  queue")
        print("    Muestra la cola de espera actual (tabla). Alias: qu, view-queue")
        print("  tables")
        print("    Muestra el estado de todas las mesas (tabla). Alias: ta, view-tables")
        print("  rules")
        print("    Muestra las reglas de prioridad. Alias: ru, view-rules")
        print("  status")
        print("    Muestra un resumen del sistema. Alias: st")
        print("  base-case")
        print("    Configura un caso base de prueba con grupos y mesas ocupadas. Alias: bc, setup-base")
        print("  help")
        print("    Muestra este mensaje de ayuda.")
        print("  exit")
        print("    Sale de la aplicación.")
        print("-----------------------------")


    def solicitar_datos_nuevo_grupo(self) -> Dict[str, Any] | None:
        # ... (igual que antes, no cambia) ...
        print("\n--- Registrar Nuevo Grupo ---")
        nombre_grupo = self.solicitar_input("Nombre del grupo: ")
        if not nombre_grupo:
            self.mostrar_mensaje("Registro cancelado (nombre vacío).", es_error=True)
            return None

        try:
            numero_personas_str = self.solicitar_input("Número de personas (1-6): ")
            numero_personas = int(numero_personas_str)
            if not (1 <= numero_personas <= 6):
                self.mostrar_mensaje("Número de personas inválido.", es_error=True)
                return None
        except ValueError:
            self.mostrar_mensaje("Número de personas debe ser un entero.", es_error=True)
            return None

        reserva_str = self.solicitar_input("¿Tiene reserva? (s/n): ").lower()
        tiene_reserva = reserva_str == 's'

        tipo_cliente_str = self.solicitar_input("Tipo de cliente (VIP, REGULAR, OCASIONAL - por defecto REGULAR): ").upper()
        if tipo_cliente_str not in ["VIP", "REGULAR", "OCASIONAL"]:
            if tipo_cliente_str == "":
                tipo_cliente_str = "REGULAR"
            else:
                self.mostrar_mensaje("Tipo de cliente inválido. Se usará REGULAR.", es_error=True)
                tipo_cliente_str = "REGULAR"

        return {
            "nombre_grupo": nombre_grupo,
            "numero_personas": numero_personas,
            "tiene_reserva": tiene_reserva,
            "tipo_cliente": tipo_cliente_str,
        }

    def solicitar_id_mesa(self, prompt: str) -> int | None:
        # ... (igual que antes, no cambia) ...
        try:
            id_mesa_str = self.solicitar_input(prompt)
            return int(id_mesa_str)
        except ValueError:
            self.mostrar_mensaje("ID de mesa inválido. Debe ser un número.", es_error=True)
            return None

    def solicitar_minutos_avanzar(self) -> int | None:
        # ... (igual que antes, no cambia) ...
        try:
            minutos_str = self.solicitar_input("Minutos a avanzar (ej: 5, 10, o dejar vacío para 5 por defecto): ")
            if not minutos_str:
                return None
            minutos = int(minutos_str)
            if minutos < 0:
                 self.mostrar_mensaje("Los minutos deben ser un número positivo.", es_error=True)
                 return -1
            return minutos
        except ValueError:
            self.mostrar_mensaje("Entrada inválida para minutos. Debe ser un número.", es_error=True)
            return -1

    def mostrar_lista_formateada(self, titulo: str, lineas: List[str]):
        """Muestra una lista de strings, útil para reglas."""
        print(f"\n{titulo}")
        # Usar solo para listas de texto plano como las reglas
        for linea in lineas:
            print(linea)


    # --- Métodos de Formateo de Tabla (Nuevos) ---

    def _format_queue_table(self, solicitudes: List[SolicitudAsignacion], hora_actual: datetime.datetime) -> List[str]:
        """Formatea los datos de la cola de espera como una tabla."""
        if not solicitudes:
            return ["La cola de espera está vacía."]

        # Definir cabeceras y extraer datos
        headers = ["ID", "Grupo", "Personas", "Reserva", "Tipo", "Llegada", "Esperando (mins)", "Prioridad Actual"]
        data = []
        for sol in solicitudes:
            cliente = sol.cliente
            tiempo_esperando_delta = hora_actual - cliente.hora_llegada
            tiempo_esperando_mins = tiempo_esperando_delta.total_seconds() / 60
            data.append([
                str(cliente.id_cliente),
                cliente.nombre_grupo,
                str(cliente.numero_personas),
                "Sí" if cliente.tiene_reserva else "No",
                cliente.tipo_cliente,
                cliente.hora_llegada.strftime('%H:%M:%S'),
                f"{tiempo_esperando_mins:.1f}",
                str(sol.prioridad_calculada) # Mostrar la tupla de prioridad de inserción
            ])

        # Calcular anchos de columna
        col_widths = [len(h) for h in headers]
        for row in data:
            for i, col in enumerate(row):
                 col_widths[i] = max(col_widths[i], len(col))

        # Crear líneas de separación
        separator = "+-" + "-+-".join("-" * w for w in col_widths) + "-+"

        # Crear cabecera de tabla
        header_row = "| " + " | ".join(headers[i].ljust(col_widths[i]) for i in range(len(headers))) + " |"

        # Crear filas de datos
        data_rows = []
        for row in data:
            data_rows.append("| " + " | ".join(row[i].ljust(col_widths[i]) for i in range(len(row))) + " |")

        # Construir tabla completa
        table_lines = [separator, header_row, separator] + data_rows + [separator]
        return table_lines


    def _format_tables_table(self, mesas: List[Mesa]) -> List[str]:
        """Formatea los datos de las mesas como una tabla."""
        # Definir cabeceras y extraer datos
        headers = ["ID Mesa", "Capacidad", "Estado", "Cliente Asignado (ID)", "Hora Asignación"]
        data = []
        for mesa in sorted(mesas, key=lambda m: m.id_mesa): # Asegurar orden por ID
            estado_str = "Disponible"
            cliente_info = ""
            hora_asignacion_str = ""
            if not mesa.disponible:
                estado_str = "Ocupada"
                cliente_info = str(mesa.cliente_asignado_id or 'N/A')
                if mesa.hora_asignacion:
                    hora_asignacion_str = mesa.hora_asignacion.strftime('%H:%M:%S')

            data.append([
                str(mesa.id_mesa),
                str(mesa.capacidad),
                estado_str,
                cliente_info,
                hora_asignacion_str
            ])

        # Calcular anchos de columna
        col_widths = [len(h) for h in headers]
        for row in data:
            for i, col in enumerate(row):
                 col_widths[i] = max(col_widths[i], len(str(col))) # Ensure col is string before len

        # Crear líneas de separación y filas
        separator = "+-" + "-+-".join("-" * w for w in col_widths) + "-+"
        header_row = "| " + " | ".join(headers[i].ljust(col_widths[i]) for i in range(len(headers))) + " |"
        data_rows = []
        for row in data:
            data_rows.append("| " + " | ".join(str(row[i]).ljust(col_widths[i]) for i in range(len(row))) + " |") # Ensure cell data is string

        table_lines = [separator, header_row, separator] + data_rows + [separator]
        return table_lines

    # --- Métodos de Visualización de Tablas (Modificados) ---

    def mostrar_cola_espera(self, solicitudes: List[SolicitudAsignacion], hora_actual: datetime.datetime):
        """Muestra la cola de espera en formato de tabla."""
        table_lines = self._format_queue_table(solicitudes, hora_actual)
        print("\n--- Cola de Espera Actual (Mayor Prioridad Primero) ---")
        for line in table_lines:
            print(line)

    def mostrar_estado_mesas(self, mesas: List[Mesa]):
        """Muestra el estado de las mesas en formato de tabla."""
        table_lines = self._format_tables_table(mesas)
        print("\n--- Estado Actual de las Mesas ---")
        for line in table_lines:
            print(line)

    # --- Método de Resumen (Nuevo) ---
    def mostrar_status_summary(self, summary_data: Dict[str, Any]):
        """Muestra un resumen rápido del estado del sistema."""
        print("\n--- Resumen del Sistema ---")
        print(f"  Hora actual de simulación: {summary_data['current_time'].strftime('%Y-%m-%d %H:%M:%S')}")
        print(f"  Grupos en cola de espera: {summary_data['queue_size']}")
        print(f"  Mesas disponibles: {summary_data['available_tables']} / {summary_data['total_tables']}")
        print("-------------------------")


    # --- Métodos de Arte ASCII (Nuevos) ---
    def mostrar_arte_ascii_bienvenida(self):
        """Muestra un mensaje de bienvenida con arte ASCII temático."""
        print("\n")
        print("     _______")
        print("    | O   O |")
        print("    |   v   |")
        print("    |  ---  |")
        print("    -------|--")
        print("   / _______ \\")
        print("  / / | []| \\ \\")
        print(" / /__|___|_\\ \\")
        print(" \\____________/")
        print("  _|_______|_")
        print(" /___________\\")
        print("\n--- Bienvenido al Sistema de Asignación de Mesas ---")
        print("     (Robot Recepcionista Inteligente)         ")
        print("\n")


    def mostrar_arte_ascii_salida(self):
        """Muestra un mensaje de despedida con arte ASCII temático."""
        print("\n")
        print("     _______")
        print("    | O   O |")
        print("    |   -   |")
        print("    |  \\_/  |")
        print("    -------|--")
        print("   / _______ \\")
        print("  / /       \\ \\")
        print(" / /_________\\ \\")
        print(" \\____________/")
        print("  _|_______|_")
        print(" /___________\\")
        print("\n--- Apagando. ¡Hasta la próxima! ---")
        print("\n")