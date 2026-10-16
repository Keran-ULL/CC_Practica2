#!/usr/bin/env bash
#
# test.sh - Bateria de pruebas automaticas para el simulador de MT.
#
# Compila el proyecto y comprueba: manejo de errores (argumentos,
# ficheros, formato y validacion de la maquina), comportamiento con una
# MT de 1 cinta y otra de 2 cintas, cadenas con simbolos fuera de Sigma,
# lectura de cadenas por fichero (-in) y las dos MT de la practica
# (data/M1.txt y data/M2.txt).
#
# Uso: ./test/test.sh   (se puede ejecutar desde cualquier directorio)

set -uo pipefail

# --- Localizar la raiz del proyecto y el ejecutable ------------------
PROYECTO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROYECTO_DIR"

# Ajusta esta ruta si tu Makefile genera el ejecutable en otro sitio.
EJECUTABLE="build/main"

MT1="data/M1.txt"
MT2="data/M2.txt"

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

PASADOS=0
FALLADOS=0

# --- Funciones auxiliares --------------------------------------------

# Anota el resultado de un test y lo muestra por pantalla.
# $1: nombre del test.  $2: 0 si ha pasado, cualquier otro valor si ha fallado.
reportar() {
  local nombre="$1"
  local resultado="$2"
  if [ "$resultado" -eq 0 ]; then
    echo "  [OK]   $nombre"
    PASADOS=$((PASADOS + 1))
  else
    echo "  [FAIL] $nombre"
    FALLADOS=$((FALLADOS + 1))
  fi
}

# Ejecuta el simulador con los argumentos dados y comprueba que TERMINA
# CON ERROR CONTROLADO (codigo de salida 1). Un 2 (error inesperado) o un
# fallo de segmentacion tambien harian fallar el test.
# $1: nombre del test. Resto: argumentos para el ejecutable.
esperar_error() {
  local nombre="$1"
  shift
  "$EJECUTABLE" "$@" </dev/null >/dev/null 2>&1
  local codigo=$?
  if [ "$codigo" -eq 1 ]; then
    reportar "$nombre" 0
  else
    reportar "$nombre" 1
    echo "         codigo de salida obtenido: $codigo (esperado: 1)"
  fi
}

# Comprueba si la maquina acepta o rechaza una cadena.
# $1: nombre del test. $2: fichero de configuracion. $3: cadena.
# $4: "ACEPTADA" o "NO ACEPTADA".
comprobar_resultado() {
  local nombre="$1" config="$2" cadena="$3" esperado="$4"
  local salida
  salida=$(printf '%s\n' "$cadena" | "$EJECUTABLE" -config "$config" 2>&1)
  if echo "$salida" | grep -q -- "^Resultado: $esperado "; then
    reportar "$nombre" 0
  else
    reportar "$nombre" 1
    echo "         esperado: $esperado / salida obtenida: $salida"
  fi
}

# Comprueba el contenido exacto de una cinta tras ejecutar la maquina.
# $1: nombre del test. $2: fichero de configuracion. $3: cadena.
# $4: numero de cinta (1, 2...). $5: contenido esperado (con la cabeza
# entre corchetes, tal y como lo imprime el simulador).
comprobar_cinta() {
  local nombre="$1" config="$2" cadena="$3" numero="$4" esperado="$5"
  local salida
  salida=$(printf '%s\n' "$cadena" | "$EJECUTABLE" -config "$config" 2>&1)
  if echo "$salida" | grep -qxF -- "Cinta $numero: $esperado"; then
    reportar "$nombre" 0
  else
    reportar "$nombre" 1
    echo "         esperado: 'Cinta $numero: $esperado' / salida obtenida: $salida"
  fi
}

# --- Compilacion -------------------------------------------------------

echo "=== Compilando el proyecto ==="
if ! make >"$TMP_DIR/make.log" 2>&1; then
  echo "La compilacion ha fallado. Salida de 'make':"
  cat "$TMP_DIR/make.log"
  exit 1
