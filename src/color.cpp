#include "color.h"

#ifdef _WIN32_
    #include <windows.h>

    void colorFondo(int color){
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, 15 | (color << 4));
    }
#elif __linux__
    #include <iostream>

    void colorFondo(int color){
        std::cout << "\e[" << paleta[color] << "m";
    }

#endif
