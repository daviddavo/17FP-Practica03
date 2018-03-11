#include "juego.h"

#include "cli.h"
#include "mazo.h"

using namespace std;

void calcularDir(unsigned & x, unsigned & y, const tDir dir){
    switch(dir){
    case NORTE: x++; break;
    case SUR: x--; break;
    case ESTE: y++; break;
    case OESTE: y--; break;
    }
}

void empujar(tJuego & juego, const unsigned x, const unsigned y, const tDir dir){
    unsigned x2 = x, y2 = y;
    calcularDir(x2, y2, dir);
    if(juego.tablero[x2][y2].estado == VACIA){
        juego.tablero[x2][y2] = juego.tablero[x][y];
        juego.tablero[x][y].estado = VACIA;
    }
}

bool avanzar(tJuego & juego){
    bool joya;
    unsigned x2 = juego.jugadores[juego.turno].x;
    unsigned y2 = juego.jugadores[juego.turno].y;
    tTortuga tortuga = juego.tablero[x2][y2].tortuga;

    calcularDir(x2, y2, tortuga.direccion);

    if(x2 > MAX_FILAS || y2 > MAX_FILAS){
        addMsg(juego.log, "No puedes moverte a esta casilla, está fuera del tablero");
    }else{
        switch(juego.tablero[x2][y2].estado){
        case HIELO:
        case MURO: addMsg(juego.log, "No puedes avanzar a esa casilla, hay un muro"); break;
        case TORTUGA: addMsg(juego.log, "No puedes ponerte encima de una tortuga"); break;
        case CAJA: empujar(juego, x2, y2, tortuga.direccion); break;
        case JOYA: joya = true; break;
        default:
            addMsg(juego.log, "Te has movido a " + x2 + ", " + y2);
            juego.tablero[x2][y2].estado = TORTUGA;
            juego.tablero[x2][y2].tortuga = tortuga;
            juego.tablero[juego.jugadores[juego.turno].x][juego.jugadores[juego.turno].y].estado = VACIA;
            juego.jugadores[juego.turno].x = x2;
            juego.jugadores[juego.turno].y = y2;
        }
    }

    return joya;
}

bool ejecutarTurno(tJuego & juego){
    bool joya = false;
    tJugador jugador = juego.jugadores[juego.turno];
    while(!joya){
        tecla::tTecla tecla = leerTecla();
        switch(tecla){
        case tecla::AVANZA: joya = avanzar(juego); break;
        }

    }

    return joya;
}

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
