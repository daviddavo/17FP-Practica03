#include <iostream> // Only for testing
#include "mazo.h"

void imprimirMazo(tMazo mazo){
	for(unsigned i = 0; i < NUM_CARTAS; i++){
		std::cout << carta2string(mazo[i]) << std::endl;
	}
}

int main(){
	tMazo mazoPrueba;
	crearMazoAleatorio(mazoPrueba);
	imprimirMazo(mazoPrueba);

	tCarta carta;
	sacar(mazoPrueba, carta);
	std::cout << "#######" << std::endl;
	std::cout << "Sacada " << carta2string(carta) << std::endl;
	imprimirMazo(mazoPrueba);

	std::cout << "#######" << std::endl;
	std::cout << insertar(mazoPrueba, LASER) << std::endl;
	imprimirMazo(mazoPrueba);

	return 0;
}
