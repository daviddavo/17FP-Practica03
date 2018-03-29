#include "puntuaciones.h"

// Funcion interna para sort()
bool sorter(const tPuntuacion &p1, const tPuntuacion &p2) {
  // TODO: Si el nombre esta vacio, poner al final de la lista
  return p1.puntos > p2.puntos;
}

bool guardarPuntuaciones(const tPuntuaciones &puntuaciones) {
  std::ofstream file(FILE_PUNTUACIONES);

  for (unsigned i = 0; i < puntuaciones.cnt && !file.fail(); i++)
    file << puntuaciones.puntuaciones[i].nombre << " "
         << puntuaciones.puntuaciones[i].puntos << std::endl;

  file.close();
  return !file.fail();
}

bool cargarPuntuaciones(tPuntuaciones &puntuaciones) {
  std::ifstream file(FILE_PUNTUACIONES);
  puntuaciones.cnt = 0;

  for (unsigned i = 0; i < MAX_PUNTUACIONES; i++) {
    tPuntuacion puntuacion;
    if (file >> puntuacion.nombre >> puntuacion.puntos)
      puntuaciones.puntuaciones[puntuaciones.cnt++] = puntuacion;
  }

  std::sort(puntuaciones.puntuaciones,
            puntuaciones.puntuaciones + puntuaciones.cnt, sorter);

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
    if (puntuaciones.cnt == MAX_PUNTUACIONES) {
      pos = MAX_PUNTUACIONES - 1;
    } else {
      pos = puntuaciones.cnt;
      puntuaciones.cnt++;
    }
    puntuaciones.puntuaciones[pos].nombre = nombre;
    puntuaciones.puntuaciones[pos].puntos = add;
  }

  // Al ser una lista tan pequeña es mas facil ordenarla que crear un metodo
  // "actualizar" Y un metodo insertar
  std::sort(puntuaciones.puntuaciones,
            puntuaciones.puntuaciones + puntuaciones.cnt, sorter);
  return f;
}
