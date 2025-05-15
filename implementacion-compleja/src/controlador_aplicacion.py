# src/controlador_aplicacion.py

import shlex
import argparse
import datetime # Necesario para el caso base

from .gestor_restaurante import GestorRestaurante
from .vista_consola import VistaConsola
from .cliente import Cliente # Necesario para crear clientes en caso base
from .solicitud_asignacion import SolicitudAsignacion # Necesario para crear solicitudes en caso base
from .mesa import Mesa # Necesario para inicializar mesas en caso base


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

        # Parser para 'add' (anteriormente 'register_group')
        # Alias: ad, add-group
        parser_add = argparse.ArgumentParser(prog='add', add_help=False)
        parser_add.add_argument('nombre_grupo', type=str, help="Nombre del grupo de clientes.")
        parser_add.add_argument('--personas', '-p', type=int, required=True, help="Número de personas en el grupo (1-6).") # Alias -p
        parser_add.add_argument('--reserva', '-r', action='store_true', help="Indica si el grupo tiene reserva.") # Alias -r
        parser_add.add_argument('--tipo', '-t', type=str, default='REGULAR', choices=['VIP', 'REGULAR', 'OCASIONAL'], help="Tipo de cliente (VIP, REGULAR, OCASIONAL). Por defecto REGULAR.") # Alias -t
        self.parsers['add'] = parser_add
        self.parsers['ad'] = parser_add # Alias
        self.parsers['add-group'] = parser_add # Alias


        # Parser para 'free' (anteriormente 'free_table')
        # Alias: fr, free-table
        parser_free = argparse.ArgumentParser(prog='free', add_help=False)
        parser_free.add_argument('id_mesa', type=int, help="ID de la mesa a liberar.")
        self.parsers['free'] = parser_free
        self.parsers['fr'] = parser_free # Alias
        self.parsers['free-table'] = parser_free # Alias


        # Parser para 'time' (anteriormente 'advance_time')
        # Alias: ti, advance, advance-time
        parser_time = argparse.ArgumentParser(prog='time', add_help=False)
        parser_time.add_argument('minutos', type=int, nargs='?', default=None, help="Número de minutos a avanzar (por defecto 5 si no se especifica).")
        self.parsers['time'] = parser_time
        self.parsers['ti'] = parser_time # Alias
        self.parsers['advance'] = parser_time # Alias
        self.parsers['advance-time'] = parser_time # Alias


        # Parser para 'assign' (anteriormente 'assign_table')
        # Alias: as, assign-table
        # No requiere argumentos
        parser_assign = argparse.ArgumentParser(prog='assign', add_help=False)
        self.parsers['assign'] = parser_assign
        self.parsers['as'] = parser_assign # Alias
        self.parsers['assign-table'] = parser_assign # Alias


        # Parser para 'queue' (anteriormente 'view_queue')
        # Alias: qu, view-queue
        # No requiere argumentos
        parser_queue = argparse.ArgumentParser(prog='queue', add_help=False)
        self.parsers['queue'] = parser_queue
        self.parsers['qu'] = parser_queue # Alias
        self.parsers['view-queue'] = parser_queue # Alias


        # Parser para 'tables' (anteriormente 'view_tables')
        # Alias: ta, view-tables
        # No requiere argumentos
        parser_tables = argparse.ArgumentParser(prog='tables', add_help=False)
        self.parsers['tables'] = parser_tables
        self.parsers['ta'] = parser_tables # Alias
        self.parsers['view-tables'] = parser_tables # Alias


        # Parser para 'rules' (anteriormente 'view_rules')
        # Alias: ru, view-rules
        # No requiere argumentos
        parser_rules = argparse.ArgumentParser(prog='rules', add_help=False)
        self.parsers['rules'] = parser_rules
        self.parsers['ru'] = parser_rules # Alias
        self.parsers['view-rules'] = parser_rules # Alias

        # Parser para 'status' (nueva funcionalidad de resumen)
        # Alias: st
        # No requiere argumentos
        parser_status = argparse.ArgumentParser(prog='status', add_help=False)
        self.parsers['status'] = parser_status
        self.parsers['st'] = parser_status # Alias

        # Parser para 'base-case' (nueva funcionalidad de caso base)
        # Alias: bc, setup-base
        # No requiere argumentos
        parser_base_case = argparse.ArgumentParser(prog='base-case', add_help=False)
        self.parsers['base-case'] = parser_base_case
        self.parsers['bc'] = parser_base_case # Alias
        self.parsers['setup-base'] = parser_base_case # Alias


        # Parsers para 'help' y 'exit' (sin argumentos específicos que parsear con argparse)


    def iniciar_aplicacion(self):
        """Inicia el bucle principal de la interfaz de línea de comandos."""
        self.vista.mostrar_arte_ascii_bienvenida() # Mostrar arte ASCII al inicio
        self.vista.mostrar_menu_principal()

        while True:
            try:
                comando_completo = self.vista.solicitar_input("\nRestauranteCLI > ")
                if not comando_completo:
                    continue

                try:
                    partes_comando = shlex.split(comando_completo)
                    if not partes_comando:
                        continue
                    comando_principal = partes_comando[0].lower()
                    argumentos_comando = partes_comando[1:]
                except ValueError as e:
                    self.vista.mostrar_mensaje(f"Error al parsear comando: {e}", es_error=True)
                    continue

                # --- Manejo de comandos (usando nuevos nombres y alias) ---

                if comando_principal == 'exit':
                    self.vista.mostrar_mensaje("Saliendo de la aplicación...")
                    self.vista.mostrar_arte_ascii_salida() # Mostrar arte ASCII al salir
                    break

                elif comando_principal == 'help':
                    self.vista.mostrar_ayuda_comandos() # La vista manejará qué mostrar

                elif comando_principal in self.parsers: # Usamos self.parsers para validar comandos con parsers
                    comando_a_ejecutar = comando_principal # Usamos el nombre ingresado (puede ser alias)
                    parser_a_usar = self.parsers[comando_a_ejecutar] # Obtenemos el parser asociado

                    try:
                        # Parsear los argumentos usando el parser correcto
                        parsed_args = parser_a_usar.parse_args(argumentos_comando)

                        # --- Ejecutar la lógica del comando ---
                        if comando_a_ejecutar in ['add', 'ad', 'add-group']:
                             # Si se llama sin argumentos, usar modo interactivo
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
                                 # args ya fueron parseados por argparse arriba, solo usar parsed_args
                                resultado = self.gestor_restaurante.registrar_llegada(
                                    parsed_args.nombre_grupo,
                                    parsed_args.reserva, # argparse ya manejó --reserva
                                    parsed_args.tipo.upper(), # argparse ya manejó --tipo
                                    parsed_args.personas # argparse ya manejó --personas
                                )
                                self.vista.mostrar_mensaje(resultado)

                        elif comando_a_ejecutar in ['free', 'fr', 'free-table']:
                             # Si se llama sin argumentos, usar modo interactivo
                             if not argumentos_comando:
                                 id_mesa = self.vista.solicitar_id_mesa("ID de la mesa a liberar: ")
                                 if id_mesa is not None:
                                     resultado = self.gestor_restaurante.liberar_mesa(id_mesa)
                                     self.vista.mostrar_mensaje(resultado)
                             else: # Usar parsed_args
                                 resultado = self.gestor_restaurante.liberar_mesa(parsed_args.id_mesa)
                                 self.vista.mostrar_mensaje(resultado)


                        elif comando_a_ejecutar in ['time', 'ti', 'advance', 'advance-time']:
                             # Si se llama sin argumentos (parsed_args.minutos is None), usar el defecto del gestor
                             minutos_a_avanzar = parsed_args.minutos if parsed_args.minutos is not None else self.gestor_restaurante.minutos_por_operacion
                             if minutos_a_avanzar < 0:
                                self.vista.mostrar_mensaje("Los minutos a avanzar deben ser no negativos.", es_error=True)
                                continue

                             self.gestor_restaurante.avanzar_tiempo_simulacion(minutos_a_avanzar)
                             # La asignación automática se intenta dentro de avanzar_tiempo_simulacion


                        elif comando_a_ejecutar in ['assign', 'as', 'assign-table']:
                            # Este comando no tiene argumentos
                            resultado = self.gestor_restaurante.intentar_asignar_mesa()
                            self.vista.mostrar_mensaje(resultado)


                        elif comando_a_ejecutar in ['queue', 'qu', 'view-queue']:
                             # Este comando no tiene argumentos
                             cola_formateada = self.gestor_restaurante.get_cola_espera_formateada()
                             # La vista ahora manejará el formato de tabla
                             self.vista.mostrar_lista_formateada("--- Cola de Espera Actual ---", cola_formateada)

                        elif comando_a_ejecutar in ['tables', 'ta', 'view-tables']:
                            # Este comando no tiene argumentos
                            mesas_formateadas = self.gestor_restaurante.get_estado_mesas_formateado()
                             # La vista ahora manejará el formato de tabla
                            self.vista.mostrar_lista_formateada("--- Estado Actual de las Mesas ---", mesas_formateadas)


                        elif comando_a_ejecutar in ['rules', 'ru', 'view-rules']:
                            # Este comando no tiene argumentos
                            reglas_formateadas = self.gestor_restaurante.get_reglas_prioridad_formateadas()
                            self.vista.mostrar_lista_formateada("--- Reglas de Prioridad ---", reglas_formateadas)

                        elif comando_a_ejecutar in ['status', 'st']:
                            # Este comando no tiene argumentos
                             summary_data = self.gestor_restaurante.get_status_summary()
                             self.vista.mostrar_status_summary(summary_data)

                        elif comando_a_ejecutar in ['base-case', 'bc', 'setup-base']:
                            # Este comando no tiene argumentos
                            self.gestor_restaurante.setup_base_case()
                            self.vista.mostrar_mensaje("Caso base configurado. Hora de inicio simulada: 18:00:00") # Notificar al usuario


                    except SystemExit: # argparse lanza SystemExit en --help o error
                        # El mensaje de ayuda o error ya fue mostrado por argparse
                        pass
                    except Exception as e:
                         self.vista.mostrar_mensaje(f"Error en comando '{comando_principal}': {e}", es_error=True)

                else: # Comando no encontrado en self.parsers
                     self.vista.mostrar_mensaje(f"Comando desconocido: '{comando_principal}'. Escribe 'help'.", es_error=True)


            except EOFError:
                self.vista.mostrar_mensaje("\nSaliendo de la aplicación (EOF)...")
                self.vista.mostrar_arte_ascii_salida() # Mostrar arte ASCII al salir
                break
            except KeyboardInterrupt:
                self.vista.mostrar_mensaje("\nOperación cancelada por el usuario. Saliendo...")
                self.vista.mostrar_arte_ascii_salida() # Mostrar arte ASCII al salir
                break
            except Exception as e:
                self.vista.mostrar_mensaje(f"Ocurrió un error inesperado: {e}", es_error=True)              