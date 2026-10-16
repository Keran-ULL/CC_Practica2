/**
 * @file lectormaquina.cc
 * @brief Implementacion de la clase LectorMaquina.
 */

#include "lectormaquina.h"
#include <sstream>
#include "errores.h"
#include "movimiento.h"
#include "transiciones.h"

/**
 * @brief Lee y parsea el fichero de configuracion indicado.
 *
 * El fichero sigue un orden fijo: Q, Sigma, Gamma, estado inicial,
 * simbolo blanco, F, numero de cintas, y despues una transicion por
 * linea hasta el final del fichero.
 */
MaquinaTuring LectorMaquina::leer(const std::string& ruta) {
  std::ifstream fichero(ruta);
  if (!fichero.is_open()) {
    throw ErrorFichero("No se ha podido abrir el fichero de configuracion: " + ruta);
  }

  const std::vector<std::string> estados =
      leerPalabras(fichero, "el conjunto de estados Q");

  Alfabeto alfabetoEntrada;
  for (const std::string& palabra : leerPalabras(fichero, "el alfabeto de entrada Sigma")) {
    alfabetoEntrada.agregar(aSimbolo(palabra, "el alfabeto de entrada Sigma"));
  }

  Alfabeto alfabetoCinta;
  for (const std::string& palabra : leerPalabras(fichero, "el alfabeto de cinta Gamma")) {
    alfabetoCinta.agregar(aSimbolo(palabra, "el alfabeto de cinta Gamma"));
  }

  const std::vector<std::string> palabrasInicial = leerPalabras(fichero, "el estado inicial");
  if (palabrasInicial.size() != 1) {
    throw ErrorFormato("La linea del estado inicial debe contener un unico estado");
  }
  const std::string estadoInicial = palabrasInicial[0];

  const std::vector<std::string> palabrasBlanco = leerPalabras(fichero, "el simbolo blanco");
  if (palabrasBlanco.size() != 1) {
    throw ErrorFormato("La linea del simbolo blanco debe contener un unico simbolo");
  }
  const char blanco = aSimbolo(palabrasBlanco[0], "el simbolo blanco");

  const std::vector<std::string> estadosFinales =
      leerPalabras(fichero, "el conjunto de estados finales F");

  const std::vector<std::string> palabrasCintas = leerPalabras(fichero, "el numero de cintas");
  if (palabrasCintas.size() != 1) {
    throw ErrorFormato("La linea del numero de cintas debe contener un unico numero");
  }
  const int numCintas = aNumeroCintas(palabrasCintas[0]);
  const std::size_t n = static_cast<std::size_t>(numCintas);

  // Cada transicion: origen, n lecturas, destino, n escrituras, n movimientos.
  Transiciones transiciones;
  std::string linea;
  while (leerLineaSignificativa(fichero, linea)) {
    const std::vector<std::string> palabras = dividirEnPalabras(linea);
    const std::size_t esperados = 2 + 3 * n;
    if (palabras.size() != esperados) {
      throw ErrorFormato("La transicion '" + linea + "' debe tener " +
                         std::to_string(esperados) +
                         " campos (origen, lecturas, destino, escrituras y movimientos "
                         "para " + std::to_string(numCintas) + " cinta(s)) y tiene " +
                         std::to_string(palabras.size()));
    }

    std::string lecturas;
    std::string escrituras;
    std::vector<Movimiento> movimientos;
    for (std::size_t i = 0; i < n; ++i) {
      lecturas += aSimbolo(palabras[1 + i], "un simbolo leido de una transicion");
      escrituras += aSimbolo(palabras[2 + n + i], "un simbolo escrito de una transicion");
      movimientos.push_back(
          aMovimiento(aSimbolo(palabras[2 + 2 * n + i], "un movimiento de una transicion")));
    }

    transiciones.agregar(palabras[0], lecturas, palabras[1 + n], escrituras, movimientos);
  }

  return MaquinaTuring(estados, alfabetoEntrada, alfabetoCinta, estadoInicial, blanco,
                       estadosFinales, numCintas, transiciones);
}

/**
 * @brief Lee la siguiente linea significativa del fichero.
 *
 * Descarta todo lo que aparezca a partir de un '#' (comentario), recorta
 * espacios en blanco al principio y al final, y salta las lineas que
 * queden vacias tras ese proceso.
 */
bool LectorMaquina::leerLineaSignificativa(std::ifstream& fichero, std::string& linea) {
  std::string bruta;
  while (std::getline(fichero, bruta)) {
    const std::size_t posComentario = bruta.find('#');
    const std::string sinComentario =
        (posComentario == std::string::npos) ? bruta : bruta.substr(0, posComentario);

    const std::size_t inicio = sinComentario.find_first_not_of(" \t\r");
    if (inicio == std::string::npos) {
      continue;
    }
    const std::size_t fin = sinComentario.find_last_not_of(" \t\r");
    linea = sinComentario.substr(inicio, fin - inicio + 1);
    return true;
  }
  return false;
}

/**
 * @brief Lee la siguiente linea significativa y la divide en palabras.
 */
std::vector<std::string> LectorMaquina::leerPalabras(std::ifstream& fichero,
                                                     const std::string& descripcion) {
  std::string linea;
  if (!leerLineaSignificativa(fichero, linea)) {
    throw ErrorFormato("Falta la linea con " + descripcion);
  }
  return dividirEnPalabras(linea);
}

/**
 * @brief Divide una linea en palabras separadas por espacios en blanco.
 */
std::vector<std::string> LectorMaquina::dividirEnPalabras(const std::string& linea) {
  std::vector<std::string> palabras;
  std::istringstream flujo(linea);
  std::string palabra;
  while (flujo >> palabra) {
    palabras.push_back(palabra);
  }
  return palabras;
}

/**
 * @brief Comprueba que una palabra es un unico simbolo (un caracter).
 */
char LectorMaquina::aSimbolo(const std::string& palabra, const std::string& contexto) {
  if (palabra.size() != 1) {
    throw ErrorFormato("'" + palabra + "' no es un simbolo valido de un unico caracter en " +
                       contexto);
  }
  return palabra[0];
}

/**
 * @brief Convierte una palabra en el numero de cintas.
 *
 * Se limita a 4 digitos para evitar desbordamientos al convertir.
 */
int LectorMaquina::aNumeroCintas(const std::string& palabra) {
  if (palabra.empty() || palabra.size() > 4 ||
      palabra.find_first_not_of("0123456789") != std::string::npos) {
    throw ErrorFormato("'" + palabra + "' no es un numero de cintas valido");
  }
  const int numero = std::stoi(palabra);
  if (numero < 1) {
    throw ErrorFormato("El numero de cintas debe ser al menos 1");
  }
  return numero;
}