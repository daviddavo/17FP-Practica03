#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

// Archivo para constantes y otras configuraciones

#include <string>

/* TABLERO */
constexpr unsigned MAX_FILAS = 8;
constexpr unsigned MAX_JUGADORES = 4;
constexpr unsigned NUM_TIPOS_CASILLAS = 7; // Numero de tEstados distintos

/* PUNTUACIONES */
constexpr unsigned MAX_PUNTUACIONES = MAX_JUGADORES;
constexpr std::string FILE_PUNTUACIONES = "puntuaciones.txt";

/* INTERFAZ */
const unsigned paleta[NUM_TIPOS_CASILLAS+MAX_JUGADORES] = {1,11,7,4,12,5, 1, 13,9,10,3};
const unsigned COLOR_TEXTO = 15;

const unsigned MOVE_DELAY = 200; // Tiempo en ms que dura cada "fotograma"
// de la animación de movimiento

#endif /* SRC_CONFIG_H_ */
