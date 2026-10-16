/**
 * @file movimiento.cc
 * @brief Implementacion de las conversiones del tipo Movimiento.
 */

#include "movimiento.h"
#include <string>
#include "errores.h"

/**
 * @brief Comprueba si un caracter representa un movimiento valido.
 */
bool esMovimiento(char caracter) {
  return caracter == 'L' || caracter == 'R' || caracter == 'S';
}

/**
 * @brief Convierte un caracter del fichero de configuracion en su
 *        Movimiento.
 */
Movimiento aMovimiento(char caracter) {
  switch (caracter) {
    case 'L':
      return Movimiento::Izquierda;
    case 'R':
      return Movimiento::Derecha;
    case 'S':
      return Movimiento::Quieto;
    default:
      throw ErrorFormato(std::string("'") + caracter +
                         "' no es un movimiento valido (debe ser L, R o S)");
  }
}

/**
 * @brief Convierte un Movimiento en el caracter que lo representa.
 */
char aCaracter(Movimiento movimiento) {
  switch (movimiento) {
    case Movimiento::Izquierda:
      return 'L';
    case Movimiento::Derecha:
      return 'R';
    case Movimiento::Quieto:
      return 'S';
  }
  return '?';
}