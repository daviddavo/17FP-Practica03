#include "tablero.h"

// Función interna
tCasilla char2casilla(const char c, unsigned& nTortuga) {
  tCasilla casilla;

  // Asumimos que es una tortuga hasta que se demuestre lo contario
  casilla.estado = TORTUGA;

  switch (c) {
    case '#':
      casilla.estado = MURO;
      break;
    case '@':
      casilla.estado = HIELO;
      break;
    case '*':  // Resulta que * es vacía y no lo pone en la práctica
    case ' ':
      casilla.estado = VACIA;
      break;
    case '$':
      casilla.estado = JOYA;
      break;
    case 'C':
      casilla.estado = CAJA;
      break;
    case 'U':
      casilla.tortuga.direccion = NORTE;
      break;
    case 'D':
      casilla.tortuga.direccion = SUR;
      break;
    case 'R':
      casilla.tortuga.direccion = ESTE;
      break;
    case 'L':
      casilla.tortuga.direccion = OESTE;
      break;
      // Y aquí si pudiese levantaría una excepción en default...
  }

  // Si es una tortuga, le asignamos su numero y sumamos uno al contador
  if (casilla.estado == TORTUGA) casilla.tortuga.numero = nTortuga++;

  return casilla;
}

// Se encarga de leer solo el tablero, función interna
bool leerTablero(tTablero tablero, std::ifstream& file) {
  bool fail = false;
  std::string line;
  unsigned nTortuga = 0;

  for (unsigned i = 0; i < MAX_FILAS && !fail; i++) {
    // Leemos la línea
    getline(file, line);
    fail = file.fail() && line.length() >= MAX_FILAS;
    for (unsigned j = 0; j < MAX_FILAS && !fail; j++) {
      // Leemos cada casilla de la línea
      tablero[i][j] = char2casilla(line[j], nTortuga);
    }
  }

  return !fail;
}

bool cargarTablero(tTablero tablero, const std::string fname,
                   const unsigned jugadores) {
  unsigned jugadoresTablero = 0, i = 0;
  std::string linea;
  std::ifstream file(fname);

  bool error = file.fail();
  while (!error && jugadoresTablero != jugadores && i < MAX_JUGADORES) {
    file >> jugadoresTablero;  // Leemos el numero de jugadores
    getline(file, linea);      // Terminamos de leer la linea
    if (jugadoresTablero == jugadores) {
      error = leerTablero(tablero, file);
    } else {
      // Descartamos las MAX_FILAS filas
      for (unsigned k = 0; k < MAX_FILAS; k++) getline(file, linea);
    }

    error = error || file.fail();
    i++;
  }

  file.close();

  return !error && i < MAX_JUGADORES;
}
