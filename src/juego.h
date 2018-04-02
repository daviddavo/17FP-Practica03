/*
 * juego.h
 *
 *  Created on: 24 mar. 2018
 *      Author: davo
 */

#ifndef SRC_JUEGO_H_
#define SRC_JUEGO_H_

#include <algorithm>  // fill
#include <random>
#include <string>

#include "config.h"

#include "mazo.h"
#include "tablero.h"

// En lugar de un tipo estructurado en el que todos sus valores son del mismo
// tipo, usaremos un array en el que sus indices indican que carta es
typedef int tMano[carta::NADA];

typedef struct {
    int x;
    int y;
} tCoord;

typedef enum { SECUENCIA, ROBAR, BICHO } tJugada;

typedef struct {
    // El tamaño máximo será el número máximo de láseres
    tCoord bloquesDerretidos[nCartas[carta::LASER]];
    unsigned nBloquesDerretidos;
    tCoord prevCoord;
    tDir prevDir;
    carta::tCarta cartaRobada;
    unsigned nCartasUsadas;  // Numero de cartas usadas, las sacaremos del mazo
    tJugada jugada;  // Si ha robado en la ultima jugada
    // En caso de ser false, entendemos que ha 'jugado', ignorando bicho
} tUltimaJugada;

typedef struct {
    std::string nombre;
    int id;                      // Jugador #
    tCoord pos;                  // Posición actual
    tCoord spawn;                // Coordenadas en las que aparece por primera vez la tortuga
    tMano mano;                  // Cantidad de cartas de cada tipo en la mano
    tMazo mazo;                  // El mazo del jugador
    bool jugando;                // Sigue jugando?
    tUltimaJugada ultimaJugada;  // Datos relevantes a BICHO
} tJugador;

typedef struct {
    int nJugadores;
    int turno;  // Turno actual del 0 a MAX_JUGADORES -1
    int joya;
    tJugador jugadores[MAX_JUGADORES];  // Jugadores
    tTablero tablero;                   // Almacena el tablero
    std::string log[LOG_SIZE];          // Lista de mensajes de la interfaz
} tJuego;

void vaciarMano(tMano);                  // Vacía la mano
bool manoVacia(tMano);                   // Retorna true si todos los elementos son 0 o menores
bool calcularPos(tCoord &, const tDir);  // Calcula la siguiente posicion (si es posible) dada una dir
void cambiarTurno(tJuego &);             // Avanza un turno
bool cargarJuego(tJuego &, const std::string, const int, const std::string[]);  // Carga el Juego
bool accionRobar(tJuego &);  // Hace la accion de robar una carta para el jugador del turno actual

bool moverTortuga(tJuego &, const tCoord &, const tTortuga &);  // Mueve la tortuga a unas coordenadas

void addMsg(std::string[LOG_SIZE], std::string);

// AHORA LAS ACCIONES DEL TABLERO
bool avanzar(tJuego &);
void girar(tJuego &, const bool);
bool disparar(tJuego &);
void ejecutarBicho(tJuego &, int);

#endif  // SRC_JUEGO_H_
