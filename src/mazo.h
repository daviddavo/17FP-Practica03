#ifndef SRC_MAZO_H_
#define SRC_MAZO_H_

#include <algorithm>
#include <string>

constexpr unsigned CARTAS_DISTINTAS = 4;
namespace carta {
// Lo metemos en un namespace para no confundirlo con las teclas
// NADA sirve de "centinela"
typedef enum { AVANZAR, IZQUIERDA, DERECHA, LASER, NADA } tCarta;
}  // namespace carta
// De esta manera, si queremos modificar el numero de cartas en el mazo, es
// mucho más fácil
constexpr unsigned nCartas[carta::NADA]{18, 8, 8, 4};
// Tenemos una función sum() que calcula en tiempo de compilación el NUM_CARTAS
constexpr unsigned sum(const unsigned arr[], const unsigned size) {
  unsigned sum = 0;
  for (unsigned i = 0; i < size; i++) sum += arr[i];
  return sum;
}

constexpr unsigned NUM_CARTAS = sum(nCartas, CARTAS_DISTINTAS);

typedef struct {
  carta::tCarta mazo[NUM_CARTAS];
  unsigned cnt;  // Inicializar a 0 en crearVacia
} tMazo;

void crearVacia(tMazo &);
void crearMazoOrdenado(tMazo &);
void crearMazoAleatorio(tMazo &);
bool sacar(tMazo &, carta::tCarta &);
bool insertar(tMazo &, const carta::tCarta);
std::string carta2str(const carta::tCarta);

#endif  // SRC_MAZO_H_
