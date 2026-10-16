/**
 * @file Alfabeto.cc
 * @brief Implementacion de la clase Alfabeto.
 */

#include "alfabeto.h"

#include <algorithm>

/**
 * @brief Anade un simbolo al alfabeto.
 *
 * Se comprueba primero si el simbolo ya esta presente (busqueda lineal
 * con std::find) para no introducir duplicados; si no lo esta, se anade
 * al final del vector.
 */
void Alfabeto::agregar(char simbolo) {
  if (!pertenece(simbolo)) {
    simbolosAlfabeto.push_back(simbolo);
  }
}

/**
 * @brief Comprueba si un simbolo pertenece al alfabeto.
 *
 * Busqueda lineal: para el numero de simbolos habitual en estos
 * automatas el coste es despreciable frente a la simplicidad de usar
 * un vector.
 */
bool Alfabeto::pertenece(char simbolo) const {
  return std::find(simbolosAlfabeto.begin(), simbolosAlfabeto.end(), simbolo) != simbolosAlfabeto.end();
}

/**
 * @brief Numero de simbolos distintos del alfabeto.
 */
std::size_t Alfabeto::tamano() const {
  return simbolosAlfabeto.size();
}

/**
 * @brief Comprueba si el alfabeto no contiene ningun simbolo.
 */
bool Alfabeto::vacio() const {
  return simbolosAlfabeto.empty();
}

/**
 * @brief Acceso de solo lectura a los simbolos del alfabeto.
 */
const std::vector<char>& Alfabeto::simbolos() const {
  return simbolosAlfabeto;
}