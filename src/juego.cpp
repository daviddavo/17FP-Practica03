#include "juego.h"

#include "cli.h"
#include "mazo.h"

using namespace std;

bool accionRobar(tJuego & juego){
    tCarta carta;
    bool success = sacar(juego.jugadores[juego.turno].mazo, carta);
    if(success){
        juego.jugadores[juego.turno].mano[carta]++;
    }
    return success;
}

bool cargarJuego(tJuego & juego){
    // s = success
    juego.nJugadores = pedirJugadores();
    bool s = cargarTablero(juego.tablero, pedirFichero(), juego.nJugadores);

    return s;
}
