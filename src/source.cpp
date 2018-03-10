#include <iostream> // Only for testing
#include "tablero.h"

using namespace std;

int main(){
    /*
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
	*/

    tTablero tablero;
    cout << "Tablero leido: " << cargarTablero(tablero, "tableros.txt", 3) << endl;
    mostrarTablero(tablero);

	return 0;
}
