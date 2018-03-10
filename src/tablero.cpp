#include <iostream>
#include <fstream>
#include "tablero.h"
#include "puntuaciones.h" // Dependencia de color.h
#include "color.h"

using namespace std;

tCasilla char2casilla(const char c, unsigned & nTortuga){
    tCasilla casilla;

    // Estado por defecto es tortuga (así no hay que ponerlo 4 veces)
    casilla.estado = TORTUGA;

    switch(c){
    case '#': casilla.estado = MURO; break;
    case '@': casilla.estado = HIELO; break;
    case '*': // Resulta que * es vacía y no lo pone en la práctica
    case ' ': casilla.estado = VACIA; break;
    case '$': casilla.estado = JOYA; break;
    case 'C': casilla.estado = CAJA; break;
    case 'U': casilla.tortuga.direccion = NORTE; break;
    case 'D': casilla.tortuga.direccion = SUR; break;
    case 'R': casilla.tortuga.direccion = ESTE; break;
    case 'L': casilla.tortuga.direccion = OESTE; break;
    // Y aquí si pudiese levantaría una excepción en default...
    }

    // Si el estado no ha cambiado:
    if(casilla.estado == TORTUGA)
        casilla.tortuga.numero = nTortuga++;

    return casilla;
}

// Si no pasamos ifstream con &, intentará "borrarlo" y dará error
bool leerTablero(tTablero tablero, ifstream & file){
    // Aqui es donde leemos las MAX_FILAS filas del tablero
    bool fail = false;
    string line;
    unsigned nTortuga = 0;

    for(unsigned i = 0; i < MAX_FILAS && !fail; i++){
        getline(file, line);
        fail = file.fail();
        for(unsigned j = 0; j < MAX_FILAS && !fail; j++){
            tablero[i][j] = char2casilla(line[j], nTortuga);
        }
    }

    return !fail;
}

bool cargarTablero(tTablero tablero, const string fname, const unsigned jugadores){
    unsigned n = 0, i = 0;
    string linea;
    ifstream file;

    file.open(fname);

    bool error = file.fail();
    while(!error && n != jugadores && i < MAX_JUGADORES){
        file >> n;
        // Terminamos de leer la linea
        getline(file, linea);
        if(n == jugadores){
            error = leerTablero(tablero, file);
        }
        else {
            for(unsigned k = 0; k < MAX_FILAS; k++){
                getline(file, linea);
            }
        }
        error = error || file.fail();
        i++;
    }

    file.close();

    return !error && i < MAX_JUGADORES;
}

void imprimirCasilla(const tCasilla casilla){
    // Por defecto color fondo es el de la paleta
    if(casilla.estado!=TORTUGA) colorFondo(paleta[casilla.estado]);

    switch(casilla.estado){
    case VACIA: cout << "  "; break;
    case HIELO: cout << "**"; break;
    case MURO: cout << "||"; break;
    case CAJA: cout << "[]"; break;
    // Sé que dijo 00, pero en la terminal quedan raro
    case JOYA: cout << "00"; break;
    case TORTUGA:
        colorFondo(paleta[casilla.tortuga.numero+NUM_TIPOS_CASILLAS]);
        switch(casilla.tortuga.direccion){
        case NORTE: cout << "^^"; break;
        case SUR: cout << "vv"; break;
        case ESTE: cout << ">>"; break;
        case OESTE: cout << "<<"; break;
        }
    }

    colorReset();
}

void mostrarTablero(const tTablero tablero){
    // Asumimos que se ha hecho ya cls
    for(unsigned i = 0; i < MAX_FILAS; i++){
        for(unsigned j = 0; j < MAX_FILAS; j++){
            imprimirCasilla(tablero[i][j]);
        }
        cout << endl;
    }
}
