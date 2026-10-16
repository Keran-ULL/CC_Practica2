#ifndef TRANSICIONES_H
#define TRANSICIONES_H

#include <string>
#include <vector>
#include "movimiento.h"
#include "transicion.h"

/**
 * @author Keran Miranda González
 * @file transiciones.h
 * @brief Declaracion de la clase Transiciones.
 *
 * Agrupa todas las reglas de la funcion de transicion de la Maquina de
 * Turing y permite buscar la regla aplicable desde una configuracion
 * (estado actual y simbolos leidos en cada cinta).
 */

/**
 * @class Transiciones
 * @brief Coleccion de reglas de transicion con busqueda por estado y
 *        simbolos leidos.
 */
class Transiciones {
public:
  /**
   * @brief Construye una coleccion de transiciones vacia.
   */
  Transiciones() = default;

  /**
   * @brief Añade una nueva transicion a la coleccion.
   *
   * @param origen Estado origen.
   * @param lecturas Simbolo leido en cada cinta.
   * @param destino Estado destino.
   * @param escrituras Simbolo a escribir en cada cinta.
   * @param movimientos Movimiento de la cabeza de cada cinta.
   * @return Identificador asignado a la transicion anadida.
   */
  int agregar(const std::string& origen,
              const std::string& lecturas,
              const std::string& destino,
              const std::string& escrituras,
              const std::vector<Movimiento>& movimientos);

  /**
   * @brief Busca la transicion aplicable desde una configuracion.
   *
   * @param estado Estado actual de la maquina.
   * @param lecturas Simbolo bajo la cabeza de cada cinta.
   * @return Puntero a la primera transicion cuyo origen y simbolos
   *         leidos coinciden, o nullptr si no hay ninguna (en cuyo caso
   *         la maquina se detiene). El puntero deja de ser valido si
   *         despues se llama a agregar().
   */
  const Transicion* buscar(const std::string& estado,
                           const std::string& lecturas) const;

  /**
   * @brief Numero total de transiciones almacenadas.
   * @return Cantidad de transiciones.
   */
  std::size_t tamano() const;

  /**
   * @brief Acceso de solo lectura a todas las transiciones, en el orden
   *        en que se anadieron. Permite a MaquinaTuring validarlas.
   * @return Referencia constante al vector interno de transiciones.
   */
  const std::vector<Transicion>& todas() const;

private:
  std::vector<Transicion> lista;  
};

#endif  