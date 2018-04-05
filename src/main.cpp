// #include <locale.h>
#include <time.h>  // semilla

#include <algorithm>  // srand

#include "interface.h"

// TODO: Borrar, solo pruebas
/*
#include "mazo.h"
using namespace std;
#include "interface.h"
*/

int main() {
    // Necesario para random_shuffle
    srand(time(NULL));

    mainMenu();

    // while(cout << _getch() << endl) {}  // Muestra la tecla pulsada

    /*
    tMazo mazo;
    crearVacia(mazo);
    carta::tCarta carta = carta::NADA;
    for (int i = 0; i < 5; i++) {
        insertar(mazo, carta::LASER);
    }
    */

    return 0;
}
