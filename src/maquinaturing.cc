/**
 * @file maquinaturing.cc
 * @brief Implementacion de la clase MaquinaTuring.
 */

#include "maquinaturing.h"

#include <algorithm>
#include <string>
#include <utility>

#include "errores.h"

/**
 * @brief Construye y valida una Maquina de Turing.
 *
 * Se guardan todos los elementos recibidos y, a continuacion, se llama a
 * validar(). Si la validacion falla, la excepcion se propaga y el objeto
 * no llega a existir.
 */
MaquinaTuring::MaquinaTuring(std::vector<std::string> estados,
                             Alfabeto alfabetoEntrada,
                             Alfabeto alfabetoCinta,
                             std::string estadoInicial,
                             char blanco,
                             std::vector<std::string> estadosFinales,
                             int numCintas,
                             Transiciones transiciones)
    : estados(std::move(estados)),
      alfabetoEntrada(std::move(alfabetoEntrada)),
      alfabetoCinta(std::move(alfabetoCinta)),
      estadoInicial(std::move(estadoInicial)),
      blanco(blanco),
      estadosFinales(std::move(estadosFinales)),
      numCintas(numCintas),
      transiciones(std::move(transiciones)) {
  validar();
}

/**
 * @brief Comprueba las restricciones de la definicion formal.
 *
 * Se valida: Q no vacio; numero de cintas >= 1; s en Q; b en Gamma; b no
 * en Sigma; Sigma contenido en Gamma; F contenido en Q; y para cada
 * transicion, estados de Q, simbolos de Gamma, un elemento por cinta en
 * lecturas/escrituras/movimientos. Por ultimo se comprueba el
 * determinismo: no puede haber dos transiciones con el mismo origen y las
 * mismas lecturas.
 */
void MaquinaTuring::validar() const {
  if (estados.empty()) {
    throw ErrorValidacion("El conjunto de estados Q no puede estar vacio");
  }

  if (numCintas < 1) {
    throw ErrorValidacion("El numero de cintas debe ser al menos 1");
  }

  if (!esEstado(estadoInicial)) {
    throw ErrorValidacion("El estado inicial '" + estadoInicial + "' no pertenece a Q");
  }

  if (!alfabetoCinta.pertenece(blanco)) {
    throw ErrorValidacion(std::string("El simbolo blanco '") + blanco +
                          "' no pertenece a Gamma");
  }

  if (alfabetoEntrada.pertenece(blanco)) {
    throw ErrorValidacion(std::string("El simbolo blanco '") + blanco +
                          "' no puede pertenecer al alfabeto de entrada Sigma");
  }

  for (char simbolo : alfabetoEntrada.simbolos()) {
    if (!alfabetoCinta.pertenece(simbolo)) {
      throw ErrorValidacion(std::string("El simbolo '") + simbolo +
                            "' de Sigma no pertenece a Gamma");
    }
  }

  for (const std::string& final : estadosFinales) {
    if (!esEstado(final)) {
      throw ErrorValidacion("El estado final '" + final + "' no pertenece a Q");
    }
  }

  const std::size_t cintas = static_cast<std::size_t>(numCintas);
  for (const Transicion& transicion : transiciones.todas()) {
    const std::string nombre = "La transicion T" + std::to_string(transicion.id);

    if (!esEstado(transicion.origen)) {
      throw ErrorValidacion(nombre + " usa un estado origen ('" + transicion.origen +
                            "') que no pertenece a Q");
    }
    if (!esEstado(transicion.destino)) {
      throw ErrorValidacion(nombre + " usa un estado destino ('" + transicion.destino +
                            "') que no pertenece a Q");
    }
    if (transicion.lecturas.size() != cintas || transicion.escrituras.size() != cintas ||
        transicion.movimientos.size() != cintas) {
      throw ErrorValidacion(nombre + " no tiene un elemento por cada cinta (" +
                            std::to_string(numCintas) + ")");
    }
    for (char simbolo : transicion.lecturas) {
      if (!alfabetoCinta.pertenece(simbolo)) {
        throw ErrorValidacion(nombre + " lee un simbolo ('" + std::string(1, simbolo) +
                              "') que no pertenece a Gamma");
      }
    }
    for (char simbolo : transicion.escrituras) {
      if (!alfabetoCinta.pertenece(simbolo)) {
        throw ErrorValidacion(nombre + " escribe un simbolo ('" + std::string(1, simbolo) +
                              "') que no pertenece a Gamma");
      }
    }
  }

  const std::vector<Transicion>& todas = transiciones.todas();
  for (std::size_t i = 0; i < todas.size(); ++i) {
    for (std::size_t j = i + 1; j < todas.size(); ++j) {
      if (todas[i].origen == todas[j].origen && todas[i].lecturas == todas[j].lecturas) {
        throw ErrorValidacion("Las transiciones T" + std::to_string(todas[i].id) + " y T" +
                              std::to_string(todas[j].id) +
                              " tienen el mismo origen y las mismas lecturas: la maquina "
                              "no seria determinista");
      }
    }
  }
}

/**
 * @brief Consulta si un estado pertenece a la maquina.
 */
bool MaquinaTuring::esEstado(const std::string& estado) const {
  return std::find(estados.begin(), estados.end(), estado) != estados.end();
}

/**
 * @brief Consulta si un estado es de aceptacion.
 */
bool MaquinaTuring::esFinal(const std::string& estado) const {
  return std::find(estadosFinales.begin(), estadosFinales.end(), estado) != estadosFinales.end();
}

/**
 * @brief Conjunto de estados, Q.
 */
const std::vector<std::string>& MaquinaTuring::getEstados() const {
  return estados;
}

/**
 * @brief Alfabeto de entrada, Sigma.
 */
const Alfabeto& MaquinaTuring::getAlfabetoEntrada() const {
  return alfabetoEntrada;
}

/**
 * @brief Alfabeto de cinta, Gamma.
 */
const Alfabeto& MaquinaTuring::getAlfabetoCinta() const {
  return alfabetoCinta;
}

/**
 * @brief Estado inicial, s.
 */
const std::string& MaquinaTuring::getEstadoInicial() const {
  return estadoInicial;
}

/**
 * @brief Simbolo blanco, b.
 */
char MaquinaTuring::getBlanco() const {
  return blanco;
}

/**
 * @brief Conjunto de estados de aceptacion, F.
 */
const std::vector<std::string>& MaquinaTuring::getEstadosFinales() const {
  return estadosFinales;
}

/**
 * @brief Numero de cintas de la maquina.
 */
int MaquinaTuring::getNumCintas() const {
  return numCintas;
}

/**
 * @brief Funcion de transicion, delta.
 */
const Transiciones& MaquinaTuring::getTransiciones() const {
  return transiciones;
}