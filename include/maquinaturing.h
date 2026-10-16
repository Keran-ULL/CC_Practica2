#ifndef MAQUINATURING_H
#define MAQUINATURING_H

#include <string>
#include <vector>
#include "alfabeto.h"
#include "transiciones.h"

/**
 * @author Keran Miranda González
 * @file maquinaturing.h
 * @brief Declaracion de la clase MaquinaTuring.
 *
 * Modela la definicion formal de una Maquina de Turing determinista y
 * multicinta, M = (Q, Sigma, Gamma, s, b, F, delta), junto con el numero
 * de cintas. Variantes elegidas: escritura y movimiento simultaneos,
 * movimientos L, R y S, y cinta infinita en ambas direcciones.
 */

/**
 * @class MaquinaTuring
 * @brief Definicion formal, ya validada, de una Maquina de Turing
 *        determinista multicinta.
 */
class MaquinaTuring {
public:
  /**
   * @brief Construye y valida una Maquina de Turing.
   *
   * @param estados Conjunto de estados, Q.
   * @param alfabetoEntrada Alfabeto de entrada, Sigma. No puede contener
   *        el simbolo blanco y debe ser subconjunto de alfabetoCinta.
   * @param alfabetoCinta Alfabeto de cinta, Gamma. Debe contener el
   *        simbolo blanco.
   * @param estadoInicial Estado inicial, s. Debe pertenecer a estados.
   * @param blanco Simbolo blanco, b. Debe pertenecer a alfabetoCinta.
   * @param estadosFinales Conjunto de estados de aceptacion, F. Debe ser
   *        subconjunto de estados.
   * @param numCintas Numero de cintas (al menos 1).
   * @param transiciones Funcion de transicion. Cada regla debe usar
   *        estados de Q, simbolos de Gamma y tener un elemento por cinta;
   *        ademas no puede haber dos reglas con el mismo origen y las
   *        mismas lecturas (determinismo).
   * @throws ErrorValidacion Si no se cumple alguna restriccion.
   */
  MaquinaTuring(std::vector<std::string> estados,
                Alfabeto alfabetoEntrada,
                Alfabeto alfabetoCinta,
                std::string estadoInicial,
                char blanco,
                std::vector<std::string> estadosFinales,
                int numCintas,
                Transiciones transiciones);

  /**
   * @brief Consulta si un estado pertenece a la maquina.
   * @param estado Nombre del estado a comprobar.
   * @return true si estado pertenece a Q.
   */
  bool esEstado(const std::string& estado) const;

  /**
   * @brief Consulta si un estado es de aceptacion.
   * @param estado Nombre del estado a comprobar.
   * @return true si estado pertenece a F.
   */
  bool esFinal(const std::string& estado) const;

  /**
   * @brief Conjunto de estados, Q.
   * @return Referencia constante al vector de estados.
   */
  const std::vector<std::string>& getEstados() const;

  /**
   * @brief Alfabeto de entrada, Sigma.
   * @return Referencia constante al alfabeto de entrada.
   */
  const Alfabeto& getAlfabetoEntrada() const;

  /**
   * @brief Alfabeto de cinta, Gamma.
   * @return Referencia constante al alfabeto de cinta.
   */
  const Alfabeto& getAlfabetoCinta() const;

  /**
   * @brief Estado inicial, s.
   * @return Nombre del estado inicial.
   */
  const std::string& getEstadoInicial() const;

  /**
   * @brief Simbolo blanco, b.
   * @return El simbolo blanco.
   */
  char getBlanco() const;

  /**
   * @brief Conjunto de estados de aceptacion, F.
   * @return Referencia constante al vector de estados finales.
   */
  const std::vector<std::string>& getEstadosFinales() const;

  /**
   * @brief Numero de cintas de la maquina.
   * @return Numero de cintas.
   */
  int getNumCintas() const;

  /**
   * @brief Funcion de transicion, delta.
   * @return Referencia constante a la coleccion de transiciones.
   */
  const Transiciones& getTransiciones() const;

private:
  /**
   * @brief Comprueba que los elementos recibidos cumplen las
   *        restricciones de la definicion formal de una Maquina de
   *        Turing, lanzando una excepcion si no es asi.
   * @throws ErrorValidacion Si alguna restriccion no se cumple.
   */
  void validar() const;

  std::vector<std::string> estados;        
  Alfabeto alfabetoEntrada;                 
  Alfabeto alfabetoCinta;                   
  std::string estadoInicial;                
  char blanco;                              
  std::vector<std::string> estadosFinales;  
  int numCintas;                            
  Transiciones transiciones;                
};

#endif  