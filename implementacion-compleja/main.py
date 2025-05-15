# main.py

import sys
import argparse
import shlex
from pathlib import Path

# Importar la clase ControladorAplicacion desde el paquete src
from src.controlador_aplicacion import ControladorAplicacion


# Función de ayuda para parsear argumentos de búsqueda (key=value) (sin cambios)
def parse_search_criteria(criteria_list):
    """Convierte una lista de strings 'key=value' en un diccionario."""
    criteria = {}
    for item in criteria_list:
        if '=' in item:
            key, value = item.split('=', 1)
            criteria[key.lower()] = value
        else:
            print(f"Advertencia: Criterio de búsqueda ignorado por formato incorrecto: '{item}'. Usar 'key=value'.")
    return criteria


def main():
    """Función principal para la interfaz de consola."""

    controlador = ControladorAplicacion() # Instanciar el controlador
    controlador.iniciar_aplicacion() # Iniciar el bucle principal

if __name__ == "__main__":
    main()