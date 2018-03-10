#include "tablero.h" // Para la NUM_TIPOS_CASILLAS
#include "puntuaciones.h" // Para MAX_JUGADORES
#include "color.h"

#ifdef _WIN32_
    #include <windows.h>

    void colorFondo(const unsigned color){
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, 15 | (color << 4));
    }
#elif __linux__
    #include <iostream>

    void colorFondo(const unsigned color){
        std::cout << "\e[" << w2l[color] << "m";
    }

    void colorReset(){
        std::cout << "\e[0m";
    }

#endif
