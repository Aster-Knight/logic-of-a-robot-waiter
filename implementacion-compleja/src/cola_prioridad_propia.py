# src/cola_prioridad_propia.py

from typing import List, Optional # Para type hints más claros

from .solicitud_asignacion import SolicitudAsignacion # Importar SolicitudAsignacion

class ColaPrioridadPropia:
    """
    Implementación de una Cola de Prioridad utilizando un min-heap manual.
    Almacena objetos SolicitudAsignacion.
    Menor valor de prioridad significa mayor prioridad en la cola.
    """
    def __init__(self):
        # La lista _heap almacenará los objetos SolicitudAsignacion.
        # El índice 0 no se usa para facilitar los cálculos de padre/hijo (1-based indexing)
        # aunque podríamos adaptarlo a 0-based indexing.
        # Por simplicidad, usaremos 0-based indexing para la lista interna.
        self._heap: List[SolicitudAsignacion] = []

    def _sift_up(self, index: int):
        """
        Mueve un elemento hacia arriba en el heap para mantener la propiedad del min-heap
        después de una inserción. Compara con el padre.
        """
        parent_index = (index - 1) // 2
        # Mientras el nodo actual no sea la raíz y sea menor que su padre
        while index > 0 and self._heap[index] < self._heap[parent_index]:
            # Intercambiar el nodo actual con su padre
            self._heap[index], self._heap[parent_index] = self._heap[parent_index], self._heap[index]
            # Moverse al índice del padre para continuar el proceso
            index = parent_index
            parent_index = (index - 1) // 2

    def _sift_down(self, index: int):
        """
        Mueve un elemento hacia abajo en el heap para mantener la propiedad del min-heap
        después de una extracción de la raíz. Compara con los hijos.
        """
        n = len(self._heap)
        while True:
            left_child_index = 2 * index + 1
            right_child_index = 2 * index + 2
            smallest = index # Asumir que el nodo actual es el más pequeño

            # Verificar si el hijo izquierdo existe y es menor que el nodo actual (o el menor encontrado hasta ahora)
            if left_child_index < n and self._heap[left_child_index] < self._heap[smallest]:
                smallest = left_child_index

            # Verificar si el hijo derecho existe y es menor que el nodo actual (o el menor encontrado hasta ahora)
            if right_child_index < n and self._heap[right_child_index] < self._heap[smallest]:
                smallest = right_child_index

            # Si el nodo actual no es el más pequeño, intercambiar con el hijo más pequeño
            # y continuar el proceso hacia abajo
            if smallest != index:
                self._heap[index], self._heap[smallest] = self._heap[smallest], self._heap[index]
                index = smallest # Moverse al índice del hijo con el que se intercambió
            else:
                # Si el nodo actual es el más pequeño (o es una hoja), la propiedad del heap se mantiene
                break

    def add(self, solicitud: SolicitudAsignacion):
        """
        Añade una solicitud a la cola de prioridad.
        """
        self._heap.append(solicitud) # Añadir al final de la lista
        self._sift_up(len(self._heap) - 1) # Moverlo hacia arriba a su posición correcta

    def pop(self) -> Optional[SolicitudAsignacion]:
        """
        Remueve y retorna la solicitud con la mayor prioridad (menor valor de tupla de prioridad).
        Retorna None si la cola está vacía.
        """
        if self.is_empty():
            return None

        # El elemento de mayor prioridad está en la raíz (índice 0)
        highest_priority_item = self._heap[0]

        # Mover el último elemento a la raíz
        last_item = self._heap.pop() # Quita el último elemento

        if self._heap: # Si el heap no quedó vacío después del pop
            self._heap[0] = last_item
            self._sift_down(0) # Reordenar el heap desde la raíz

        return highest_priority_item

    def peek(self) -> Optional[SolicitudAsignacion]:
        """
        Retorna la solicitud con la mayor prioridad sin removerla.
        Retorna None si la cola está vacía.
        """
        if not self.is_empty():
            return self._heap[0]
        return None

    def is_empty(self) -> bool:
        """Verifica si la cola está vacía."""
        return len(self._heap) == 0

    def size(self) -> int:
        """Retorna el número de elementos en la cola."""
        return len(self._heap)

    def get_all_items(self) -> List[SolicitudAsignacion]:
        """
        Retorna todos los items en la cola, ordenados por prioridad.
        Esto es para visualización, no para operaciones de cola.
        Crea una copia ordenada del heap. No es eficiente para heaps grandes.
        Una forma más eficiente de iterar ordenadamente sería extraer elementos
        y luego reinsertarlos, o construir una lista ordenada.
        """
        # Crear una copia y ordenarla. Dado que SolicitudAsignacion implementa __lt__,
        # sorted() funcionará correctamente.
        return sorted(list(self._heap))

    def __str__(self):
        # Para una representación simple del heap (no necesariamente ordenada para visualización directa)
        return str([str(item.cliente) for item in self._heap])