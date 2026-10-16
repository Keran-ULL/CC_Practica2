#ifndef TRANSICION_H
#define TRANSICION_H

#include <string>
#include <vector>

#include "movimiento.h"

/**
 * @author Keran Miranda González
 * @file transicion.h
 * @brief Declaracion de la estructura Transicion.
 *
 * Representa una unica regla de la funcion de transicion de una Maquina
 * de Turing multicinta, con escritura y movimiento simultaneos:
 *
 *   delta(origen, lecturas) = (destino, escrituras, movimientos)
 *
 * Como cada simbolo de Gamma es un unico caracter, los simbolos leidos
 * (uno por cinta) se guardan juntos en un std::string, y lo mismo los
 * simbolos a escribir. Es un simple contenedor de datos, por lo que sus
 * campos son publicos.
 */

/**
 * @struct Transicion
 * @brief Una regla de transicion, identificada por el orden en que
 *        aparece en el fichero de configuracion.
 */
struct Transicion {
  int id;                              ///< Identificador (orden de aparicion en el fichero), empieza en 1.
  std::string origen;                  ///< Estado origen.
  std::string lecturas;                ///< Simbolo leido en cada cinta (lecturas[i] es el de la cinta i).
  std::string destino;                 ///< Estado destino.
  std::string escrituras;              ///< Simbolo a escribir en cada cinta.
  std::vector<Movimiento> movimientos; ///< Movimiento de la cabeza de cada cinta.
};

#endif  