fi
if [ ! -x "$EJECUTABLE" ]; then
  echo "No se encuentra el ejecutable en '$EJECUTABLE'."
  echo "Ajusta la variable EJECUTABLE al principio de este script."
  exit 1
fi
for fichero in "$MT1" "$MT2"; do
  if [ ! -f "$fichero" ]; then
    echo "No se encuentra el fichero de la MT: $fichero"
    exit 1
  fi
done
echo "OK: $EJECUTABLE"
echo

# --- Ficheros de configuracion usados en los tests ---------------------

# MT de 1 cinta valida (Ejemplo 1 del enunciado): cadenas binarias con un
# numero impar de 0.
cat > "$TMP_DIR/impar0.txt" << 'CFG'
# Cadenas binarias con un numero impar de 0
q0 q1 q2
0 1
0 1 .
q0
.
q2
1
q0 0 q1 0 R
q0 1 q0 1 R
q1 0 q0 0 R
q1 1 q1 1 R
q1 . q2 . R
CFG

# MT de 2 cintas valida: copia la cinta 1 en la cinta 2.
cat > "$TMP_DIR/copia2.txt" << 'CFG'
# 2 cintas: copia la entrada de la cinta 1 a la cinta 2
q0 qf
a b
a b .
q0
.
qf
2
q0 a . q0 a a R R
q0 b . q0 b b R R
q0 . . qf . . S S
CFG

# MT que se desplaza a la izquierda de la entrada (comprueba la cinta
# infinita por ambos lados): escribe una 'x' dos celdas a la izquierda.
cat > "$TMP_DIR/izquierda.txt" << 'CFG'
q0 q1 q2 qf
a
a x .
q0
.
qf
1
q0 a q1 a L
q1 . q2 x L
q2 . qf x S
CFG

# Cabecera comun para las configuraciones erroneas: se parte de una
# configuracion correcta y se rompe una sola cosa en cada fichero.
crear_config() {
  # $1: nombre del fichero. Resto de lineas por stdin.
  cat > "$TMP_DIR/$1"
}

crear_config pocas_lineas.txt << 'CFG'
q0 q1
a b
CFG

crear_config estado_inicial_invalido.txt << 'CFG'
q0 q1
a b
a b .
q5
.
q1
1
q0 a q1 a R
CFG

crear_config final_invalido.txt << 'CFG'
q0 q1
a b
a b .
q0
.
q9
1
q0 a q1 a R
CFG

crear_config simbolo_multicaracter.txt << 'CFG'
q0 q1
a bb
a bb .
q0
.
q1
1
q0 a q1 a R
CFG

crear_config blanco_en_sigma.txt << 'CFG'
q0 q1
a .
a .
q0
.
q1
1
q0 a q1 a R
CFG

crear_config blanco_fuera_de_gamma.txt << 'CFG'
q0 q1
a b
a b
q0
.
q1
1
q0 a q1 a R
CFG

crear_config sigma_fuera_de_gamma.txt << 'CFG'
q0 q1
a b
a .
q0
.
q1
1
q0 a q1 a R
CFG

crear_config cintas_cero.txt << 'CFG'
q0 q1
a b
a b .
q0
.
q1
0
CFG

crear_config cintas_no_numero.txt << 'CFG'
q0 q1
a b
a b .
q0
.
q1
dos
CFG

crear_config transicion_pocos_campos.txt << 'CFG'
q0 q1
a b
a b .
q0
.
q1
2
q0 a q1 a R
CFG

crear_config transicion_estado_desconocido.txt << 'CFG'
q0 q1
a b
a b .
q0
.
q1
1
q0 a q7 a R
CFG

crear_config transicion_simbolo_desconocido.txt << 'CFG'
q0 q1
a b
a b .
q0
.
q1
1
q0 z q1 a R
CFG

crear_config movimiento_invalido.txt << 'CFG'
q0 q1
a b
a b .
q0
.
q1
1
q0 a q1 a X
CFG

