#include "mazo.h"

void crearVacia(tMazo &mazo) {
    mazo.cnt = mazo.top = 0;
    mazo.bot = -1;
}

void crearMazoOrdenado(tMazo &mazo) {
    // Primero rellenamos el array. Recorremos las distintas cartas que existen
    // vemos cuantas cartas de ese tipo hay en nCartas y las metemos
    // Se que es un metodo un tanto raro, pero me parece mejor y mas escalable
    // que hacer N for's
    crearVacia(mazo);
    unsigned j = 0;  // Posicion de la carta en el array
    for (unsigned i = 0; i < carta::NADA; i++) {
        for (unsigned k = 0; k < nCartas[i]; k++, j++) {
            // mazo.mazo[j] = static_cast<carta::tCarta>(i);
            insertar(mazo, static_cast<carta::tCarta>(i));
        }
    }
}

void crearMazoAleatorio(tMazo &mazo) {
    // Creamos un mazo ordenado y lo barajamos
    crearMazoOrdenado(mazo);
    // Solo se va a poder 'barajar' si se acaba de crear el mazo, en el momento en
    // el que begin != 0 ya no funcionaría
    std::random_shuffle(mazo.mazo, mazo.mazo + NUM_CARTAS);
}

bool sacar(tMazo &mazo, carta::tCarta &carta, bool superior /* = true */) {
    bool posible = mazo.cnt > 0;

    if (posible) {
        // Si solo hay un elemento, volvemos al '-1'
        if (mazo.bot == mazo.top) {
            carta = mazo.mazo[mazo.bot];  // La carta es ese unico elemento
            // TODO: Debug, borrar lo de NADA
            mazo.mazo[mazo.bot] = carta::NADA;
            mazo.top = mazo.bot = -1;  // Ambos son '-1'
        } else if (superior) {
            carta = mazo.mazo[mazo.top];
            mazo.mazo[mazo.top] = carta::NADA;
            if (mazo.top == 0)
                mazo.top = NUM_CARTAS - 1;
            else
                mazo.top--;
        } else {
            carta = mazo.mazo[mazo.bot];
            mazo.mazo[mazo.bot] = carta::NADA;
            if (mazo.bot == NUM_CARTAS - 1)
                mazo.bot = 0;
            else
                mazo.bot++;
        }

        mazo.cnt--;
    }

    return posible;
}

bool insertar(tMazo &mazo, const carta::tCarta carta, bool superior /* = false */) {
    // Si el bicho ha sido usado, el tamaño máximo se reduce en uno
    bool posible = mazo.cnt < NUM_CARTAS;
    if (posible) {
        // Caso 'inicial': La deque se acaba de crear (independiente superior o inferior)
        if (mazo.bot == -1) {
            mazo.top = 0;
            mazo.bot = 0;
            mazo.mazo[0] = carta;
        } else if (superior) {
            // Damos la vuelta
            if (mazo.top == NUM_CARTAS - 1)
                mazo.top = 0;
            else
                mazo.top++;

            mazo.mazo[mazo.top] = carta;
        } else {
            // Damos 'la vuelta' por abajo
            if (mazo.bot == 0) mazo.bot = NUM_CARTAS - 1;
            // Inserción normal
            else
                mazo.bot--;

            mazo.mazo[mazo.bot] = carta;
        }

        mazo.cnt++;
    }

    return posible;
}

std::string carta2str(const carta::tCarta carta) {
    switch (carta) {
        case carta::AVANZAR:
            return "^";
        case carta::IZQUIERDA:
            return "<";
        case carta::DERECHA:
            return ">";
        case carta::LASER:
            return "~";
        case carta::BICHO:
            return "&";
        default:
            return "?";
    }
}
