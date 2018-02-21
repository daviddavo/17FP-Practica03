#include <algorithm>

#ifndef SRC_MODULES_MAZO_H_
#define SRC_MODULES_MAZO_H_

// Definimos las cartas y la cantidad de cada uno
constexpr unsigned CARTAS_DISTINTAS = 4;
typedef enum {AVANZAR, GIROIZQUIERDA, GIRODERECHA, LASER, NADA} tCarta;
constexpr unsigned nCartas[NADA] {18, 8, 8, 4};

// En caso de cambiar el numero de cartas, no es necesario cambiar nada más que las dos cosas anteriores
constexpr unsigned sum(const unsigned arr[]){
	unsigned sum = 0;
	for(unsigned i = 0; i < NADA; i++) sum += arr[i];
	return sum;
}
constexpr unsigned NUM_CARTAS = sum(nCartas);

typedef tCarta tMazo[NUM_CARTAS];

void crearVacia(tMazo &);
void crearMazoAleatorio(tMazo &);
bool sacar (tMazo  &, tCarta &);

#endif /* SRC_MODULES_MAZO_H_ */