crear_config no_determinista.txt << 'CFG'
q0 q1
a b
a b .
q0
.
q1
1
q0 a q1 a R
q0 a q0 b R
CFG

# --- 1. Manejo de errores ----------------------------------------------

echo "=== 1. Manejo de errores ==="
esperar_error "Sin argumentos (falta -config)"
esperar_error "Opcion no reconocida"                  -config "$TMP_DIR/impar0.txt" -foo bar
esperar_error "-config sin valor"                     -config
esperar_error "-in sin valor"                         -config "$TMP_DIR/impar0.txt" -in
esperar_error "Fichero de configuracion inexistente"  -config "$TMP_DIR/no_existe.txt"
esperar_error "Fichero de entrada (-in) inexistente"  -config "$TMP_DIR/impar0.txt" -in "$TMP_DIR/no_existe.txt"
esperar_error "Config con lineas insuficientes"       -config "$TMP_DIR/pocas_lineas.txt"
esperar_error "Estado inicial no pertenece a Q"       -config "$TMP_DIR/estado_inicial_invalido.txt"
esperar_error "Estado final no pertenece a Q"         -config "$TMP_DIR/final_invalido.txt"
esperar_error "Simbolo de mas de un caracter"         -config "$TMP_DIR/simbolo_multicaracter.txt"
esperar_error "Blanco incluido en Sigma"              -config "$TMP_DIR/blanco_en_sigma.txt"
esperar_error "Blanco fuera de Gamma"                 -config "$TMP_DIR/blanco_fuera_de_gamma.txt"
esperar_error "Sigma no incluido en Gamma"            -config "$TMP_DIR/sigma_fuera_de_gamma.txt"
esperar_error "Numero de cintas igual a 0"            -config "$TMP_DIR/cintas_cero.txt"
esperar_error "Numero de cintas no numerico"          -config "$TMP_DIR/cintas_no_numero.txt"
esperar_error "Transicion con campos de menos"        -config "$TMP_DIR/transicion_pocos_campos.txt"
esperar_error "Transicion con estado desconocido"     -config "$TMP_DIR/transicion_estado_desconocido.txt"
esperar_error "Transicion con simbolo desconocido"    -config "$TMP_DIR/transicion_simbolo_desconocido.txt"
esperar_error "Transicion con movimiento invalido"    -config "$TMP_DIR/movimiento_invalido.txt"
esperar_error "Maquina no determinista"               -config "$TMP_DIR/no_determinista.txt"
echo

# --- 2. MT de 1 cinta (cadenas binarias con numero impar de 0) ---------

echo "=== 2. MT de 1 cinta (numero impar de 0) ==="
comprobar_resultado "'0' aceptada"            "$TMP_DIR/impar0.txt" "0"     "ACEPTADA"
comprobar_resultado "'1011' aceptada"         "$TMP_DIR/impar0.txt" "1011"  "ACEPTADA"
comprobar_resultado "'000' aceptada"          "$TMP_DIR/impar0.txt" "000"   "ACEPTADA"
comprobar_resultado "'00' rechazada"          "$TMP_DIR/impar0.txt" "00"    "NO ACEPTADA"
comprobar_resultado "'111' rechazada"         "$TMP_DIR/impar0.txt" "111"   "NO ACEPTADA"
comprobar_resultado "Cadena vacia rechazada"  "$TMP_DIR/impar0.txt" ""      "NO ACEPTADA"
echo

# --- 3. MT de 2 cintas y cinta infinita por la izquierda ---------------

echo "=== 3. Varias cintas y cinta infinita ==="
comprobar_resultado "Copia 2 cintas: se acepta 'abba'"      "$TMP_DIR/copia2.txt" "abba" "ACEPTADA"
comprobar_cinta     "Copia 2 cintas: cinta 2 contiene 'abba'" "$TMP_DIR/copia2.txt" "abba" 2 "abba[.]"
comprobar_resultado "Cinta infinita a la izquierda"          "$TMP_DIR/izquierda.txt" "a" "ACEPTADA"
comprobar_cinta     "Cinta infinita: se escribe a la izquierda" "$TMP_DIR/izquierda.txt" "a" 1 "[x]xa"
echo

