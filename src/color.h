/*
 * color.h
 *
 *  Created on: 24 mar. 2018
 *      Author: davo
 */

#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

#include "config.h"

const unsigned w2l[16] = {
    40,   // Negro
    44,   // Azul
    42,   // Verde
    46,   // Aqua -> Cyan
    41,   // Rojo
    45,   // Purple / Magenta
    43,   // Amarillo
    47,   // Blanco -> gris claro
    100,  // Gris -> gris oscuro
    104,  // Azul claro
    102,  // Verde claro
    106,  // Aqua claro -> cyan claro
    101,  // Rojo claro
    105,  // Morado claro -> Rosa
    103,  // Amarillo claro
    107,  // Blanco claro -> Blanco
};        // Reset es el 49

void colorFondo(const unsigned);            // Cambia el color del fondo (manteniendo el texto)
void colorTexto(const unsigned);            // Cambia el color del texto (manteniendo el fondo)
void colorReset();                          // Pone los colores por defecto
void clear();                               // Limpia la pantalla
void move(const unsigned, const unsigned);  // Mueve el cursor a una posicion

#endif  // SRC_COLOR_H_
