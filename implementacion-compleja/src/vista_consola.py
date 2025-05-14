# src/vista_consola.py

from typing import List, Dict, Any

class VistaConsola:
    """
    Responsable de toda la interacción de entrada/salida con el usuario en la consola.
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
        print("\n--- Herramienta de Gestión de Estructura de Directorios (Restaurante) ---")
        print("Escribe 'help' para ver los comandos disponibles.")

    def mostrar_ayuda_comandos(self, parsers: Dict[str, Any]): # El tipo de Any es argparse.ArgumentParser
        """Muestra la ayuda general o específica para un comando."""
        print("\nComandos disponibles:")
        print("  register_group <nombre> [--reserva] [--tipo <VIP|REGULAR|OCASIONAL>] --personas <num>")
        print("    Registra la llegada de un nuevo grupo de clientes.")
        print("  free_table <id_mesa>")
        print("    Marca una mesa como desocupada.")
        print("  assign_table")
        print("    Intenta asignar la siguiente mesa según prioridad (automático después de free_table/advance_time).")
        print("  advance_time [<minutos>]")
        print("    Avanza el tiempo de la simulación (por defecto 5 minutos).")
        print("  view_queue")
        print("    Muestra la cola de espera actual.")
        print("  view_tables")
        print("    Muestra el estado de todas las mesas.")
        print("  view_rules")
        print("    Muestra las reglas de prioridad.")
        print("  exit")
        print("    Sale de la aplicación.")
        # No se usa parsers para esta ayuda general por ahora, se puede integrar si se desea
        # print("\nEscribe 'help <comando>' para ver ayuda específica (funcionalidad futura).")


    def solicitar_datos_nuevo_grupo(self) -> Dict[str, Any] | None:
        """
        Solicita interactivamente los datos para registrar un nuevo grupo.
        Retorna un diccionario con los datos o None si el usuario cancela o hay error.
        """
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
                tipo_cliente_str = "REGULAR" # Valor por defecto
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
        """Solicita y valida un ID de mesa."""
        try:
            id_mesa_str = self.solicitar_input(prompt)
            return int(id_mesa_str)
        except ValueError:
            self.mostrar_mensaje("ID de mesa inválido. Debe ser un número.", es_error=True)
            return None

    def solicitar_minutos_avanzar(self) -> int | None:
        """Solicita y valida los minutos para avanzar el tiempo."""
        try:
            minutos_str = self.solicitar_input("Minutos a avanzar (ej: 5, 10, o dejar vacío para 5 por defecto): ")
            if not minutos_str: # Si está vacío, usar el valor por defecto del gestor
                return None # Indicar que se use el valor por defecto
            minutos = int(minutos_str)
            if minutos < 0:
                 self.mostrar_mensaje("Los minutos deben ser un número positivo.", es_error=True)
                 return -1 # Indicar error
            return minutos
        except ValueError:
            self.mostrar_mensaje("Entrada inválida para minutos. Debe ser un número.", es_error=True)
            return -1 # Indicar error


    def mostrar_lista_formateada(self, titulo: str, lineas: List[str]):
        """Muestra una lista de strings, útil para cola de espera o estado de mesas."""
        print(f"\n{titulo}")
        if not lineas or (len(lineas) == 1 and "vacía" in lineas[0].lower()):
            print(lineas[0] if lineas else "No hay información para mostrar.")
        else:
            for linea in lineas:
                print(linea)