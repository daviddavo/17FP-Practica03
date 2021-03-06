// #include <locale.h>
#include <time.h>  // semilla

#include <algorithm>  // srand

#include "interface.h"

int main() {
    // Necesario para random_shuffle
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    srand(time(NULL));

    mainMenu();

    // while (std::cout << _getch() << std::endl);  // Muestra la tecla pulsada

    return 0;
}
