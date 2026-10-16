/**
 * @author Keran Miranda González
 * @file LectorCadenas.h
 * @brief Declaracion de la clase LectorCadenas.
 *
 * Proporciona las cadenas de entrada que el Simulador debe comprobar,
 * leyendolas una por una, bien por teclado (std::cin) o bien de un
 * fichero de texto (una cadena por linea), segun con que constructor se
 * cree. 
 */
#ifndef LECTOR_CADENAS_H
#define LECTOR_CADENAS_H

#include <fstream>
#include <iostream>
#include <string>

/**
 * @class LectorCadenas
 * @brief Fuente de cadenas de entrada, por teclado o por fichero.
 */
class LectorCadenas {
public:
  /**
   * @brief Construye un lector que lee por teclado (std::cin).
   */
  LectorCadenas();

  /**
   * @brief Construye un lector que lee de un fichero de texto.
   * @param rutaFichero Ruta del fichero con una cadena por linea.
   * @throws ErrorFichero Si el fichero no se puede abrir para lectura.
   */
  explicit LectorCadenas(const std::string& rutaFichero);

  /**
   * @brief Obtiene la siguiente cadena de entrada disponible.
   *
   * @param cadena Donde se guarda la cadena leida.
   * @return true si se ha leido una cadena, false si no quedan mas
   *         (fin de fichero o fin de la entrada por teclado).
   */
  bool siguienteCadena(std::string& cadena);

private:
  std::ifstream ficheroEntrada;  
  std::istream* entrada;        
  bool desdeTeclado;             
};

#endif  