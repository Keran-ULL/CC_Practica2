#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <string>
#include <vector>

#include "maquinaturing.h"

/**
 * @author Keran Miranda González
 * @file simulador.h
 * @brief Declaracion de la estructura Resultado y de la clase Simulador.
 *
 * Ejecuta una Maquina de Turing determinista multicinta sobre una cadena
 * de entrada. Al ser determinista no hay busqueda ni retroceso: es un
 * bucle que, en cada paso, busca la unica transicion aplicable y la
 * aplica. La ejecucion termina unicamente cuando no existe ninguna
 * transicion aplicable, y la cadena se acepta si el estado en el que se
 * detiene es de aceptacion.
 *
 * No se impone ningun limite de pasos: si la maquina no se detiene para
 * una entrada, la simulacion tampoco lo hara.
 */

/**
 * @struct Resultado
 * @brief Resultado de ejecutar la maquina sobre una cadena de entrada.
 */
struct Resultado {
  bool aceptada;                    
  std::string estadoFinal;          
  std::vector<std::string> cintas;  ///< Contenido de cada cinta, con la cabeza entre [ ].
};

/**
 * @class Simulador
 * @brief Motor de ejecucion de una MaquinaTuring.
 */
class Simulador {
public:
  /**
   * @brief Construye un simulador para una maquina concreta.
   * @param maquina Maquina a ejecutar. Debe seguir siendo valida durante
   *        toda la vida del Simulador (no se copia, se guarda una
   *        referencia).
   */
  explicit Simulador(const MaquinaTuring& maquina);

  /**
   * @brief Ejecuta la maquina sobre una cadena de entrada.
   *
   * La cadena se coloca en la primera cinta, con la cabeza en su primer
   * simbolo; el resto de cintas empiezan vacias (solo blancos).
   *
   * @param entrada Cadena de entrada (puede ser vacia).
   * @return El resultado: si se acepta, el estado en el que se detuvo y el
   *         contenido final de todas las cintas.
   * @throws ErrorEntrada Si la cadena contiene un simbolo que no
   *         pertenece al alfabeto de entrada Sigma.
   */
  Resultado ejecutar(const std::string& entrada) const;

private:
  const MaquinaTuring& maquina;
};

#endif  