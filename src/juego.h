/*
 * juego.h
 *
 *  Created on: 9 mar. 2018
 *      Author: davo
 */

#ifndef SRC_JUEGO_H_
#define SRC_JUEGO_H_

typedef enum {AVANZA, DERECHA, IZQUIERDA, DISPARO, SALIR} tTecla;

// Usaremos los índices segun el enum tCarta: AVANZAR = 0, GIROIZQUIERDA = 1...
typedef unsigned tMano[NADA];

typedef struct{
    std::string nombre;
    tMazo mazo; // Cartas en su mazo
    tMano mano; // Cartas en la mano del jugador
    unsigned x; // Corrdenadas
    unsigned y;
} tJugador;

typedef struct{
    unsigned jugadores;
    unsigned turno;
    tJugador jugador[MAX_JUGADORES];
    tTablero tablero;
} tJuego;

bool cargarJuego(tJuego juego);
void mostrarJuego(const tJuego & juego);
bool ejecutarTurno(tJuego & juego);
bool accionRobar(tJuego & juego);
bool accionSecuencia(tJuego & juego, tMazo & cartas);
void cambiarTurno (tJuego & juego);
bool esFinDePartida(tJuego & juego);
void incluirCarta(tMano &mano, tCarta carta);

#endif /* SRC_JUEGO_H_ */
