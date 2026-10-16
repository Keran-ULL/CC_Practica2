/**
 * @file cinta.cc
 * @brief Implementacion de la clase Cinta.
 */

#include "cinta.h"
#include <algorithm>

/**
 * @brief Construye una cinta con un contenido inicial.
 *
 * Se coloca un blanco en la posicion 0 y el contenido a partir de la
 * posicion 1. Si el contenido esta vacio, se anade un unico blanco para
 * que la cabeza (posicion 1) apunte siempre a una celda existente.
 */
Cinta::Cinta(char blanco, const std::string& contenido)
    : cabeza(1), blanco(blanco) {
  celdas.push_back(blanco);
  if (contenido.empty()) {
    celdas.push_back(blanco);
  } else {
    celdas.insert(celdas.end(), contenido.begin(), contenido.end());
  }
}

/**
 * @brief Lee el simbolo situado bajo la cabeza.
 */
char Cinta::leer() const {
  return celdas[cabeza];
}

/**
 * @brief Escribe un simbolo en la celda situada bajo la cabeza.
 */
void Cinta::escribir(char simbolo) {
  celdas[cabeza] = simbolo;
}

/**
 * @brief Mueve la cabeza, ampliando la cinta si se sale por un extremo.
 *
 * Por la derecha basta con anadir un blanco al final. Por la izquierda
 * se inserta un blanco al principio del vector; como todas las celdas se
 * desplazan una posicion, la cabeza se queda en el indice 0, que ahora
 * es la celda nueva.
 */
void Cinta::mover(Movimiento movimiento) {
  switch (movimiento) {
    case Movimiento::Derecha:
      ++cabeza;
      if (cabeza == celdas.size()) {
        celdas.push_back(blanco);
      }
      break;
    case Movimiento::Izquierda:
      if (cabeza == 0) {
        celdas.insert(celdas.begin(), blanco);
      } else {
        --cabeza;
      }
      break;
    case Movimiento::Quieto:
      break;
  }
}

/**
 * @brief Representacion textual de la cinta con la cabeza entre
 *        corchetes.
 *
 * Se busca la primera y la ultima celda no blancas, se amplia el rango
 * para incluir siempre la celda de la cabeza, y se recorre ese rango
 * marcando la celda de la cabeza.
 */
std::string Cinta::aTexto() const {
  std::size_t inicio = cabeza;
  std::size_t fin = cabeza;

  for (std::size_t i = 0; i < celdas.size(); ++i) {
    if (celdas[i] != blanco) {
      inicio = std::min(inicio, i);
      fin = std::max(fin, i);
    }
  }

  std::string texto;
  for (std::size_t i = inicio; i <= fin; ++i) {
    if (i == cabeza) {
      texto += '[';
      texto += celdas[i];
      texto += ']';
    } else {
      texto += celdas[i];
    }
  }
  return texto;
}