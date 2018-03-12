#include "color.h"

#ifdef _WIN32_
    #include <windows.h>

    void colorFondo(const unsigned color){
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, 15 | (color << 4));
    }

    void clear(){
        system("cls");
    }
#elif __linux__
    #include <iostream>
    // ESTAS FUNCIONES TAMBIÉN FUNCIONAN EN WINDOWS 10

    void colorFondo(const unsigned color){
        std::cout << "\e[" << w2l[color] << "m";
    }

    void colorTexto(const unsigned color){
        std::cout << "\e[" << (w2l[color]-10) << "m";
    }

    // 49-> reset back, 39 -> reset fore, 0 -> reset all
    void colorReset(){
        std::cout << "\e[49;" << (w2l[COLOR_TEXTO]-10) << "m";
    }

    void clear(){
        system("clear");
    }

#endif
