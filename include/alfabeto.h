/**
 * @author Keran Miranda González
 * @file alfabeto.h
 * @brief Declaracion de la clase Alfabeto.
 *
 * Representa un conjunto de simbolos formados por un unico caracter,
 * empleado tanto para el alfabeto de entrada (Sigma) como para el
 * alfabeto de pila (Gamma) del automata.
 */

#ifndef ALFABETO_H
#define ALFABETO_H

#include <vector>

/**
 * @class Alfabeto
 * @brief Conjunto de simbolos (caracteres) con comprobacion de pertenencia.
 */
class Alfabeto {
public:
    /**
     * @brief Construye un alfabeto vacio.
     */
    Alfabeto() = default;

    /**
     * @brief Anade un simbolo al alfabeto.
     *
     * Si el simbolo ya estaba presente no se vuelve a anadir, para que
     * el alfabeto siga comportandose como un conjunto (sin duplicados)
     * aunque internamente se use un vector.
     *
     * @param simbolo Caracter a anadir.
     */
    void agregar(char simbolo);

    /**
     * @brief Comprueba si un simbolo pertenece al alfabeto.
     * @param simbolo Caracter a comprobar.
     * @return true si el simbolo pertenece al alfabeto, false en caso
     *         contrario.
     */
    bool pertenece(char simbolo) const;

    /**
     * @brief Numero de simbolos distintos del alfabeto.
     * @return Tamano del alfabeto.
     */
    std::size_t tamano() const;

    /**
     * @brief Comprueba si el alfabeto no contiene ningun simbolo.
     * @return true si esta vacio.
     */
    bool vacio() const;

    /**
     * @brief Acceso de solo lectura a los simbolos, util para recorrerlos
     *        (p. ej. al mostrar el alfabeto o al validar el automata),
     *        en el mismo orden en que se anadieron.
     * @return Referencia constante al vector interno de simbolos.
     */
    const std::vector<char>& simbolos() const;

private:
    std::vector<char> simbolosAlfabeto;  
};

#endif  // ALFABETO_H