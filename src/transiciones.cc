/**
 * @file transiciones.cc
 * @brief Implementacion de la clase Transiciones.
 */

#include "transiciones.h"

/**
 * @brief Anade una nueva transicion a la coleccion.
 *
 * El identificador es la posicion (1-indexada) que ocupara la transicion
 * en el vector interno, por lo que coincide con el orden de llegada.
 */
int Transiciones::agregar(const std::string& origen,
                          const std::string& lecturas,
                          const std::string& destino,
                          const std::string& escrituras,
                          const std::vector<Movimiento>& movimientos) {
  const int id = static_cast<int>(lista.size()) + 1;
  lista.push_back(Transicion{id, origen, lecturas, destino, escrituras, movimientos});
  return id;
}

/**
 * @brief Busca la transicion aplicable desde una configuracion.
 *
 * Busqueda lineal: el numero de transiciones de estas maquinas es
 * pequeno, y se mantiene la clase simple. Como la maquina es
 * determinista, a lo sumo habra una transicion aplicable.
 */
const Transicion* Transiciones::buscar(const std::string& estado,
                                       const std::string& lecturas) const {
  for (const Transicion& transicion : lista) {
    if (transicion.origen == estado && transicion.lecturas == lecturas) {
      return &transicion;
    }
  }
  return nullptr;
}

/**
 * @brief Numero total de transiciones almacenadas.
 */
std::size_t Transiciones::tamano() const {
  return lista.size();
}

/**
 * @brief Acceso de solo lectura a todas las transiciones.
 */
const std::vector<Transicion>& Transiciones::todas() const {
  return lista;
}