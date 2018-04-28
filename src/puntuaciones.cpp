#include "puntuaciones.h"

// Funcion interna para sort()
bool sorter(const tPuntuacion &p1, const tPuntuacion &p2) {
    return p1.puntos > p2.puntos;
}

bool guardarPuntuaciones(const tPuntuaciones &puntuaciones) {
    std::ofstream file(FILE_PUNTUACIONES);

    for (unsigned i = 0; i < puntuaciones.cnt && !file.fail(); i++)
        file << puntuaciones.puntuaciones[i].nombre << " " << puntuaciones.puntuaciones[i].puntos << std::endl;

    file.close();
    return !file.fail();
}

bool cargarPuntuaciones(tPuntuaciones &puntuaciones) {
    std::ifstream file(FILE_PUNTUACIONES);
    puntuaciones.cnt = 0;
	puntuaciones.puntuaciones = new tPuntuacion[INIT_PUNTUACIONES];
	puntuaciones.MAX = INIT_PUNTUACIONES;

	tPuntuacion puntuacion;
	while (file >> puntuacion.nombre >> puntuacion.puntos) {
		if (puntuaciones.cnt == puntuaciones.MAX) redimensionar(puntuaciones);
		puntuaciones.puntuaciones[puntuaciones.cnt++] = puntuacion;
	}

    std::sort(puntuaciones.puntuaciones, puntuaciones.puntuaciones + puntuaciones.cnt, sorter);

    file.close();
    return !file.fail();
}

bool actualizarPuntuacion(tPuntuaciones &puntuaciones, const std::string nombre, const unsigned add) {
    bool f = false;  // Buscamos el jugador
    for (unsigned i = 0; i < puntuaciones.cnt && !f; i++) {
        if (puntuaciones.puntuaciones[i].nombre == nombre) {
            f = true;
            puntuaciones.puntuaciones[i].puntos += add;
        }
    }

    // No lo encontramos, lo añadimos
    if (!f) {
        // Al ser una lista ordenada de mayor a menor, en caso de estar llena
        // La posicion MAX -1 sera la de menor puntuacion
        unsigned pos;
        if (puntuaciones.cnt == puntuaciones.MAX) { // Si la lista está llena
            // if (puntuaciones.puntuaciones[MAX_PUNTUACIONES-1].puntos < add) // Si el menor es menor que el nuevo
            //	pos = MAX_PUNTUACIONES - 1;
			redimensionar(puntuaciones);
        }
        pos = puntuaciones.cnt;
        puntuaciones.cnt++;
        puntuaciones.puntuaciones[pos].nombre = nombre;
        puntuaciones.puntuaciones[pos].puntos = add;
    }

    // Al ser una lista tan pequeña es mas facil ordenarla que crear un metodo
    // "actualizar" Y un metodo insertar
    std::sort(puntuaciones.puntuaciones, puntuaciones.puntuaciones + puntuaciones.cnt, sorter);
    return f;
}

void redimensionar(tPuntuaciones & puntuaciones, const unsigned aumentar) {
	int max = puntuaciones.MAX + aumentar;

	tPuntuacion * aux = NULL;
	aux = new tPuntuacion[puntuaciones.MAX + aumentar];

	for (unsigned i = 0; i < puntuaciones.cnt; i++) {
		aux[i] = puntuaciones.puntuaciones[i];
	}
	liberar(puntuaciones);
	puntuaciones.puntuaciones = aux;
	puntuaciones.MAX = max;
}

void liberar(tPuntuaciones & puntuaciones) {
	delete [] puntuaciones.puntuaciones;
	puntuaciones.MAX = 0;
}