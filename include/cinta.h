#ifndef CINTA_H
#define CINTA_H

#include <string>
#include <vector>
#include "movimiento.h"

/**
 * @author Keran Miranda González
 * @file cinta.h
 * @brief Declaracion de la clase Cinta.
 *
 * Cinta de una Maquina de Turing, infinita en ambas direcciones: si la
 * cabeza de L/E se sale por cualquiera de los dos extremos, se anade una
 * celda con el simbolo blanco y se sigue.
 *
 * Internamente es un std::vector<char>. El contenido inicial se coloca a
 * partir de la posicion 1, y la posicion 0 se deja con un blanco, de
 * modo que un movimiento a la izquierda desde el primer simbolo (caso
 * muy habitual) no obliga a ampliar la cinta. Aun asi la cinta puede
 * seguir creciendo por la izquierda cuando hace falta: en ese caso se
 * inserta un blanco al principio y la cabeza se queda en la posicion 0,
 * que pasa a ser la celda nueva.
 */

/**
 * @class Cinta
 * @brief Cinta infinita en ambas direcciones con una cabeza de L/E.
 */
class Cinta {
public:
  /**
   * @brief Construye una cinta con un contenido inicial.
   *
   * @param blanco Simbolo blanco de la maquina.
   * @param contenido Simbolos iniciales de la cinta (por defecto, vacio).
   */
  explicit Cinta(char blanco, const std::string& contenido = "");

  /**
   * @brief Lee el simbolo situado bajo la cabeza.
   * @return El simbolo de la celda actual.
   */
  char leer() const;

  /**
   * @brief Escribe un simbolo en la celda situada bajo la cabeza.
   * @param simbolo Simbolo a escribir.
   */
  void escribir(char simbolo);

  /**
   * @brief Mueve la cabeza una celda a la izquierda, a la derecha o la
   *        deja quieta, ampliando la cinta con un blanco si se sale por
   *        un extremo.
   * @param movimiento Movimiento a realizar.
   */
  void mover(Movimiento movimiento);

  /**
   * @brief Representacion textual de la cinta con la posicion de la
   *        cabeza entre corchetes (p. ej. "ab[c]d").
   *
   * Se omiten los blancos de los extremos, salvo la celda de la cabeza,
   * que siempre se muestra.
   *
   * @return El contenido de la cinta como texto.
   */
  std::string aTexto() const;

private:
  std::vector<char> celdas;  ///< Contenido de la cinta; la posicion 0 es siempre una celda valida.
  std::size_t cabeza;        ///< Indice en celdas de la celda bajo la cabeza.
  char blanco;               ///< Simbolo blanco con el que se rellenan las celdas nuevas.
};

#endif  