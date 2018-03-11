#include "juego.h"

#include "cli.h"
#include "mazo.h"

using namespace std;

void calcularDir(unsigned & x, unsigned & y, const tDir dir){
    switch(dir){
    case NORTE: x--; break;
    case SUR: x++; break;
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
        avanzar(juego);
    }
}

bool avanzar(tJuego & juego){
    bool joya;
    unsigned x2 = juego.jugadores[juego.turno].x;
    unsigned y2 = juego.jugadores[juego.turno].y;
    tTortuga tortuga = juego.tablero[x2][y2].tortuga;

    cout << tortuga.direccion << endl;
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
            addMsg(juego.log, juego.jugadores[juego.turno].nombre + " se ha movido a " + to_string(x2) + "," + to_string(y2));
            juego.tablero[x2][y2].estado = TORTUGA;
            juego.tablero[x2][y2].tortuga = tortuga;
            juego.tablero[juego.jugadores[juego.turno].x][juego.jugadores[juego.turno].y].estado = VACIA;
            juego.jugadores[juego.turno].x = x2;
            juego.jugadores[juego.turno].y = y2;
        }
    }

    return joya;
}

void gira(tJuego & juego, const bool derecha){
    unsigned x = juego.jugadores[juego.turno].x, y = juego.jugadores[juego.turno].y;
    tDir direccion = juego.tablero[x][y].tortuga.direccion;
    if(derecha){
        // El +4 es para que no entre en el rango de los negativos
        // Va a seguir funcionando en los positivos pues n%4 = (n+4)%4
        direccion = static_cast<tDir>( (direccion+1+4)%4 );
    } else {
        direccion = static_cast<tDir>( (direccion-1+4)%4 );
    }
    juego.tablero[x][y].tortuga.direccion = direccion;
}

bool ejecutarTurno(tJuego & juego){
    bool joya = false, update;
    tJugador jugador = juego.jugadores[juego.turno];

    while(!joya){
        update = true;
        tecla::tTecla tecla = leerTecla();
        cout << tecla << endl;
        switch(tecla){
        case tecla::AVANZA: joya = avanzar(juego); break;
        case tecla::DERECHA: gira(juego, 1); break;
        case tecla::IZQUIERDA: gira(juego, 0); break;
        default: update = false;
        }
        if(update) mostrarJuego(juego);
    }

    // mostrarJuego(juego);


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
    // Leemos el tablero
    bool s = cargarTablero(juego.tablero, pedirFichero(), juego.nJugadores);

    // Asignamos los jugadores buscando las tortugas por el tablero
    unsigned int i = 0;
    for(unsigned x = 0; x < MAX_FILAS && i < juego.nJugadores; x++){
        for(unsigned y = 0; y < MAX_FILAS && i < juego.nJugadores; y++){
            if(juego.tablero[x][y].estado == TORTUGA){
                juego.jugadores[i].id = i;
                juego.jugadores[i].x = x;
                juego.jugadores[i].y = y;
                // Ponemos la mano del jugador a 0
                fill(juego.jugadores[i].mano, juego.jugadores[i].mano+NADA, 0);
                i++;
            }
        }
    }

    return s;
}
