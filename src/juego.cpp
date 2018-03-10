#include <iostream>
#include <fstream>
#include <string>
#include "secuenciaCartas.h"
#include "puntuaciones.h"
#include "tablero.h"
#include "juego.h"

using namespace std;

string pedirFichero(){
    string fname;

    cout << "Introduzca el nombre del mapa (con extension): ";
    cin >> fname;

    return fname;
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

bool cargarJuego(tJuego & juego){
    // s = success
    bool s = cargarTablero(juego.tablero, pedirFichero(), pedirJugadores());
    juego.jugadores = pedirJugadores();

    return s;
}
