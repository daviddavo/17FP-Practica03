#ifndef SRC_MODULES_PUNTUACIONES_H_
#define SRC_MODULES_PUNTUACIONES_H_

const unsigned MAX_JUGADORES = 4;
const std::string FILE_PUNT = "puntuaciones.txt";
// No es necesario usar NUM_LINEAS si usamos .eof()

typedef struct {
    std::string nombre;
    unsigned puntos;
} tPuntuacion;
typedef tPuntuacion tPuntuaciones[MAX_JUGADORES];

bool operator<(const tPuntuacion &, const tPuntuacion &); // Usado internamente, pero puede ser usado fuera?
bool cargarPuntuaciones(tPuntuaciones &); // Carga puntuaciones del fichero
void guardarPuntuaciones(const tPuntuaciones &); // Guarda las puntuaciones al fichero
void mostrarPuntuaciones(const tPuntuaciones &); // Muestra las puntuaciones en pantalla
bool actualizarPuntuacion(tPuntuaciones &, const std::string, const unsigned); // Actualiza las puntuaciones

#endif /* SRC_MODULES_PUNTUACIONES_H_ */
