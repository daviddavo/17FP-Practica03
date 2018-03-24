/*
 * tablero.h
 *
 *  Created on: 24 mar. 2018
 *      Author: davo
 */

#ifndef SRC_TABLERO_H_
#define SRC_TABLERO_H_

#include "config.h"
#include <string>
#include <iostream>
#include <fstream>

typedef enum {NORTE, ESTE, SUR, OESTE} tDir;
typedef enum {VACIA, HIELO, MURO, CAJA, JOYA, TORTUGA, ANIMATION_LASER} tEstadoCasilla;
typedef struct { // tTortuga también es usado para laser, siendo numero la tortuga
    unsigned numero; // que lo ha lanzado, y tDir la direccion del laser
    tDir direccion;
} tTortuga;

typedef struct{
    tEstadoCasilla estado;
    tTortuga tortuga;
} tCasilla;

typedef tCasilla tTablero[MAX_FILAS][MAX_FILAS];
bool cargarTablero(tTablero, const std::string, const unsigned);

#endif /* SRC_TABLERO_H_ */
