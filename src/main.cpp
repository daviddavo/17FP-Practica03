#include <algorithm>
#include <time.h>
#include <iostream>
// LIBRERIAS DE PRUEBAS
#include "tablero.h"

// ACUERDATE DE BORRAR ESTO: SE SUPONE QUE NO HEMOS DADO TEMPLATES
template <typename T, size_t N> void printArray(const T (&array)[N]){
    for(unsigned i = 0; i < N; i++){
        std::cout << array[i] << std::endl;
    }
}

int main(){
    // Necesario para random_shuffle
    srand(time(NULL));

    // TEST MAZO
    /*
    tMazo mazo;
    carta::tCarta carta;
    crearMazoAleatorio(mazo);
    printArray(mazo.mazo);
    */

    // TEST TABLERO
    tTablero tablero;
    bool fail = cargarTablero(tablero, "tableros.txt", 3);

    return 0;
}