# --- 4. Cadenas con simbolos fuera de Sigma ----------------------------

echo "=== 4. Cadenas con simbolos fuera de Sigma ==="
# No aborta: informa del error y sigue con las demas cadenas.
printf '0\n12\n01\n' > "$TMP_DIR/mixtas.txt"
salida_ok=$("$EJECUTABLE" -config "$TMP_DIR/impar0.txt" -in "$TMP_DIR/mixtas.txt" 2>"$TMP_DIR/stderr.txt")
codigo=$?
if [ "$codigo" -eq 0 ]; then
  reportar "El programa no aborta (codigo de salida 0)" 0
else
  reportar "El programa no aborta (codigo de salida 0)" 1
fi
if grep -q "^Error:" "$TMP_DIR/stderr.txt"; then
  reportar "Se informa del error por stderr" 0
else
  reportar "Se informa del error por stderr" 1
fi
num_resultados=$(printf '%s\n' "$salida_ok" | grep -c '^Resultado:')
if [ "$num_resultados" -eq 2 ]; then
  reportar "Se procesan las otras 2 cadenas" 0
else
  reportar "Se procesan las otras 2 cadenas" 1
fi
echo

# --- 5. Lectura de cadenas por fichero (-in) ---------------------------

echo "=== 5. Lectura de cadenas por fichero (-in) ==="
printf '0\n00\n\n' > "$TMP_DIR/cadenas.txt"
salida_fichero=$("$EJECUTABLE" -config "$TMP_DIR/impar0.txt" -in "$TMP_DIR/cadenas.txt")
num_cadenas=$(printf '%s\n' "$salida_fichero" | grep -c '^Entrada:')
if [ "$num_cadenas" -eq 3 ]; then
  reportar "Se procesan las 3 cadenas del fichero (incluida la vacia)" 0
else
  reportar "Se procesan las 3 cadenas del fichero (incluida la vacia)" 1
fi
echo

# --- 6. MT 1: L = { a^n b^m | m >= n, n > 0 } ---------------------------

echo "=== 6. MT 1 (a^n b^m, m >= n, n > 0) ==="
for cadena in ab abb aabb aabbb aaabbbbb abbbbbb; do
  comprobar_resultado "'$cadena' aceptada" "$MT1" "$cadena" "ACEPTADA"
done
for cadena in "" a b aa bb ba aab aaabb aba abab aaa bbaa; do
  comprobar_resultado "'${cadena:-vacia}' rechazada" "$MT1" "$cadena" "NO ACEPTADA"
done
echo

# --- 7. MT 2: contar b y a en unario ----------------------------------

echo "=== 7. MT 2 (cuenta de b y a en unario) ==="
comprobar_cinta "abbabaabb -> 11111.1111"  "$MT2" "abbabaabb" 1 "[1]1111.1111"
comprobar_cinta "aa -> 0.11"               "$MT2" "aa"        1 "[0].11"
comprobar_cinta "bb -> 11.0"               "$MT2" "bb"        1 "[1]1.0"
comprobar_cinta "ab -> 1.1"                "$MT2" "ab"        1 "[1].1"
comprobar_cinta "babab -> 111.11"          "$MT2" "babab"     1 "[1]11.11"
comprobar_cinta "Cadena vacia -> 0.0"      "$MT2" ""          1 "[0].0"
comprobar_cinta "La cinta 2 queda vacia"   "$MT2" "abbabaabb" 2 "[.]"
comprobar_resultado "La MT 2 acepta siempre (abba)" "$MT2" "abba" "ACEPTADA"
echo

# --- Resumen -----------------------------------------------------------

echo "=== Resumen ==="
echo "Pasados : $PASADOS"
echo "Fallados: $FALLADOS"

[ "$FALLADOS" -eq 0 ]
