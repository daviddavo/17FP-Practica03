#include "puntuaciones.h"

bool operator<(const tPuntuacion & p1, const tPuntuacion & p2){
    return p1.puntos < p2.puntos;
}

bool actualizarPuntuacion(tPuntuaciones & puntuaciones, const std::string nombre, const unsigned nuevos){
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
