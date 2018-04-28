/*
 * puntuaciones.h
 *
 *  Created on: 24 mar. 2018
 *      Author: davo
 */

#ifndef SRC_PUNTUACIONES_H_
#define SRC_PUNTUACIONES_H_

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

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
    tPuntuacion * puntuaciones;
    unsigned cnt;
	unsigned MAX;
} tPuntuaciones;

// Nota: Puntuaciones es una lista ordenada, para hacerlo mas bonito
bool cargarPuntuaciones(tPuntuaciones &);         // Carga puntuaciones del fichero
bool guardarPuntuaciones(const tPuntuaciones &);  // Guarda las puntuaciones al fichero
// Nota: mostrarPuntuaciones ha sido movido al modulo interface.cpp
bool actualizarPuntuacion(tPuntuaciones &, const std::string, const unsigned);  // Actualiza el struct
void redimensionar(tPuntuaciones &, const unsigned = 4);  // Redimensionar el array de puntuaciones
void liberar(tPuntuaciones &); // Delete del array

#endif  // SRC_PUNTUACIONES_H_
