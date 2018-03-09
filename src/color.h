/*
 * color.h
 *
 *  Created on: 9 mar. 2018
 *      Author: davo
 */

#ifndef SRC_COLOR_H_
#define SRC_COLOR_H_

const int paleta[NUM_TIPOS_CASILLAS+MAX_JUGADORES] = {1,11,7,4,12,5,13,9,10,3};

// ARRAY PARA CONVERTIR LA PALETA DE WINDOWS A LINUX
// Solo en Background, foreground = background - 10
unsigned w2l[16] = {
    40, // Negro
    44, // Azul
    42, // Verde
    46, // Aqua -> Cyan
    41, // Rojo
    45, // Purple / Magenta
    43, // Amarillo
    47, // Blanco -> gris claro
    100, // Gris -> gris oscuro
    104, // Azul claro
    102, // Verde claro
    106, // Aqua claro -> cyan claro
    101, // Rojo claro
    105, // Morado claro -> Rosa
    103, // Amarillo claro
    107, // Blanco claro -> Blanco
}; // Reset es el 49

#endif /* SRC_COLOR_H_ */
