#ifndef SRC_CLI_H_
#define SRC_CLI_H_

// Es recomendable incluir todas las cabeceras necesarias para que
// el módulo funcione. Así si queremos exportar cli.h a otra práctica
// Sólo habrá que poner include cli.h, y no 6 líneas con includes
// Incluso se podría poner la cabecera precompilada
#include <iostream>

#include "config.h"

#include "color.h"
#include "juego.h"

#ifdef _WIN32_
    #include <conio.h> // Para getch
#elif __linux__
    #include <termios.h> // Para el emulador getch
    #include <unistd.h> // tcsetattr

    int getch();
#endif

namespace tecla{
    typedef enum {AVANZA, DERECHA, IZQUIERDA, DISPARO, NADA, SALIR} tTecla;
}

typedef std::string tLog[LOG_SIZE];

void mostrarJuego(const tJuego & juego);
void mostrarJugadores(const tJugador [MAX_JUGADORES], const unsigned, const unsigned); // DEBUGGING
void flushLog(tLog);
void addMsg(tLog, const std::string);
void mostrarLog(const tLog); // DEBUGGING
std::string pedirFichero();
unsigned pedirJugadores();
void anyKey();
tecla::tTecla leerTecla();

#endif /* SRC_CLI_H_ */
