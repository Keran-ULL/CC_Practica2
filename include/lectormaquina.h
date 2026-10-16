#ifndef LECTORMAQUINA_H
#define LECTORMAQUINA_H

#include <fstream>
#include <string>
#include <vector>

#include "maquinaturing.h"

/**
 * @file lectormaquina.h
 * @brief Declaracion de la clase LectorMaquina.
 *
 * Lee el fichero de texto con la definicion de una Maquina de Turing y
 * construye el objeto MaquinaTuring correspondiente. Toda la logica de
 * parseo vive aqui; la validacion semantica de la definicion formal la
 * realiza el propio MaquinaTuring en su constructor.
 *
 * Formato del fichero (los comentarios empiezan por '#'):
 *
 *   q1 q2 q3 ...           conjunto Q
 *   a1 a2 ...              conjunto Sigma
 *   A1 A2 ...              conjunto Gamma
 *   q1                     estado inicial
 *   b                      simbolo blanco
 *   q2 q3                  conjunto F
 *   n                      numero de cintas
 *   origen lec1..lecN destino esc1..escN mov1..movN     una por linea
 *
 * Con una cinta, las transiciones tienen el formato "q a q' a' m".
 */

/**
 * @class LectorMaquina
 * @brief Lector del fichero de configuracion de una Maquina de Turing.
 *
 * No mantiene estado propio: todos sus metodos son estaticos.
 */
class LectorMaquina {
public:
  /**
   * @brief Lee y parsea el fichero de configuracion indicado.
   *
   * @param ruta Ruta del fichero de texto con la definicion de la maquina.
   * @return La MaquinaTuring construida y validada.
   * @throws ErrorFichero Si el fichero no se puede abrir.
   * @throws ErrorFormato Si alguna linea no respeta el formato esperado.
   * @throws ErrorValidacion Si la maquina no cumple su definicion formal.
   */
  static MaquinaTuring leer(const std::string& ruta);

private:
  /**
   * @brief Lee la siguiente linea significativa del fichero, ignorando
   *        lineas en blanco y descartando los comentarios (desde '#').
   *
   * @param fichero Flujo de entrada del que se lee.
   * @param linea Donde se guarda la linea leida, sin comentario y sin
   *        espacios al principio ni al final.
   * @return true si se ha leido una linea, false si se llego al final.
   */
  static bool leerLineaSignificativa(std::ifstream& fichero, std::string& linea);

  /**
   * @brief Lee la siguiente linea significativa y la divide en palabras.
   *
   * @param fichero Flujo de entrada del que se lee.
   * @param descripcion Que se esperaba leer, para el mensaje de error.
   * @return Las palabras de la linea.
   * @throws ErrorFormato Si no quedan lineas en el fichero.
   */
  static std::vector<std::string> leerPalabras(std::ifstream& fichero,
                                               const std::string& descripcion);

  /**
   * @brief Divide una linea en las palabras separadas por espacios.
   * @param linea Linea a dividir.
   * @return Las palabras, en el orden en que aparecen.
   */
  static std::vector<std::string> dividirEnPalabras(const std::string& linea);

  /**
   * @brief Comprueba que una palabra es un unico simbolo (un caracter).
   *
   * Al contrario que en el automata con pila, el punto no tiene un
   * significado especial: es un simbolo valido (de hecho es el blanco en
   * los ejemplos).
   *
   * @param palabra Palabra a comprobar.
   * @param contexto Donde aparece la palabra, para el mensaje de error.
   * @return El caracter que representa la palabra.
   * @throws ErrorFormato Si la palabra no tiene exactamente un caracter.
   */
  static char aSimbolo(const std::string& palabra, const std::string& contexto);

  /**
   * @brief Convierte una palabra en el numero de cintas.
   * @param palabra Palabra con el numero (solo digitos).
   * @return El numero de cintas, al menos 1.
   * @throws ErrorFormato Si no es un numero entero positivo.
   */
  static int aNumeroCintas(const std::string& palabra);
};

#endif  