// #include <locale.h>
#include <time.h>  // semilla

#include <algorithm>  // srand

#include "interface.h"

int main() {
    // Necesario para random_shuffle
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    srand(time(NULL));

    mainMenu();

    // while(cout << _getch() << endl) {}  // Muestra la tecla pulsada

    return 0;
}
