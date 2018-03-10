#include <string>
#include <algorithm> // min
#include <iostream>
#include <fstream>
#include "puntuaciones.h"
using namespace std;

bool operator<(const tPuntuacion & p1, const tPuntuacion & p2){
    return p1.puntos < p2.puntos;
}

bool guardarPuntuaciones(const tPuntuaciones puntuaciones){
    ofstream file;
    file.open(FILE_PUNT);
    for(unsigned i = 0; i < MAX_JUGADORES && !file.fail(); i++){
        file << puntuaciones[i].nombre << " " << puntuaciones[i].puntos << endl;
    }

    return file.fail();
}

bool cargarPuntuaciones(tPuntuaciones puntuaciones){
    ifstream file;
    file.open(FILE_PUNT);

    for(unsigned i = 0; i < MAX_JUGADORES && !file.fail(); i++){
        tPuntuacion puntuacion;
        file >> puntuacion.nombre >> puntuacion.puntos;
        puntuaciones[i] = puntuacion;
    }

    file.close();
    return !file.fail();
}

bool actualizarPuntuacion(tPuntuaciones puntuaciones, const std::string nombre, const unsigned nuevos){
    bool f = false; // found
    unsigned vacio = MAX_JUGADORES;
    // Buscamos el jugador y lo actualizamos
    for(unsigned i = 0; i < MAX_JUGADORES && !f; i++){
        if(puntuaciones[i].nombre == nombre){
            f = true;
            puntuaciones[i].puntos = nuevos;
        }else if(puntuaciones[i].nombre == ""){
            vacio = i;
        }
    }
    // No encontrado
    if(!f && vacio < MAX_JUGADORES){
        puntuaciones[vacio] = {nombre, nuevos};
        f = true;
    }
    // No hay espacio para uno nuevo :(
    else if(!f){
        // Buscamos el jugador con menos puntos
        unsigned mini = 0;
        for(unsigned i = 1; i < MAX_JUGADORES; i++){
            mini = (puntuaciones[i]<puntuaciones[mini])?i:mini;
            /* Lo de la anterior línea es equivalente a lo siguiente:
            if(puntuaciones[i] < puntuaciones[mini]){
                mini = i;
            }else{
                mini = mini;
            }*/
        }
        // Y lo sustituimos
        puntuaciones[mini] = {nombre, nuevos};
        f = true;
    }

    return f;
}

void mostrarPuntuaciones(const tPuntuaciones puntuaciones){
    for(unsigned int i = 0; i < MAX_JUGADORES; i++){
        if(puntuaciones[i].nombre != ""){
            cout << puntuaciones[i].nombre << ": " << puntuaciones[i].puntos << " punto(s)" << endl;
        }
    }
}
