#include "color.h"

#ifdef _WIN32
    #include <windows.h>

    /* Nota sobre Windows API:
     * la función SetConsoleTextAttribute recibe de parámetro un WORD (byte) en el cual el
     * primer nibble es el color del texto, y el segundo el color de fondo
     * Para establecer el color de fondo usaremos primero GetConsoleScreenBufferInfo para
     * saber cual es el color del texto actual, y no cambiarlo, ídem para establecer el color
     * del texto. Para eliminar los 4 bits finales (o iniciales) usaremos la máscara
     * 00001111 o !00001111 definida como constante local de este módulo
     */

    const WORD COLOR_MASK = 0b00001111;

    void colorFondo(const unsigned color){
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO bufferinfo;
		// Esta función recibe como argumento un puntero, por lo que pondremos &
        GetConsoleScreenBufferInfo(handle, &bufferinfo);
        SetConsoleTextAttribute(handle, (bufferinfo.wAttributes & COLOR_MASK) | (color << 4));
    }

	void colorTexto(const unsigned color) {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO bufferinfo;
		GetConsoleScreenBufferInfo(handle, &bufferinfo);
		SetConsoleTextAttribute(handle, color | (bufferinfo.wAttributes & ~COLOR_MASK));
	}

	// Alias
	void colorReset() {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, COLOR_MASK);
	}

    void clear(){
        system("cls");
    }

    void move(const unsigned line, const unsigned col){
    	SetCursorPos(line, col);
    }
#elif __linux__
    #include <iostream>
    // ESTAS FUNCIONES TAMBIÉN FUNCIONARÍAN EN WINDOWS 10 2015

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
        move(0, 0);
    }

    void move(const unsigned line, const unsigned col){
        std::cout << "\e[" << line << ";" << col << "f";
    }

#endif
