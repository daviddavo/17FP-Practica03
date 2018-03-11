/*
 * tablero.h
 *
 *  Created on: 9 mar. 2018
 *      Author: davo
 */

#ifndef SRC_TABLERO_H_
#define SRC_TABLERO_H_

#include <string>

#include "config.h"

typedef enum {NORTE, ESTE, SUR, OESTE} tDir;
typedef enum {VACIA, HIELO, MURO, CAJA, JOYA, TORTUGA, ANIMATION_LASER} tEstadoCasilla;
typedef struct{ // Esta estructura también sirve para laser
    unsigned numero; // En caso de ANIMATION_LASER, será el color
    tDir direccion; // Aquí su direccion
} tTortuga;
typedef struct {
    tEstadoCasilla estado;
    tTortuga tortuga;
} tCasilla;

typedef tCasilla tTablero[MAX_FILAS][MAX_FILAS];

bool cargarTablero(tTablero, const std::string, const unsigned);

#endif /* SRC_TABLERO_H_ */
