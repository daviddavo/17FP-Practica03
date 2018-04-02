/*
 * tablero.h
 *
 *  Created on: 24 mar. 2018
 *      Author: davo
 */

#ifndef SRC_TABLERO_H_
#define SRC_TABLERO_H_

#include <fstream>
#include <iostream>
#include <string>
#include "config.h"

typedef enum { NORTE, ESTE, SUR, OESTE } tDir;
typedef enum { VACIA, HIELO, MURO, CAJA, JOYA, TORTUGA, ANIMATION_LASER } tEstadoCasilla;
typedef struct {
    // tTortuga tambien es usado para laser, siendo numero la
    // tortuga que lo ha lanzado y tDir la direccion del laser
    unsigned numero;
    tDir direccion;
} tTortuga;

typedef struct {
    tEstadoCasilla estado;
    tTortuga tortuga;
} tCasilla;

typedef tCasilla tTablero[MAX_FILAS][MAX_FILAS];
bool validfName(const std::string);  // Retorna si es un nombre de fichero valido
bool cargarTablero(tTablero, const std::string, const unsigned);

#endif  // SRC_TABLERO_H_
