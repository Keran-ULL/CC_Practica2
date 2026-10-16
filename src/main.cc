/**
 * @author Keran Miranda González
 * @file main.cc
 * @brief Punto de entrada del simulador de una Maquina de Turing.
 *
 * Orquesta el flujo completo del programa: lee las opciones de linea de
 * comandos, construye la Maquina de Turing a partir del fichero de
 * configuracion y ejecuta la maquina sobre cada cadena de entrada,
 * mostrando si se acepta y el contenido final de las cintas.
 *
 * Uso:
 *   main -config <f> [-in <f>]
 *
 * Asignatura: Complejidad Computacional. Curso 2026/27.
 */

#include <iostream>
#include <memory>
#include <string>

#include "argparser.h"
#include "errores.h"
#include "lectorcadenas.h"
#include "lectormaquina.h"
#include "maquinaturing.h"
#include "simulador.h"

/**
 * @brief Ejecuta el simulador sobre una cadena de entrada y muestra el
 *        resultado por pantalla.
 *
 * Si la cadena contiene simbolos fuera de Sigma, se informa del error y
 * se continua con las siguientes cadenas (no es un error fatal).
 *
 * @param simulador Simulador ya inicializado con la maquina a ejecutar.
 * @param entrada Cadena de entrada (puede ser vacia).
 */
void procesarEntrada(const Simulador& simulador, const std::string& entrada) {
  std::cout << "Entrada: \"" << entrada << "\"\n";
  try {
    const Resultado resultado = simulador.ejecutar(entrada);
    std::cout << "Resultado: " << (resultado.aceptada ? "ACEPTADA" : "NO ACEPTADA")
              << " (estado final: " << resultado.estadoFinal << ")\n";
    for (std::size_t i = 0; i < resultado.cintas.size(); ++i) {
      std::cout << "Cinta " << (i + 1) << ": " << resultado.cintas[i] << '\n';
    }
  } catch (const ErrorEntrada& error) {
    std::cerr << "Error: " << error.what() << '\n';
  }
  std::cout << '\n';
}

/**
 * @brief Funcion principal del programa.
 *
 * Se limita a coordinar las distintas clases. Cualquier error fatal
 * (argumentos invalidos, fichero de configuracion mal formado, maquina
 * que no cumple la definicion formal, ficheros inaccesibles...) se
 * propaga como una excepcion derivada de ErrorAutomata y se captura aqui
 * de forma centralizada.
 *
 * @param argc Numero de argumentos recibidos por linea de comandos.
 * @param argv Vector de argumentos recibidos por linea de comandos.
 * @return 0 si termina correctamente, 1 si se produce un error
 *         controlado, 2 ante un error no anticipado.
 */
int main(int argc, char* argv[]) {
  try {
    const Opciones opciones = ArgParser::parsear(argc, argv);

    const MaquinaTuring maquina = LectorMaquina::leer(opciones.ficheroConfig);
    const Simulador simulador(maquina);

    const std::unique_ptr<LectorCadenas> lector =
        opciones.ficheroEntrada.empty()
            ? std::make_unique<LectorCadenas>()
            : std::make_unique<LectorCadenas>(opciones.ficheroEntrada);

    std::string entrada;
    while (lector->siguienteCadena(entrada)) {
      procesarEntrada(simulador, entrada);
    }
  } catch (const ErrorAutomata& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  } catch (const std::exception& error) {
    std::cerr << "Error inesperado: " << error.what() << '\n';
    return 2;
  }

  return 0;
}