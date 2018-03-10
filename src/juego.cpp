#include <iostream>
#include <fstream>
#include <string>
#include "secuenciaCartas.h"
#include "puntuaciones.h"
#include "tablero.h"
#include "color.h"
#include "juego.h"

using namespace std;

string pedirFichero(){
    string fname;

    cout << "Introduzca el nombre del mapa (con extension): ";
    cin >> fname;

    return fname;
}

string carta2str(const tCarta carta){
    switch(carta){
    case AVANZAR: return "^";
    case GIROIZQUIERDA: return "<";
    case GIRODERECHA: return ">";
    case LASER: return "~";
    default: return " ";
    }
}

unsigned pedirJugadores(){
    unsigned j;
    cout << "Introduce el numero de jugadores (1-" << MAX_JUGADORES<< "): ";
    cin >> j;
    while(j < 1 || j > MAX_FILAS){
        cout << "Introduce un numero del 1 al " << MAX_JUGADORES << ": ";
        cin >> j;
    }

    return j;
}

void mostrarJugador(const tJugador & jugador, const unsigned width, const bool jugando){
    // Le ponemos color a la tabla:
    colorFondo(paleta[NUM_TIPOS_CASILLAS+jugador.id-1]);
    // Calculamos el número de espacios necesarios para mostrar al jugador
    unsigned nSpaces = width - jugador.nombre.length() + 1;
    cout << (jugando?'>':' ') << ' ' << jugador.id << '.';
    // Mostramos los espacios
    for(unsigned i = 0; i < nSpaces; i++) cout << ' ';
    // Mostramos el nombre del jugador
    cout << jugador.nombre << ": ";
    colorReset();
    // Ahora mostramos la mano
    for(unsigned i = 0; i < NADA; i++){
        cout << ((jugador.mano[i] > 10)?" ":"  ") << jugador.mano[i];
        colorFondo(COLOR_MANO);
        cout << ' ' << carta2str(static_cast<tCarta>(i)) << ' ';
        colorReset();
    }

    cout << endl;
}

void mostrarJugadores(const tJugador jugadores[MAX_JUGADORES], const unsigned nJugadores, const unsigned turno){
    // Jugando es el ID del jugador que está jugando
    // Primero vamos a calcular el ancho necesario para mostrar a los jugadores
    unsigned maxw = 0;
    for(unsigned int i = 0; i < nJugadores; i++){
        // Esto hace lo mismo que max() pero para unsigned
        maxw = (maxw<jugadores[i].nombre.length())?jugadores[i].nombre.length():maxw;
    }
    for(unsigned int i = 0; i < nJugadores; i++){
        mostrarJugador(jugadores[i], maxw, jugadores[i].id == turno);
    }
}

void clear(){
#ifdef _WIN32_
    system("cls");
#elif __linux__
    system("clear");
#endif
}

void mostrarJuego(const tJuego & juego){
    clear();
    mostrarTablero(juego.tablero);
    cout << endl << "JUGADORES:" << endl;
    mostrarJugadores(juego.jugadores, juego.nJugadores, juego.turno);
}

bool cargarJuego(tJuego & juego){
    // s = success
    juego.nJugadores = pedirJugadores();
    bool s = cargarTablero(juego.tablero, pedirFichero(), juego.nJugadores);

    return s;
}
