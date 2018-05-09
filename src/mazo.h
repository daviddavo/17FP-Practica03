#ifndef SRC_MAZO_H_
#define SRC_MAZO_H_

#ifdef _DEBUG 
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h> 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif

#include <algorithm>
#include <string>

constexpr unsigned CARTAS_DISTINTAS = 5;
namespace carta {
	// Lo metemos en un namespace para no confundirlo con las teclas
	// NADA sirve de "centinela"
	typedef enum { AVANZAR, IZQUIERDA, DERECHA, LASER, BICHO, NADA } tCarta;
	typedef tCarta * tArrayPtrCartas[CARTAS_DISTINTAS];
}  // namespace carta
// De esta manera, si queremos modificar el numero de cartas en el mazo, es
// mucho mas facil
constexpr unsigned nCartas[CARTAS_DISTINTAS]{18, 8, 8, 4, 1};
// Tenemos una funcion sum() que calcula en tiempo de compilacion el NUM_CARTAS
constexpr int sum(const unsigned arr[], const unsigned size) {
    int sum = 0;
    for (unsigned i = 0; i < size; i++) sum += arr[i];
    return sum;
}

constexpr int NUM_CARTAS = sum(nCartas, CARTAS_DISTINTAS);

// Vamos a implementar tMazo como una cola, implementada como un array circular
typedef struct {
    carta::tCarta* mazo[NUM_CARTAS];
    int cnt;  // Inicializar a 0 en crearVacia
    int bot;  // Donde van a entrar los nuevos elementos a la cola
    int top;  // Donde van a salir los nuevos elementos en la cola
} tMazo;

// Como no podemos meter más cartas de las que hemos sacado, prescindiremos de funciones como
// bool llena() o bool vacia()

void crearVacia(tMazo &);
void crearMazoOrdenado(tMazo &);
void crearMazoAleatorio(tMazo &);
bool sacar(tMazo &, carta::tCarta &, bool = true);          // El booleano indica si queremos insertarlo por arriba
bool insertar(tMazo &, const carta::tCarta, bool = false);  // Idem
std::string carta2str(const carta::tCarta);
void liberar(tMazo &);										// Libera el espacio dedicado a un mazo

#endif  // SRC_MAZO_H_
