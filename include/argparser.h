#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>

/**
 * @file argparser.h
 * @brief Declaracion de la estructura Opciones y la clase ArgParser.
 *
 * Interpreta las opciones de linea de comandos del programa:
 *
 *   mt_simulator -config <f> [-in <f>]
 */

/**
 * @struct Opciones
 * @brief Opciones de linea de comandos ya interpretadas.
 */
struct Opciones {
  std::string ficheroConfig;   
  std::string ficheroEntrada;  
};

/**
 * @class ArgParser
 * @brief Interprete de las opciones de linea de comandos.
 *
 * No mantiene estado propio: todos sus metodos son estaticos.
 */
class ArgParser {
public:
  /**
   * @brief Interpreta las opciones de linea de comandos.
   *
   * @param argc Numero de argumentos, tal y como los recibe main().
   * @param argv Vector de argumentos, tal y como los recibe main().
   * @return Las opciones ya interpretadas y validadas.
   * @throws ErrorArgumentos Si falta -config, si a una opcion le falta su
   *         valor, o si aparece una opcion no reconocida.
   */
  static Opciones parsear(int argc, char* argv[]);

private:
  /**
   * @brief Obtiene el valor asociado a una opcion y avanza el indice
   *        para dejarlo apuntando a ese valor.
   *
   * @param argc Numero de argumentos.
   * @param argv Vector de argumentos.
   * @param indice Indice de la opcion; se incrementa en 1.
   * @param opcion Nombre de la opcion, para el mensaje de error.
   * @return El valor asociado a la opcion.
   * @throws ErrorArgumentos Si no queda ningun argumento tras la opcion.
   */
  static std::string obtenerValor(int argc, char* argv[], int& indice,
                                  const std::string& opcion);
};

#endif  