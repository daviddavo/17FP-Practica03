#include <iostream> // Only for testing
#include "secuenciaCartas.h"

void imprimirMazo(tMazo mazo){
	for(unsigned i = 0; i < mazo.contador; i++){
		std::cout << mazo.arrayCarta[i] << std::endl;
	}
}

int main(){
	tMazo mazoPrueba;
	crearMazoAleatorio(mazoPrueba);
	imprimirMazo(mazoPrueba);

	tCarta carta;
	sacar(mazoPrueba, carta);
	std::cout << "#######" << std::endl;
	std::cout << "Sacada " << carta << std::endl;
	imprimirMazo(mazoPrueba);

	std::cout << "#######" << std::endl;
	std::cout << insertar(mazoPrueba, LASER) << std::endl;
	imprimirMazo(mazoPrueba);

	return 0;
}
