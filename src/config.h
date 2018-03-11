#ifndef SRC_CONFIG_H_
#define SRC_CONFIG_H_

// CABECERA CON TODAS LAS CONFIGURACIONES EXPLICADAS

const unsigned MAX_FILAS = 8; // Numero de filas de los mapas
const unsigned MAX_JUGADORES = 4;
const unsigned NUM_TIPOS_CASILLAS = 6; // Numero de casillas distintas
const unsigned LOG_SIZE = 7; // Tamaño del buffer de mensajes

const unsigned paleta[NUM_TIPOS_CASILLAS+MAX_JUGADORES] = {1,11,7,4,12,5,13,9,10,3};
const unsigned COLOR_MANO = 1;

#endif /* SRC_CONFIG_H_ */
