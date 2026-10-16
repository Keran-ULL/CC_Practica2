/**
 * @file simulador.cc
 * @brief Implementacion de la clase Simulador.
 */

#include "simulador.h"

#include "cinta.h"
#include "errores.h"

/**
 * @brief Construye un simulador para una maquina concreta.
 */
Simulador::Simulador(const MaquinaTuring& maquina) : maquina(maquina) {}

/**
 * @brief Ejecuta la maquina sobre una cadena de entrada.
 *
 * Primero se comprueba que todos los simbolos de la entrada pertenecen a
 * Sigma. Despues se crean las cintas (la primera con la entrada, las
 * demas vacias) y se repite: leer los simbolos bajo las cabezas, buscar
 * la transicion aplicable y, si existe, escribir y mover en todas las
 * cintas a la vez y cambiar de estado. Cuando no hay transicion, la
 * maquina se detiene.
 */
Resultado Simulador::ejecutar(const std::string& entrada) const {
  for (char simbolo : entrada) {
    if (!maquina.getAlfabetoEntrada().pertenece(simbolo)) {
      throw ErrorEntrada(std::string("El simbolo '") + simbolo +
                         "' de la entrada no pertenece al alfabeto de entrada Sigma");
    }
  }

  const char blanco = maquina.getBlanco();
  const int numCintas = maquina.getNumCintas();

  std::vector<Cinta> cintas;
  cintas.emplace_back(blanco, entrada);
  for (int i = 1; i < numCintas; ++i) {
    cintas.emplace_back(blanco);
  }

  std::string estado = maquina.getEstadoInicial();
  while (true) {
    std::string lecturas;
    for (const Cinta& cinta : cintas) {
      lecturas += cinta.leer();
    }

    const Transicion* transicion = maquina.getTransiciones().buscar(estado, lecturas);
    if (transicion == nullptr) {
      break;
    }

    for (std::size_t i = 0; i < cintas.size(); ++i) {
      cintas[i].escribir(transicion->escrituras[i]);
      cintas[i].mover(transicion->movimientos[i]);
    }
    estado = transicion->destino;
  }

  Resultado resultado;
  resultado.aceptada = maquina.esFinal(estado);
  resultado.estadoFinal = estado;
  for (const Cinta& cinta : cintas) {
    resultado.cintas.push_back(cinta.aTexto());
  }
  return resultado;
}