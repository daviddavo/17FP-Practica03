#include <string>
#include <algorithm> // min
#include <iostream>
#include <fstream>
#include "puntuaciones.h"
using namespace std;

bool sorter(const tPuntuacion & p1, const tPuntuacion & p2){
    // Si el nombre está vacío, ignoramos los puntos, lo ponemos al final de la lista
    return p1.puntos > p2.puntos || p2.nombre == "";
}

bool guardarPuntuaciones(const tPuntuaciones puntuaciones){
    ofstream file;
    file.open(FILE_PUNT);
    for(unsigned i = 0; i < MAX_PUNTUACIONES && !file.fail(); i++){
        if(puntuaciones[i].puntos != 0)
            file << puntuaciones[i].nombre << " " << puntuaciones[i].puntos << endl;
    }

    return file.fail();
}

bool cargarPuntuaciones(tPuntuaciones puntuaciones){
    ifstream file;
    file.open(FILE_PUNT);

    // file.eof hace que no falle al leer el archivo, por si MAX_PUNTUACIONES > numero de lineas en el fichero
    for(unsigned i = 0; i < MAX_PUNTUACIONES; i++){
        tPuntuacion puntuacion = {"", 0};
        puntuaciones[i] = puntuacion;
        if(file >> puntuacion.nombre >> puntuacion.puntos) puntuaciones[i] = puntuacion;;
    }

    // Lo ordenamos por si alguien ha tocado el fichero manualmente (que tramposo)
    sort(puntuaciones, puntuaciones+MAX_PUNTUACIONES, sorter); // Por algo hemos definido operator<
    file.close();
    return !file.fail();
}

bool actualizarPuntuacion(tPuntuaciones puntuaciones, const std::string nombre, const unsigned add){
    // Add son los puntos que se suman, es bastante complejo intentar de calcular "nuevos"
    bool f = false; // found
    unsigned vacio = MAX_PUNTUACIONES;
    // Buscamos el jugador y lo actualizamos
    for(unsigned i = 0; i < MAX_PUNTUACIONES && !f; i++){
        if(puntuaciones[i].nombre == nombre){
            f = true;
            puntuaciones[i].puntos += add;
        }else if(puntuaciones[i].nombre == ""){
            vacio = i;
        }
    }
    // No encontrado, lo añadimos en un espacio vacío
    if(!f && vacio < MAX_PUNTUACIONES){
        puntuaciones[vacio] = {nombre, add};
        f = true;
    }
    // No hay espacio para uno nuevo :(
    else if(!f){
        // Al ser una lista ordenada, el ultimo será el de menor puntuaciones
        puntuaciones[MAX_PUNTUACIONES-1] = {nombre, add};
        f = true;
    }

    // tras añadirlo, reordenamos la lista
    // No podemos hacer una función "insertar", pues también necesitaríamos "actualizar"
    sort(puntuaciones, puntuaciones + MAX_PUNTUACIONES, sorter);
    return f;
}

void mostrarPuntuaciones(const tPuntuaciones puntuaciones){
    for(unsigned int i = 0; i < MAX_PUNTUACIONES; i++){
        if(puntuaciones[i].nombre != ""){
            cout << "\t\t\t" <<  puntuaciones[i].nombre << ": " << puntuaciones[i].puntos << " punto(s)" << endl;
        }
    }
}
