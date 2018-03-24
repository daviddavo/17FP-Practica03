#include "mazo.h"

void crearVacia(tMazo & mazo){
    mazo.cnt = 0;
}

void crearMazoOrdenado(tMazo & mazo){
    // Primero rellenamos el array. Recorremos las distintas cartas que existen
    // vemos cuantas cartas de ese tipo hay en nCartas y las metemos
    // Sé que es un método un tanto raro, pero me parece mejor y más escalable
    // que hacer N for's
    crearVacia(mazo);
    unsigned j = 0; // Posicion de la carta en el array
    for(unsigned i = 0; i < carta::NADA; i++){
        for(unsigned k = 0; k < nCartas[i]; k++, j++){
            mazo.mazo[j] = static_cast<carta::tCarta>(i);
        }
    }
    mazo.cnt = j;
}

void crearMazoAleatorio(tMazo & mazo){
    // Creamos un mazo ordenado y lo barajamos
    crearMazoOrdenado(mazo);
    std::random_shuffle(mazo.mazo, mazo.mazo + mazo.cnt);
}

bool sacar(tMazo & mazo, carta::tCarta & carta){
    // Como me cabrea el .mazo[], si esto fuese una clase podría redefinir el operador
    bool posible = mazo.cnt > 0;

    if(posible){
        // Sacamos la primera carta
        carta = mazo.mazo[0];
        mazo.cnt--;
        // Movemos las cartas restantes hacia "arriba"
        for(unsigned i = 0; i < mazo.cnt; i++){
            mazo.mazo[i] = mazo.mazo[i+1];
        }
    }

    return posible;
}

bool insertar(tMazo & mazo, const carta::tCarta carta){
    bool posible = mazo.cnt < NUM_CARTAS;
    if(posible){
        mazo.mazo[mazo.cnt] = carta;
        mazo.cnt++;
    }

    return posible;
}
