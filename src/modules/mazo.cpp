#include "mazo.h";

void crearVacia(tMazo & mazo){
	for(unsigned int i = 0; i < NUM_CARTAS; i++){
		mazo[i] = NADA;
	}
}

void crearMazoAleatorio(tMazo & mazo){
	// Primero rellenamos el array. Recorremos las distintas cartas que existen
	// Y vemos cuantas cartas de ese tipo hay en el array y las metemos
	unsigned j = 0; // Posicion de la carta en el array, al final será tamaño -1
	for(unsigned i = 0; i < NADA; i++){
		for(unsigned k = 0; k < nCartas[i]; k++, j++){
			mazo[j] = static_cast<tCarta>(i);
		}
	}
	// Finalmente, lo desordenamos
	random_shuffle(mazo, mazo + j + 1);
}

bool sacar(tMazo & mazo, tCarta & carta){
	// Sacamos la primera
	carta = mazo[0];
	// Movemos todos uno a la izda
	for(unsigned i = 0; i < NUM_CARTAS-1; i++){
		mazo[i] = mazo[i+1];
	}
	// Rellenamos el último como "nada"
	mazo[NUM_CARTAS-1] = NADA;

	return carta != NADA;
}
