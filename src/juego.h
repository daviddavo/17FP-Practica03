#ifndef SRC_JUEGO_H_
#define SRC_JUEGO_H_

#include <string>
#include <algorithm>

#include "mazo.h"
#include "tablero.h"
#include "puntuaciones.h"

// Usaremos los índices segun el enum tCarta: AVANZAR = 0, GIROIZQUIERDA = 1...
typedef unsigned tMano[NADA];

typedef struct{
    std::string nombre;
    unsigned id;
    tMazo mazo; // Cartas en su mazo
    tMano mano; // Cartas en la mano del jugador
    unsigned x; // Corrdenadas
    unsigned y;
    bool jugando;
} tJugador;

typedef struct{
    unsigned nJugadores; // Numero de jugadores, siempre menor que MAX_JUGADORES
    unsigned turno; // Turno actual 0 a MAX_JUGADORES-1
    unsigned joya; // Ultimo jugador en haber conseguido una joya
    tJugador jugadores[MAX_JUGADORES]; // Almacena los jugadores
    tTablero tablero; // Almacena el tablero
    std::string log[LOG_SIZE]; // Los mensajes para la interfaz
} tJuego;

bool cargarJuego(tJuego & juego);
bool ejecutarTurno(tJuego & juego);
bool accionRobar(tJuego & juego);
bool accionSecuencia(tJuego & juego, tMano & cartas);
void cambiarTurno (tJuego & juego);
bool esFinDePartida(tJuego & juego);
void incluirCarta(tMano &mano, tCarta carta);
bool calcularDir(unsigned &, unsigned &, const tDir);
void jugar(tPuntuaciones &);

// DEBUG ONLY
bool avanzar(tJuego &);

#endif /* SRC_JUEGO_H_ */
