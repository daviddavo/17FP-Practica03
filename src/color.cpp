#include "color.h"

#ifdef _WIN32
#include <windows.h>

const WORD COLOR_MASK = 0b00001111;

void colorFondo(const unsigned color) {
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO bufferinfo;
  // Esta funcion recibe como argumento un puntero, por lo que pondremos &
  GetConsoleScreenBufferInfo(handle, &bufferinfo);
  SetConsoleTextAttribute(handle,
                          (bufferinfo.wAttributes & COLOR_MASK) | (color << 4));
}

void colorTexto(const unsigned color) {
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO bufferinfo;
  GetConsoleScreenBufferInfo(handle, &bufferinfo);
  SetConsoleTextAttribute(handle,
                          color | (bufferinfo.wAttributes & ~COLOR_MASK));
}

// Alias
void colorReset() {
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(handle, COLOR_MASK);
}

void clear() { system("cls"); }

void move(const unsigned line, const unsigned col) { 
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD p = { col-1, line-1 };  // En windows es al reves, y empieza en 0
    SetConsoleCursorPosition(handle, p);
}

#elif __linux__
#include <iostream>

// Nota: W10 da soporte a la escritura de caracteres especiales en la terminal
// desde 2016
void colorFondo(const unsigned color) {
  std::cout << "\e[" << w2l[color] << "m";
}

void colorTexto(const unsigned color) {
  std::cout << "\e[" << (w2l[color] - 10) << "m";
}

// 49-> reset back, 39 -> reset fore, 0 -> reset all
void colorReset() { std::cout << "\e[49;" << (w2l[COLOR_TEXTO] - 10) << "m"; }

void clear() {
  system("clear");
  move(0, 0);
}

void move(const unsigned line, const unsigned col) {
  std::cout << "\e[" << line << ";" << col << "f";
}
#endif
