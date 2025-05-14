# src/controlador_aplicacion.py

import shlex
import argparse # Se usará argparse para parsear argumentos de los comandos ingresados

from .gestor_restaurante import GestorRestaurante
from .vista_consola import VistaConsola

class ControladorAplicacion:
    """
    Actúa como intermediario entre la Vista (consola) y el Modelo (GestorRestaurante).
    Maneja la lógica de la CLI personalizada.
    """
    def __init__(self):
        self.gestor_restaurante = GestorRestaurante()
        self.vista = VistaConsola()
        self._inicializar_parsers_comandos()

    def _inicializar_parsers_comandos(self):
        """Define los parsers de argparse para cada comando."""
        self.parsers = {}

        # Parser para 'register_group'
        parser_register = argparse.ArgumentParser(prog='register_group', add_help=False)
        parser_register.add_argument('nombre_grupo', type=str, help="Nombre del grupo de clientes.")
        parser_register.add_argument('--personas', type=int, required=True, help="Número de personas en el grupo (1-6).")
        parser_register.add_argument('--reserva', action='store_true', help="Indica si el grupo tiene reserva.")
        parser_register.add_argument('--tipo', type=str, default='REGULAR', choices=['VIP', 'REGULAR', 'OCASIONAL'], help="Tipo de cliente (VIP, REGULAR, OCASIONAL). Por defecto REGULAR.")
        self.parsers['register_group'] = parser_register

        # Parser para 'free_table'
        parser_free = argparse.ArgumentParser(prog='free_table', add_help=False)
        parser_free.add_argument('id_mesa', type=int, help="ID de la mesa a liberar.")
        self.parsers['free_table'] = parser_free

        # Parser para 'advance_time'
        parser_advance = argparse.ArgumentParser(prog='advance_time', add_help=False)
        parser_advance.add_argument('minutos', type=int, nargs='?', default=None, help="Número de minutos a avanzar (por defecto 5 si no se especifica).") # nargs='?' hace que sea opcional
        self.parsers['advance_time'] = parser_advance

        # No se necesitan parsers para comandos sin argumentos como view_queue, view_tables, assign_table, view_rules, exit

    def iniciar_aplicacion(self):
        """Inicia el bucle principal de la interfaz de línea de comandos."""
        self.vista.mostrar_menu_principal()

        while True:
            try:
                comando_completo = self.vista.solicitar_input("\nRestauranteCLI > ")
                if not comando_completo:
                    continue

                try:
                    partes_comando = shlex.split(comando_completo) # Maneja comillas para nombres con espacios
                    if not partes_comando:
                        continue
                    comando_principal = partes_comando[0].lower()
                    argumentos_comando = partes_comando[1:]
                except ValueError as e: # Error de shlex.split (ej. comillas sin cerrar)
                    self.vista.mostrar_mensaje(f"Error al parsear comando: {e}", es_error=True)
                    continue


                if comando_principal == 'exit':
                    self.vista.mostrar_mensaje("Saliendo de la aplicación...")
                    break

                elif comando_principal == 'help':
                    self.vista.mostrar_ayuda_comandos(self.parsers) # Pasamos parsers por si queremos ayuda específica más adelante

                elif comando_principal == 'register_group':
                    # Si se llama 'register_group' sin argumentos, usar modo interactivo
                    if not argumentos_comando:
                        datos_grupo = self.vista.solicitar_datos_nuevo_grupo()
                        if datos_grupo:
                            resultado = self.gestor_restaurante.registrar_llegada(
                                datos_grupo["nombre_grupo"],
                                datos_grupo["tiene_reserva"],
                                datos_grupo["tipo_cliente"],
                                datos_grupo["numero_personas"]
                            )
                            self.vista.mostrar_mensaje(resultado)
                    else: # Intentar parsear desde la línea de comando
                        try:
                            parsed_args = self.parsers['register_group'].parse_args(argumentos_comando)
                            resultado = self.gestor_restaurante.registrar_llegada(
                                parsed_args.nombre_grupo,
                                parsed_args.reserva,
                                parsed_args.tipo.upper(),
                                parsed_args.personas
                            )
                            self.vista.mostrar_mensaje(resultado)
                        except SystemExit: # argparse lanza SystemExit en --help o error
                            pass # argparse ya imprimió el mensaje
                        except Exception as e: # Otros errores de argparse o lógicos
                             self.vista.mostrar_mensaje(f"Error en comando '{comando_principal}': {e}", es_error=True)


                elif comando_principal == 'free_table':
                    if not argumentos_comando: # Si se llama 'free_table' sin ID
                        id_mesa = self.vista.solicitar_id_mesa("ID de la mesa a liberar: ")
                        if id_mesa is not None:
                             resultado = self.gestor_restaurante.liberar_mesa(id_mesa)
                             self.vista.mostrar_mensaje(resultado)
                    else: # Intentar parsear desde la línea de comando
                        try:
                            parsed_args = self.parsers['free_table'].parse_args(argumentos_comando)
                            resultado = self.gestor_restaurante.liberar_mesa(parsed_args.id_mesa)
                            self.vista.mostrar_mensaje(resultado)
                        except SystemExit:
                            pass
                        except Exception as e:
                             self.vista.mostrar_mensaje(f"Error en comando '{comando_principal}': {e}", es_error=True)

                elif comando_principal == 'assign_table':
                    resultado = self.gestor_restaurante.intentar_asignar_mesa()
                    self.vista.mostrar_mensaje(resultado)

                elif comando_principal == 'advance_time':
                    minutos_a_avanzar = self.gestor_restaurante.minutos_por_operacion # Valor por defecto
                    if argumentos_comando: # Si se proporcionan minutos
                        try:
                            parsed_args = self.parsers['advance_time'].parse_args(argumentos_comando)
                            if parsed_args.minutos is not None: # Si el usuario especificó un valor
                                minutos_a_avanzar = parsed_args.minutos
                                if minutos_a_avanzar < 0:
                                     self.vista.mostrar_mensaje("Los minutos a avanzar deben ser no negativos.", es_error=True)
                                     continue
                        except SystemExit:
                            continue # argparse ya manejó el error o la ayuda
                        except Exception as e:
                             self.vista.mostrar_mensaje(f"Error en argumento de '{comando_principal}': {e}", es_error=True)
                             continue
                    else: # Si se llama 'advance_time' sin argumentos, usar el valor por defecto o pedir
                        # Aquí podríamos pedir interactivamente si queremos, o simplemente usar el defecto.
                        # Por ahora, si no hay args, usa el defecto del gestor (ej. 5 min).
                        pass # Se usará minutos_a_avanzar = self.gestor_restaurante.minutos_por_operacion

                    self.gestor_restaurante.avanzar_tiempo_simulacion(minutos_a_avanzar)


                elif comando_principal == 'view_queue':
                    cola_formateada = self.gestor_restaurante.get_cola_espera_formateada()
                    self.vista.mostrar_lista_formateada("--- Cola de Espera Actual ---", cola_formateada)

                elif comando_principal == 'view_tables':
                    mesas_formateadas = self.gestor_restaurante.get_estado_mesas_formateado()
                    self.vista.mostrar_lista_formateada("--- Estado Actual de las Mesas ---", mesas_formateadas)

                elif comando_principal == 'view_rules':
                    reglas_formateadas = self.gestor_restaurante.get_reglas_prioridad_formateadas()
                    self.vista.mostrar_lista_formateada("--- Reglas de Prioridad ---", reglas_formateadas)


                else:
                    self.vista.mostrar_mensaje(f"Comando desconocido: '{comando_principal}'. Escribe 'help'.", es_error=True)

            except EOFError: # Ctrl+D
                self.vista.mostrar_mensaje("\nSaliendo de la aplicación (EOF)...")
                break
            except KeyboardInterrupt: # Ctrl+C
                self.vista.mostrar_mensaje("\nOperación cancelada por el usuario. Saliendo...")
                break
            except Exception as e:
                self.vista.mostrar_mensaje(f"Ocurrió un error inesperado: {e}", es_error=True)
                