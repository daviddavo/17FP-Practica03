#ifndef SRC_CLI_H_
#define SRC_CLI_H_

// Es recomendable incluir todas las cabeceras necesarias para que
// el módulo funcione. Así si queremos exportar cli.h a otra práctica
// Sólo habrá que poner include cli.h, y no 6 líneas con includes
// Incluso se podría poner la cabecera precompilada
#include <iostream>
#include <time.h>

#include "config.h"

#include "color.h"
#include "juego.h"
#include "puntuaciones.h"

#ifdef _WIN32_
    #include <conio.h> // Para getch
#elif __linux__
    #include <termios.h> // Para el emulador getch
    #include <unistd.h> // tcsetattr

    int getch(); // Emulador de getch de conio.h
#endif

namespace tecla{
    typedef enum {AVANZA, DERECHA, IZQUIERDA, DISPARO, NADA, SALIR, ROBAR, JUGAR} tTecla;
}

typedef std::string tLog[LOG_SIZE];

void mostrarJuego(const tJuego & juego); // Muestra el juego: header, body y jugadores
void mostrarJugadores(const tJugador [MAX_JUGADORES], const unsigned, const unsigned); // DEBUGGING
void flushLog(tLog); // Borra el log (a veces es necesario)
void addMsg(tLog, const std::string); // Añade un mensaje al log
std::string pedirFichero(); // Pide el nombre del fichero
unsigned pedirJugadores(); // Pide el número de jugadores
void anyKey(); // Equivalente a system(pause)
tecla::tTecla leerTecla(); // Lee una tecla
void mostrarBody(const tTablero, const tLog); // Muestra el "cuerpo" del juego (mapa y log)
bool animateLaser(tJuego &, const tDir, unsigned &, unsigned &, const unsigned); // Animación del láser en una direccion
bool continuar(const tPuntuaciones, const bool); // Pregunta si se desea seguir jugando
void gameOver(const tJuego &); // La pantalla al finalizar el juego
void mainMenu(); // Menu principal del juego

#endif /* SRC_CLI_H_ */
