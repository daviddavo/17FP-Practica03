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
  std::string nombre;
  int id;  // Jugador #
  int x;   // Posicion actual de la tortuga
  int y;
  int sx;  // Spawn x
  int sy;  // Spawn y
  tMano mano;
  tMazo mazo;
  bool jugando;  // Sigue jugando?
                 // TODO: Bicho
} tJugador;

typedef struct {
  int nJugadores;
  int turno;  // Turno actual del 0 a MAX_JUGADORES -1
  int joya;
  tJugador jugadores[MAX_JUGADORES];  // Jugadores
  tTablero tablero;                   // Almacena el tablero
  std::string log[LOG_SIZE];          // Lista de mensajes de la interfaz
} tJuego;

bool manoVacia(tMano);  // Retorna true si todos los elementos son 0 o menores
bool calcularPos(int &, int &, const tDir);  // Calcula la siguiente posicion (si es posible) dada una dir
void cambiarTurno(tJuego &);  // Avanza un turno
bool cargarJuego(tJuego &, const std::string, const int, const std::string[]);  // Carga el Juego
bool accionRobar(tJuego &);  // Hace la accion de robar una carta para el jugador del turno actual

bool moverTortuga(tJuego &, const int, const int, const tTortuga &);  // Mueve la tortuga a unas coordenadas

void addMsg(std::string[LOG_SIZE], std::string);

// AHORA LAS ACCIONES DEL TABLERO
bool avanzar(tJuego &);
void girar(tJuego &, const bool);
bool disparar(tJuego &);

#endif  // SRC_JUEGO_H_
