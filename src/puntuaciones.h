#ifndef SRC_MODULES_PUNTUACIONES_H_
#define SRC_MODULES_PUNTUACIONES_H_

#include <string>
#include <algorithm> // min
const unsigned MAX_JUGADORES = 4;

typedef struct {
    std::string nombre;
    unsigned puntos;
} tPuntuacion;
typedef tPuntuacion tPuntuaciones[MAX_JUGADORES];

bool operator<(const tPuntuacion &, const tPuntuacion &);
bool cargarPuntuaciones(tPuntuaciones &);
void guardarPuntuaciones(const tPuntuaciones &);
void mostrarPuntuaciones(const tPuntuaciones &);
bool actualizarPuntuacion(tPuntuaciones &, const std::string, const unsigned);

#endif /* SRC_MODULES_PUNTUACIONES_H_ */
