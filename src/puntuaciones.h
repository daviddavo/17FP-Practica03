/*
 * puntuaciones.h
 *
 *  Created on: 24 mar. 2018
 *      Author: davo
 */

#ifndef SRC_PUNTUACIONES_H_
#define SRC_PUNTUACIONES_H_

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>
#include "config.h"

typedef struct {
  std::string nombre;
  unsigned puntos;
} tPuntuacion;
typedef struct {
  tPuntuacion puntuaciones[MAX_PUNTUACIONES];
  unsigned cnt;
} tPuntuaciones;

// Nota: Puntuaciones es una lista ordenada, para hacerlo mas bonito
bool cargarPuntuaciones(tPuntuaciones &);  // Carga puntuaciones del fichero
bool guardarPuntuaciones(const tPuntuaciones &);  // Guarda las puntuaciones al fichero
// Nota: mostrarPuntuaciones ha sido movido al modulo interface.cpp
bool actualizarPuntuacion(tPuntuaciones &, const std::string, const unsigned);  // Actualiza el struct

#endif  // SRC_PUNTUACIONES_H_
