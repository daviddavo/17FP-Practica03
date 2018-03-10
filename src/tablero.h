/*
 * tablero.h
 *
 *  Created on: 9 mar. 2018
 *      Author: davo
 */

#ifndef SRC_TABLERO_H_
#define SRC_TABLERO_H_

#include <string>

const unsigned MAX_FILAS = 8;
const unsigned NUM_TIPOS_CASILLAS = 6;

typedef enum {NORTE, SUR, ESTE, OESTE} tDir;
typedef enum {VACIA, HIELO, MURO, CAJA, JOYA, TORTUGA} tEstadoCasilla;
typedef struct{
    unsigned numero;
    tDir direccion;
} tTortuga;
typedef struct {
    tEstadoCasilla estado;
    tTortuga tortuga;
} tCasilla;

typedef tCasilla tTablero[MAX_FILAS][MAX_FILAS];


bool cargarTablero(tTablero, const std::string, const unsigned);
void mostrarTablero(const tTablero);

#endif /* SRC_TABLERO_H_ */
