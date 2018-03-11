#include <iostream> // Only for testing
#include <string>

#include "juego.h"
#include "cli.h"

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

    /*
    tTablero tablero;
    cout << "Tablero leido: " << cargarTablero(tablero, "tableros.txt", 3) << endl;
    mostrarTablero(tablero);*/

    /*
    tPuntuaciones puntuaciones;
    cargarPuntuaciones(puntuaciones);
    mostrarPuntuaciones(puntuaciones);
    actualizarPuntuacion(puntuaciones, "davo", 200);
    mostrarPuntuaciones(puntuaciones);
    guardarPuntuaciones(puntuaciones);
    */

    /*
    tJugador jugadores[4];
    for(unsigned i = 1; i < 4+1; i++){
        jugadores[i-1].id = i;
    }
    jugadores[0].nombre = "Juan";
    jugadores[1].nombre = "Pedro";
    jugadores[2].nombre = "Maria";
    jugadores[3].nombre = "Timmy";
    mostrarJugadores(jugadores, 4, 2);
    */

    tJuego juego;
    cargarJuego(juego);
    juego.turno = 0;
    for(unsigned i = 0; i < 4; i++) juego.jugadores[0].mano[i] = 10;
    juego.jugadores[0].nombre = "Esto";
    juego.jugadores[1].nombre = "Es";
    juego.jugadores[2].nombre = "Una_prueba";
    juego.jugadores[3].nombre = "De_movimiento";
    mostrarJuego(juego);
    ejecutarTurno(juego);

	return 0;
}
