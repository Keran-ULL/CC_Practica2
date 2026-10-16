/**
 * @file argparser.cc
 * @brief Implementacion de la clase ArgParser.
 */

#include "argparser.h"

#include "errores.h"

/**
 * @brief Interpreta las opciones de linea de comandos.
 *
 * Recorre argv de izquierda a derecha reconociendo -config e -in;
 * cualquier otra cosa es un error. Al final se comprueba que -config
 * haya aparecido.
 */
Opciones ArgParser::parsear(int argc, char* argv[]) {
  Opciones opciones;

  int i = 1;
  while (i < argc) {
    const std::string argumento = argv[i];

    if (argumento == "-config") {
      opciones.ficheroConfig = obtenerValor(argc, argv, i, "-config");
    } else if (argumento == "-in") {
      opciones.ficheroEntrada = obtenerValor(argc, argv, i, "-in");
    } else {
      throw ErrorArgumentos("Opcion no reconocida: '" + argumento + "'");
    }

    ++i;
  }

  if (opciones.ficheroConfig.empty()) {
    throw ErrorArgumentos("Falta la opcion obligatoria -config <f>");
  }

  return opciones;
}

/**
 * @brief Obtiene el valor asociado a una opcion y avanza el indice.
 */
std::string ArgParser::obtenerValor(int argc, char* argv[], int& indice,
                                    const std::string& opcion) {
  if (indice + 1 >= argc) {
    throw ErrorArgumentos("A la opcion '" + opcion + "' le falta el valor");
  }
  ++indice;
  return argv[indice];
}