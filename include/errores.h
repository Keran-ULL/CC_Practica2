#ifndef ERRORES_H
#define ERRORES_H

#include <stdexcept>
#include <string>

/**
 * @file Errores.h
 * @brief Jerarquia de excepciones del simulador de automata con pila.
 *
 * Se agrupan todas en un unico fichero de cabecera para mantener el
 * manejo de errores compacto: cada excepcion es una clase minima que
 * solo anade un mensaje descriptivo sobre std::runtime_error, asi que no
 * hace falta un .cc aparte.
 *
 * Todas las clases del proyecto deberian lanzar la subclase de
 * ErrorAutomata que mejor describa el fallo (en vez de excepciones
 * estandar genericas como std::invalid_argument). main() captura
 * ErrorAutomata de forma centralizada y muestra error.what().
 */

/**
 * @class ErrorAutomata
 * @brief Excepcion base de la que heredan todos los errores propios del
 *        simulador.
 */
class ErrorAutomata : public std::runtime_error {
public:
    /**
     * @brief Construye el error con un mensaje descriptivo.
     * @param mensaje Descripcion del error, pensada para mostrarse tal
     *        cual al usuario.
     */
    explicit ErrorAutomata(const std::string& mensaje) : std::runtime_error(mensaje) {}
};

/**
 * @class ErrorArgumentos
 * @brief Error en las opciones recibidas por linea de comandos (falta
 *        una opcion obligatoria, un valor no reconocido, etc.).
 */
class ErrorArgumentos : public ErrorAutomata {
public:
    explicit ErrorArgumentos(const std::string& mensaje) : ErrorAutomata(mensaje) {}
};

/**
 * @class ErrorFichero
 * @brief Error de acceso a un fichero: no existe, no se puede abrir o
 *        no se puede escribir en el.
 */
class ErrorFichero : public ErrorAutomata {
public:
    explicit ErrorFichero(const std::string& mensaje) : ErrorAutomata(mensaje) {}
};

/**
 * @class ErrorFormato
 * @brief Error de formato al parsear el fichero de configuracion: una
 *        linea con una sintaxis inesperada, un simbolo que deberia ser
 *        un unico caracter y no lo es, un campo que falta, etc.
 */
class ErrorFormato : public ErrorAutomata {
public:
    explicit ErrorFormato(const std::string& mensaje) : ErrorAutomata(mensaje) {}
};

/**
 * @class ErrorValidacion
 * @brief Error de validacion semantica: el fichero tiene un formato
 *        correcto pero el automata resultante no cumple las
 *        restricciones de su definicion formal (p. ej. el estado
 *        inicial no pertenece a Q).
 */
class ErrorValidacion : public ErrorAutomata {
public:
    explicit ErrorValidacion(const std::string& mensaje) : ErrorAutomata(mensaje) {}
};

/**
 * @class ErrorEntrada
 * @brief Error en una cadena de entrada a procesar: contiene un simbolo
 *        que no pertenece al alfabeto de entrada Sigma. A diferencia del
 *        resto, no impide seguir con las demas cadenas.
 */
class ErrorEntrada : public ErrorAutomata {
public:
  explicit ErrorEntrada(const std::string& mensaje) : ErrorAutomata(mensaje) {}
};

#endif  