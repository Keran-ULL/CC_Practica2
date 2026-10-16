#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

/**
 * @author Keran Miranda González
 * @file movimiento.h
 * @brief Declaracion del tipo Movimiento y de sus conversiones de caracter.
 *
 * Una transicion de la Maquina de Turing indica, para cada cinta, hacia
 * donde se mueve la cabeza de L/E despues de escribir: izquierda (L),
 * derecha (R) o no moverse (S). En el fichero de configuracion se
 * representan con esos tres caracteres.
 */

/**
 * @enum Movimiento
 * @brief Movimiento de la cabeza de lectura/escritura de una cinta.
 */
enum class Movimiento {
  Izquierda,  
  Derecha,    
  Quieto      
};

/**
 * @brief Comprueba si un caracter representa un movimiento valido.
 * @param caracter Caracter a comprobar.
 * @return true si es 'L', 'R' o 'S'.
 */
bool esMovimiento(char caracter);

/**
 * @brief Convierte un caracter del fichero de configuracion en su
 *        Movimiento.
 * @param caracter 'L', 'R' o 'S'.
 * @return El Movimiento correspondiente.
 * @throws ErrorFormato Si el caracter no es 'L', 'R' ni 'S'.
 */
Movimiento aMovimiento(char caracter);

/**
 * @brief Convierte un Movimiento en el caracter que lo representa.
 * @param movimiento Movimiento a convertir.
 * @return 'L', 'R' o 'S'.
 */
char aCaracter(Movimiento movimiento);

#endif  