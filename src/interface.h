/*
 * interface.h
 *
 *  Created on: 24 mar. 2018
 *      Author: davo
 */

#ifndef SRC_INTERFACE_H_
#define SRC_INTERFACE_H_

#include <time.h>  // Para las "animaciones"

#include <iostream>
#include <string>

#include "config.h"

#include "color.h"
#include "juego.h"
#include "puntuaciones.h"
#include "tablero.h"

#ifdef _WIN32
#include <conio.h>  // Para getch
#elif __linux__
#include <termios.h>  // Para el "emulador" de getch
#include <unistd.h>   // tcsetattr (getch)

int _getch();
#endif

namespace tecla {
typedef enum { AVANZA, DERECHA, IZQUIERDA, DISPARO, BICHO, NADA, SALIR, DELETE } tTecla;
}

void __sleep(const int);    // Hace lo mismo que yo un domingo (dormir) durante x milisegundos
void mainMenu();           // Menu principal del juego
void anyKey(std::string);  // Pulse cualquier tecla para continuar

#endif  // SRC_INTERFACE_H_
