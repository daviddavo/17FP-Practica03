#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

// Archivo para constantes y otras configuraciones

#include <string>

/* TABLERO */
constexpr unsigned MAX_FILAS = 8;
constexpr unsigned MAX_JUGADORES = 4;

/* PUNTUACIONES */
constexpr unsigned MAX_PUNTUACIONES = MAX_JUGADORES;
constexpr std::string FILE_PUNTUACIONES = "puntuaciones.txt";

#endif /* SRC_CONFIG_H_ */
