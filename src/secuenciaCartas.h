#ifndef SRC_MODULES_MAZO_H_
#define SRC_MODULES_MAZO_H_

#include <algorithm>

const int MAX_CARTAS = 38;
const int MAX_AVANZA = 18;
const int MAX_GIRODERECHA = 8;
const int MAX_GIROIZQUIERDA = 8;
const int MAX_PISTOLALASER = 4;

// NADA sirve para saber el numbero de cartas distintas
typedef enum { AVANZAR, GIROIZQUIERDA, GIRODERECHA, LASER, NADA } tCarta;

typedef tCarta tArrayCarta [MAX_CARTAS];

typedef struct{
	tArrayCarta arrayCarta;
	unsigned contador;
}tMazo;

void crearVacia(tMazo & mazo);
bool sacar(tMazo & mazo, tCarta& carta);
bool insertar(tMazo & mazo, tCarta carta);
void crearMazoOrdenado(tMazo & mazo);
void crearMazoAleatorio(tMazo & mazo);

#endif /* SRC_MODULES_MAZO_H_ */
